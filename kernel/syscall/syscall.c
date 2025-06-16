#include "../include/terminal.h"
#include "syscall.h"

static void sys_print(const char* user_string) {
    // No paging yet, so we trust the pointer
    terminal_print(user_string);
    terminal_print("\n");
}

void syscall_dispatcher(uint32_t id, uint32_t arg1, uint32_t arg2) {
    switch (id) {
        case SYS_PRINT:
            sys_print((const char*)arg1);
            break;
        default:
            terminal_print("Unknown syscall ID\n");
            break;
    }
}

void init_syscalls(void) {
    terminal_print("Syscalls initialized.\n");
}
