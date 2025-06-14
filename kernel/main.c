// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"
#include "./include/keyboard.h"

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
    while (1)
    {
    char c = keyboard_buffer_dequeue();
    if (c != 0) {
        terminal_print("CHAR: ");
        terminal_print_hex((uint8_t)c); // You need a hex printing function here
        terminal_print("\n");
    }

    }
}



