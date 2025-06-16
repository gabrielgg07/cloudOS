#include <stdint.h>
#include "../../include/port.h"
#include "../../include/terminal.h"
#include "../../include/disk.h"

void ata_identify() {
    outb(0x1F6, 0xA0); // Select master drive (bit 4 = master)
    outb(0x1F2, 0);    // Sector count = 0
    outb(0x1F3, 0);    // LBA low = 0
    outb(0x1F4, 0);    // LBA mid = 0
    outb(0x1F5, 0);    // LBA high = 0
    outb(0x1F7, 0xEC); // Command: IDENTIFY

    if (inb(0x1F7) == 0) {
        terminal_print("No disk connected.\n");
        return;
    }

    // Wait for BSY to clear and DRQ to set
    uint8_t status;
    while ((status = inb(0x1F7)) & 0x80); // Wait for BSY = 0
    if (!(status & 0x08)) {
        terminal_print("Disk not ready.\n");
        return;
    }

    // Read 256 words (512 bytes) from data port
    uint16_t buffer[256];
    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(0x1F0);
    }

    uint32_t total_sectors = buffer[60] | ((uint32_t)buffer[61] << 16);
    uint32_t size_mb = total_sectors / 2048; // 512B per sector

    terminal_print("Disk Detected:\n");
    terminal_print("  Total Sectors: ");
    terminal_print_int(total_sectors);
    terminal_print("\n  Size (MB): ");
    terminal_print_int(size_mb);
    terminal_print(" MB\n");
}


void ata_read_sector(uint32_t lba, uint8_t* buffer) {
    // Wait for drive to be ready
    while (inb(0x1F7) & 0x80); // BSY

    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F)); // Select master + high LBA bits
    outb(0x1F2, 1); // Sector count (1 sector)
    outb(0x1F3, (uint8_t)lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x20); // READ SECTORS command

    // Wait for DRQ
    while (!(inb(0x1F7) & 0x08)); // DRQ

    // Read 512 bytes (256 words)
    for (int i = 0; i < 256; i++) {
        uint16_t word = inw(0x1F0);
        buffer[i * 2] = (uint8_t)(word & 0xFF);
        buffer[i * 2 + 1] = (uint8_t)(word >> 8);
    }

    // Wait for BSY to clear
    while (inb(0x1F7) & 0x80);
}

void ata_write_sector(uint32_t lba, const uint8_t* data) {
    // Wait for drive to be ready
    while (inb(0x1F7) & 0x80); // BSY

    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F)); // Select master + high LBA bits
    outb(0x1F2, 1); // Sector count (1 sector)
    outb(0x1F3, (uint8_t)lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x30); // WRITE SECTORS command

    // Wait for DRQ
    while (!(inb(0x1F7) & 0x08)); // DRQ

    // Write 512 bytes (256 words)
    for (int i = 0; i < 256; i++) {
        uint16_t word = ((uint16_t)data[i * 2 + 1] << 8) | data[i * 2];
        outw(0x1F0, word);
    }

    // Wait for BSY to clear
    while (inb(0x1F7) & 0x80);
}

