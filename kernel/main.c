// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"
#include "./include/keyboard.h"
#include "./include/lib/string.h"
#include "./include/shell.h"
#include "./task/task.h"

void helloE() {
    while (1) {
        for (volatile int i = 0; i < 100000000; i++){
            for (volatile int a = 0; a < 1; a++){
           
            }
        }
        terminal_print("Hello from task E!\n");
    }
}


void kernel_main() {
    extern uint32_t kernel_start;
    extern uint32_t kernel_end;
    terminal_printf("Kernel loaded at 0x%x–0x%x\n", &kernel_start, &kernel_end);
    terminal_initialize();

    create_task(run_shell);
    create_task(helloE);
    arch_init();

    terminal_print("Welcome to cloudOS!\n");
    int i = 0;
    while(i < 1000000000) i++; // Simple delay
    terminal_clear();
    //helloE();
    terminal_print("before int");
    asm volatile("int $14"); // Enable interrupts
    terminal_print("after int");
    while (1) {
        //un_shell();
    }
    // This point should never be reached
}



