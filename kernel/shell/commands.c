#include "commands.h"
#include "../include/terminal.h"


#include "../include/gdt.h"

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
        "mov %[msg], %%ebx\n"
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