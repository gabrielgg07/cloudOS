// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"

void kernel_main() {
    terminal_initialize();
    arch_init();

    terminal_print("Hello, World!\n");

    terminal_print("About to test IRQ\n");
    //asm volatile ("int $33");
    terminal_print("Returned from int 33\n");
    int c = 0;
    int b = 0;
    int a = 0;
    while (1){
    };
}



