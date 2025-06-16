#include "commands.h"
#include "../include/terminal.h"
#include "../network/nic.h"
#include "../include/gdt.h"
#include "../network/nic.h"
#include "../include/disk.h"
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

void cmd_diskwrite(const char* input) {
    uint8_t buf[512] = {0};
    const char* msg = "Hello from CloudOS!";
    for (int i = 0; msg[i]; i++) {
        buf[i] = (uint8_t)msg[i];
    }

    ata_write_sector(1, buf);  // Write to LBA 1
    terminal_print("Data written to disk.\n");
}

void cmd_diskread(const char* input) {
    uint8_t buf[513] = {0}; // Extra byte for null terminator
    ata_read_sector(1, buf);
    terminal_print("Read from disk:\n  ");
    terminal_print((char*)buf);
    terminal_print("\n");
}
