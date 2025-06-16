#include "../include/fat.h"
#include "../include/disk.h"
#include "../include/terminal.h"
#include "../include/lib/util.h"
#include "../include/lib/string.h"
#include <stdint.h>
#include <stddef.h>


uint16_t current_directory_cluster = 0;  // 0 means root directory


typedef struct {
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t root_entry_count;
    uint16_t sectors_per_fat;

    uint16_t root_dir_sectors;
    uint32_t first_root_sector;
    uint32_t first_data_sector;
} fat16_info_t;

fat16_info_t fat_info;

void fat16_init() {
    uint8_t sector[512];
    ata_read_sector(0, sector);  // Read boot sector at LBA 0

    // Safety check: valid FAT signature?
    if (sector[510] != 0x55 || sector[511] != 0xAA) {
        terminal_print("Invalid boot sector signature (expected 0x55AA)\n");
        return;
    }

    uint16_t bytes_per_sector     = *(uint16_t*)&sector[11];
    uint8_t  sectors_per_cluster  = sector[13];
    uint16_t reserved_sectors     = *(uint16_t*)&sector[14];
    uint8_t  num_fats             = sector[16];
    uint16_t root_entry_count     = *(uint16_t*)&sector[17];
    uint16_t sectors_per_fat      = *(uint16_t*)&sector[22];

    // Guard against zero division
    if (bytes_per_sector == 0 || sectors_per_cluster == 0 || sectors_per_fat == 0) {
        terminal_print("Invalid values in boot sector (possibly corrupted or wrong FS format)\n");
        return;
    }

    fat_info.bytes_per_sector = bytes_per_sector;
    fat_info.sectors_per_cluster = sectors_per_cluster;
    fat_info.reserved_sectors = reserved_sectors;
    fat_info.num_fats = num_fats;
    fat_info.root_entry_count = root_entry_count;
    fat_info.sectors_per_fat = sectors_per_fat;

    fat_info.root_dir_sectors = ((root_entry_count * 32) + (bytes_per_sector - 1)) / bytes_per_sector;
    fat_info.first_root_sector = reserved_sectors + (num_fats * sectors_per_fat);
    fat_info.first_data_sector = fat_info.first_root_sector + fat_info.root_dir_sectors;

    terminal_printf("FAT16 FS Initialized:\n");
    terminal_printf("  Bytes/Sector: %d\n", bytes_per_sector);
    terminal_printf("  Sectors/Cluster: %d\n", sectors_per_cluster);
    terminal_printf("  Reserved: %d\n", reserved_sectors);
    terminal_printf("  Num FATs: %d\n", num_fats);
    terminal_printf("  Root Entries: %d\n", root_entry_count);
    terminal_printf("  Sectors/FAT: %d\n", sectors_per_fat);
    terminal_printf("  Root Start LBA: %d\n", fat_info.first_root_sector);
    terminal_printf("  Data Start LBA: %d\n", fat_info.first_data_sector);
}


void fat16_list_root() {
    int entries = fat_info.root_entry_count;
    int bytes_per_sector = fat_info.bytes_per_sector;
    int root_dir_sectors = fat_info.root_dir_sectors;

    uint8_t sector[512];

    for (int i = 0; i < root_dir_sectors; i++) {
        ata_read_sector(fat_info.first_root_sector + i, sector);

        for (int j = 0; j < bytes_per_sector; j += 32) {
            uint8_t first_byte = sector[j];

            if (first_byte == 0x00) return; // No more entries
            if (first_byte == 0xE5) continue; // Deleted
            if ((sector[j + 11] & 0x0F) == 0x0F) continue; // Long filename

            uint8_t attr = sector[j + 11];
            if (attr & 0x08) continue; // Volume label
            // if (attr & 0x10) continue; // Directory — keep if you want only files

            // Name and extension
            char name[9] = {0};
            char ext[4] = {0};

            for (int k = 0; k < 8; k++) name[k] = sector[j + k];
            for (int k = 0; k < 3; k++) ext[k] = sector[j + 8 + k];

            // Trim trailing spaces
            for (int k = 7; k >= 0 && name[k] == ' '; k--) name[k] = '\0';
            for (int k = 2; k >= 0 && ext[k] == ' '; k--) ext[k] = '\0';

            terminal_print("  ");
            terminal_print(name);
            if (ext[0]) {
                terminal_print(".");
                terminal_print(ext);
            }
            terminal_print("\n");
        }
    }
}

uint16_t fat16_get_next_cluster(uint16_t cluster) {
    uint16_t fat_sector = fat_info.reserved_sectors + (cluster * 2) / fat_info.bytes_per_sector;
    uint16_t offset = (cluster * 2) % fat_info.bytes_per_sector;

    uint8_t sector[512];
    ata_read_sector(fat_sector, sector);
    return *(uint16_t*)&sector[offset];
}

int fat16_read_file(const char* target_name) {
    uint8_t sector[512];
    char name[13] = {0}; // 8+1+3+1

    int root_sectors = fat_info.root_dir_sectors;

    for (int i = 0; i < root_sectors; i++) {
        ata_read_sector(fat_info.first_root_sector + i, sector);

        for (int j = 0; j < fat_info.bytes_per_sector; j += 32) {
            uint8_t first_byte = sector[j];
            if (first_byte == 0x00) {
                terminal_print("File not found\n"); 
                return 0;
            }
            if (first_byte == 0xE5) continue;
            if ((sector[j + 11] & 0x0F) == 0x0F) continue;

            // Extract and format name
            int p = 0;
            for (int k = 0; k < 8 && sector[j + k] != ' '; k++) {
                name[p++] = sector[j + k];
            }

            // Add dot and extension if present
            if (sector[j + 8] != ' ') {
                name[p++] = '.';
                for (int k = 0; k < 3 && sector[j + 8 + k] != ' '; k++) {
                    name[p++] = sector[j + 8 + k];
                }
            }

            name[p] = '\0';

            // Simple case-insensitive match
            const char* a = name;
            const char* b = target_name;
            int match = 1;
            while (*a && *b) {
                char ca = (*a >= 'a' && *a <= 'z') ? *a - 32 : *a;
                char cb = (*b >= 'a' && *b <= 'z') ? *b - 32 : *b;
                if (ca != cb) { match = 0; break; }
                a++; b++;
            }
            if (*a != *b) match = 0;

            if (match) {
                uint16_t cluster = *(uint16_t*)&sector[j + 26];
                uint32_t size = *(uint32_t*)&sector[j + 28];

                char buf[513];
                while (cluster >= 0x0002 && cluster < 0xFFF8) {
                    uint32_t lba = fat_info.first_data_sector + (cluster - 2) * fat_info.sectors_per_cluster;
                    ata_read_sector(lba, (uint8_t*)buf);
                    buf[512] = '\0';

                    for (int i = 0; i < 512 && size > 0; i++, size--) {
                        char str[2] = {buf[i], '\0'};
                        terminal_print(str);
                    }

                    cluster = fat16_get_next_cluster(cluster);
                }

                terminal_print("\n");
                return 1;
            }
        }
    }

    terminal_print("File not found.\n");
    return 0;
}


uint16_t fat16_find_free_cluster() {
    uint8_t sector[512];
    uint16_t total_clusters = fat_info.sectors_per_fat * (fat_info.bytes_per_sector / 2);
    for (uint16_t cluster = 2; cluster < total_clusters; cluster++) {
        uint16_t fat_offset = cluster * 2;
        uint16_t sector_num = fat_info.reserved_sectors + (fat_offset / 512);
        uint16_t offset = fat_offset % 512;

        ata_read_sector(sector_num, sector);
        uint16_t val = *(uint16_t*)&sector[offset];
        if (val == 0x0000) {
            return cluster;
        }
    }
    return 0;  // No free cluster
}

void fat16_mark_cluster(uint16_t cluster, uint16_t value) {
    uint8_t sector[512];
    uint16_t fat_offset = cluster * 2;
    uint16_t sector_num = fat_info.reserved_sectors + (fat_offset / 512);
    uint16_t offset = fat_offset % 512;

    ata_read_sector(sector_num, sector);
    *(uint16_t*)&sector[offset] = value;
    ata_write_sector(sector_num, sector);
}

int fat16_find_free_dir_entry(uint8_t* out_sector, uint8_t* out_offset) {
    uint8_t sector[512];
    for (int i = 0; i < fat_info.root_dir_sectors; i++) {
        uint32_t lba = fat_info.first_root_sector + i;
        ata_read_sector(lba, sector);

        for (int j = 0; j < 512; j += 32) {
            if (sector[j] == 0x00 || sector[j] == 0xE5) {
                *out_sector = i;
                *out_offset = j;
                return 1;
            }
        }
    }
    return 0;
}

void fat16_create_file(const char* name, const char* ext, const char* content, uint32_t size) {
    uint16_t cluster = fat16_find_free_cluster();
    if (cluster == 0) {
        terminal_print("No free clusters.\n");
        return;
    }

    fat16_mark_cluster(cluster, 0xFFFF);  // EOF

    uint8_t sec_index, offset;
    if (!fat16_find_free_dir_entry(&sec_index, &offset)) {
        terminal_print("No free dir entry.\n");
        return;
    }

    uint8_t dir_sector[512];
    ata_read_sector(fat_info.first_root_sector + sec_index, dir_sector);

    memset(&dir_sector[offset], 0, 32);
    for (int i = 0; i < 8 && name[i]; i++) dir_sector[offset + i] = name[i];
    for (int i = 0; i < 3 && ext[i]; i++) dir_sector[offset + 8 + i] = ext[i];
    dir_sector[offset + 11] = 0x20;  // Archive (regular file)
    *(uint16_t*)&dir_sector[offset + 26] = cluster;
    *(uint32_t*)&dir_sector[offset + 28] = size;

    ata_write_sector(fat_info.first_root_sector + sec_index, dir_sector);

    // Write file content
    uint32_t lba = fat_info.first_data_sector + (cluster - 2);
    uint8_t file_sector[512] = {0};
    for (uint32_t i = 0; i < size && i < 512; i++) {
        file_sector[i] = content[i];
    }
    ata_write_sector(lba, file_sector);

    terminal_printf("File %s.%s created\n", name, ext);
}

void fat16_mkdir(const char* dirname) {
    if (!dirname || !dirname[0]) {
        terminal_print("Usage: mkdir NAME\n");
        return;
    }

    char name[8] = { ' ' };
    for (int i = 0; i < 8 && dirname[i] && dirname[i] != '.'; i++) {
        name[i] = dirname[i];
    }

    uint16_t cluster = fat16_find_free_cluster();
    if (!cluster) {
        terminal_print("No free clusters for new directory.\n");
        return;
    }

    fat16_mark_cluster(cluster, 0xFFFF);

    // Create '.' and '..' entries in the directory
    uint8_t dir_data[512] = {0};

    // '.' entry
    for (int i = 0; i < 8; i++) dir_data[i] = '.';  // name
    dir_data[11] = 0x10;                            // attribute: directory
    *(uint16_t*)&dir_data[26] = cluster;            // start cluster
    *(uint32_t*)&dir_data[28] = 0;                  // size

    // '..' entry
    dir_data[32 + 0] = '.';
    dir_data[32 + 1] = '.';
    dir_data[32 + 11] = 0x10;
    *(uint16_t*)&dir_data[32 + 26] = 0;             // parent cluster (root = 0)
    *(uint32_t*)&dir_data[32 + 28] = 0;

    // Write directory sector
    uint32_t lba = fat_info.first_data_sector + (cluster - 2);
    ata_write_sector(lba, dir_data);

    // Create entry in root directory
    uint8_t sector[512];
    for (int i = 0; i < fat_info.root_dir_sectors; i++) {
        uint32_t root_lba = fat_info.first_root_sector + i;
        ata_read_sector(root_lba, sector);
        for (int j = 0; j < 512; j += 32) {
            if (sector[j] == 0x00 || sector[j] == 0xE5) {
                for (int k = 0; k < 8; k++) sector[j + k] = name[k];
                for (int k = 8; k < 11; k++) sector[j + k] = ' ';
                sector[j + 11] = 0x10; // directory
                *(uint16_t*)&sector[j + 26] = cluster;
                *(uint32_t*)&sector[j + 28] = 0;
                ata_write_sector(root_lba, sector);
                terminal_printf("Directory '%s' created.\n", dirname);
                return;
            }
        }
    }

    terminal_print("No free root dir entry for mkdir.\n");
}

int fat16_cd(const char* dirname) {
        if (!dirname || !dirname[0]) {
        terminal_print("Usage: cd <dir>\n");
        return 0;
    }

    // ✅ Handle special case: cd ..
    if (strcmp(dirname, "..") == 0) {
        if (current_directory_cluster == 0) {
            terminal_print("Already in root.\n");
            return 1;
        }

        // Read the current directory cluster and find ".."
        uint8_t sector[512];
        uint32_t lba = fat_info.first_data_sector + (current_directory_cluster - 2) * fat_info.sectors_per_cluster;
        ata_read_sector(lba, sector);

        uint16_t parent = *(uint16_t*)&sector[32 + 26];  // ".." is second entry
        current_directory_cluster = parent;
        terminal_printf("Moved up to parent directory (cluster %d)\n", parent);
        return 1;
    }
    uint8_t sector[512];
    char name[9] = {0};

    // Copy and pad name (up to 8 chars, no extension)
    for (int i = 0; i < 8 && dirname[i]; i++) {
        name[i] = dirname[i];
    }
    for (int i = strlen(dirname); i < 8; i++) {
        name[i] = ' ';
    }

    int entries_per_sector = fat_info.bytes_per_sector / 32;
    int total_sectors = (current_directory_cluster == 0)
        ? fat_info.root_dir_sectors
        : fat_info.sectors_per_cluster;

    for (int i = 0; i < total_sectors; i++) {
        uint32_t lba = (current_directory_cluster == 0)
            ? fat_info.first_root_sector + i
            : fat_info.first_data_sector + (current_directory_cluster - 2) * fat_info.sectors_per_cluster + i;

        ata_read_sector(lba, sector);

        for (int j = 0; j < fat_info.bytes_per_sector; j += 32) {
            if (sector[j] == 0x00 || sector[j] == 0xE5) continue;
            if ((sector[j + 11] & 0x0F) == 0x0F) continue;

            if (!(sector[j + 11] & 0x10)) continue; // not a directory

            int match = 1;
            for (int k = 0; k < 8; k++) {
                if (name[k] != sector[j + k]) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                uint16_t cluster = *(uint16_t*)&sector[j + 26];
                current_directory_cluster = cluster;
                terminal_printf("Changed directory to %s (cluster %d)\n", dirname, cluster);
                return 1;
            }
        }
    }

    terminal_printf("Directory '%s' not found.\n", dirname);
    return 0;
}
