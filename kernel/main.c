// kernel/main.c

#include "./include/terminal.h"
#include "./include/arch.h"
#include "./include/keyboard.h"
#include "./include/lib/string.h"
#include "./include/shell.h"
#include "./task/task.h"

void helloE(){
    int a = 0;
    int b = 0;
    while (1) {
        if (a == 1000000000) {
            if (b == 1000000000) {
                terminal_print("Hello from task E!\n");
                int b = 0;
            }
            b++;
            a = 0;
        }
        a++;
    }
}

void kernel_main() {
    terminal_initialize();
    arch_init();

    terminal_print("Welcome to cloudOS!\n");
    int i = 0;
    while(i < 1000000000) i++; // Simple delay
    create_task(run_shell);
    create_task(helloE);
    terminal_clear();
    run_shell();
    // This point should never be reached
}



