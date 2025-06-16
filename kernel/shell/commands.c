#include "commands.h"
#include "../include/terminal.h"
#include "../network/nic.h"
#include "../include/gdt.h"
#include "../network/nic.h"
#include "../include/disk.h"
#include "../include/lib/util.h"

const char* get_args(const char* input) {
    // Skip the command name
    while (*input && *input != ' ') input++;  // Skip command
    while (*input == ' ') input++;            // Skip spaces
    return input;
}

void enter_user_mode() {
    const char* msg = "Hello from user mode!\n";
    uint32_t user_stack = 0x800000;

    asm volatile (
        "cli\n"
        "mov $0x23, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        "pushl $0x23\n"
        "pushl %[stack]\n"
        "pushf\n"
        "pushl $0x1B\n"
        "pushl $1f\n"
        "iret\n"

        "1:\n"
        "mov $1, %%eax\n"
        "mov %[msg], %%ebx\n"
        "int $0x80\n"

        "mov $1, %%eax\n"
        "int $0x80\n"

        "2:\n"
        "jmp 2b\n"
        :
        : [stack] "r"(user_stack), [msg] "r"(msg)
        : "memory", "eax", "ebx"
    );
}

void cmd_help(const char* input) {
    terminal_print(input);
    terminal_print("Available commands:\n");
    terminal_print("  help   - Show this help message\n");
    terminal_print("  clear  - Clear the screen\n");
    terminal_print("  echo   - Echo input text\n");
}

// Clear command
void cmd_clear(const char* input) {
    terminal_clear();
}

// Echo command
void cmd_echo(const char* input) {
    // input is like: "echo hello world"
    const char* msg = input + 5;  // Skip "echo " (5 chars)
    while (*msg == ' ') msg++;    // Trim leading space
    terminal_print(msg);
    terminal_put_char('\n');
}

void cmd_sysinfo(const char* input) {
    terminal_print("System Information:\n");
    // CPU Vendor string
    char vendor[13];
    uint32_t eax, ebx, ecx, edx;
    __asm__ volatile (
        "cpuid"
        : "=b"(ebx), "=d"(edx), "=c"(ecx)
        : "a"(0)
    );
    *(uint32_t*)&vendor[0] = ebx;
    *(uint32_t*)&vendor[4] = edx;
    *(uint32_t*)&vendor[8] = ecx;
    vendor[12] = 0;
    terminal_print("  CPU Vendor: ");
    terminal_print("  ");
    terminal_print(vendor);
    terminal_print("\n");
    char brand[49];
    uint32_t* brand_ptr = (uint32_t*)brand;

    for (uint32_t i = 0; i < 3; ++i) {
        __asm__ volatile (
            "cpuid"
            : "=a"(brand_ptr[i*4 + 0]), "=b"(brand_ptr[i*4 + 1]), "=c"(brand_ptr[i*4 + 2]), "=d"(brand_ptr[i*4 + 3])
            : "a"(0x80000002 + i)
        );
    }
    brand[48] = 0;

    terminal_print("  CPU Name: ");
    terminal_print(brand);
    terminal_print("\n");
    const char* bios_area = (const char*)0xF0000;
    for (uintptr_t addr = 0xF0000; addr < 0x100000; addr += 16) {
        if (*(uint32_t*)addr == 0x5F4D535F) { // "_SM_"
            terminal_print("  SMBIOS table found at 0x");
            // convert addr to hex string
            break;
        }
    }
    terminal_print("  Memory: 16GB\n");
    terminal_print("  OS: Custom Kernel\n");
}




void cmd_nicinfo(const char* input) {
    print_nic_info();
}

void cmd_diskinfo(const char* input) {
    ata_identify();
}

uint32_t parse_first_arg(const char* input, const char** out_rest) {
    // Skip the command name
    while (*input && *input != ' ') input++;
    while (*input == ' ') input++;

    // Parse number
    uint32_t val = strtol(input, (char**)out_rest, 10);

    // Skip whitespace before rest
    while (**out_rest == ' ') (*out_rest)++;

    return val;
}



void cmd_diskwrite(const char* input) {
    const char* msg = get_args(input);
    int sector = parse_first_arg(input, &msg); // Skip the command name
    if (!msg || !*msg) {
        terminal_print("Error: No message provided for disk write.\n");
        return;
    }
    uint8_t buf[512] = {0};
    for (int i = 0; msg[i]; i++) {
        buf[i] = (uint8_t)msg[i];
    }

    ata_write_sector(sector, buf);  // Write to specified LBA
    terminal_print("Data written to disk.\n");
}

void cmd_diskread(const char* input) {
    const char* msg = get_args(input);
    int sector = parse_first_arg(input, &msg); // Skip the command name
    uint8_t buf[513] = {0}; // Extra byte for null terminator
    ata_read_sector(sector, buf);
    terminal_print("Read from disk:\n  ");
    terminal_print((char*)buf);
    terminal_print("\n");
}

void cmd_ls(const char* input) {
    terminal_print("Root Directory:\n");
    fat16_list_root();
}

void cmd_cat(const char* input) {
    const char* filename = get_args(input);
    fat16_read_file(filename);
}

void cmd_touch(const char* input) {
    const char* args = get_args(input);

    char name[9] = {0};  // 8 + null
    char ext[4] = {0};   // 3 + null

    // Parse "FILENAME.EXT"
    int i = 0;
    while (*args && *args != '.' && i < 8) {
        name[i++] = *args++;
    }
    name[i] = '\0';

    if (*args == '.') {
        args++;  // skip the dot
        i = 0;
        while (*args && i < 3) {
            ext[i++] = *args++;
        }
        ext[i] = '\0';
    }

    // Pad name and ext with spaces for FAT format
    for (int j = i; j < 8; j++) name[j] = ' ';
    for (int j = i; j < 3; j++) ext[j] = ' ';

    fat16_create_file(name, ext, "Created from touch\n", 19);
}

void cmd_mkdir(const char* input) {
    const char* dirname = get_args(input);
    fat16_mkdir(dirname);
}

void cmd_cd(const char* input) {
    const char* dirname = get_args(input);
    fat16_cd(dirname);
}

//helper
int parse_echo_input(const char* input, char* out_text, char* out_filename) {
    int i = 0;
    while (input[i] && input[i] != '>') {
        out_text[i] = input[i];
        i++;
    }

    if (input[i] != '>') return 0;  // invalid format

    out_text[i] = '\0';

    i++; // skip '>'

    // Skip whitespace
    while (input[i] == ' ') i++;

    int j = 0;
    while (input[i] && j < 32) {
        out_filename[j++] = input[i++];
    }
    out_filename[j] = '\0';

    return 1;
}

void cmd_echoDirect(const char* input) {
    char text[256] = {0};
    char file[32] = {0};
    char name[9] = {0};
    char ext[4] = {0};

    if (!parse_echo_input(get_args(input), text, file)) {
        terminal_print("Usage: echo <text> > FILENAME.TXT\n");
        return;
    }

    // Extract name and extension
    int i = 0;
    while (file[i] && file[i] != '.' && i < 8) {
        name[i] = file[i];
        i++;
    }
    name[i] = '\0';

    if (file[i] == '.') {
        i++;
        int j = 0;
        while (file[i] && j < 3) {
            ext[j++] = file[i++];
        }
        ext[j] = '\0';
    }

    // Pad name/ext with spaces (FAT format)
    for (int k = strlen(name); k < 8; k++) name[k] = ' ';
    for (int k = strlen(ext); k < 3; k++) ext[k] = ' ';

    fat16_create_file(name, ext, text, strlen(text));
}