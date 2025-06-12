// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"

void kernel_main() {
    arch_init();
    terminal_initialize();
    terminal_print("Hello, World!");
}



