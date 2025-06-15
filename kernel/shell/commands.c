#include "commands.h"
#include "../include/terminal.h"


#include "../include/gdt.h"

void enter_user_mode() {
    uint32_t user_stack = 0x800000;  // 8 MB, make sure it's unused
    asm volatile (
        "cli\n"                          // Disable interrupts

        // Set user data segments (0x23 = user DS | RPL 3)
        "mov $0x23, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"

        // Push user-mode stack
        "pushl $0x23\n"                 // SS = user data segment
        "pushl %[stack]\n"             // ESP = user stack

        "pushf\n"                       // Push EFLAGS

        "pushl $0x1B\n"                 // CS = user code segment
        "pushl $1f\n"                   // EIP = label 1

        "iret\n"                        // Far return into user mode

         // === USER MODE STARTS HERE ===
        "1:\n"
        "nop\n"
        "jmp .\n"                       // Loop in user mode
        :
        : [stack] "r" (user_stack)
        : "memory", "eax"
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