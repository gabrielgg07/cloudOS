#pragma once

#include <stdint.h>

void init_syscalls(void);
void syscall_dispatcher(uint32_t id, uint32_t arg1, uint32_t arg2);

// Define syscall IDs
#define SYS_PRINT 1
