// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"
#include "./include/keyboard.h"
#include "./include/lib/string.h"
#include "./include/shell.h"



void kernel_main() {
    terminal_initialize();
    arch_init();

    terminal_print("Welcome to cloudOS!\n");
    int i = 0;
    while(i < 1000000000) i++; // Simple delay
    terminal_clear();
    run_shell();
    // This point should never be reached
}



