#pragma once
#include <stdint.h>

typedef struct task {
    uint32_t id;
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;
    struct task* next;
} task_t;

void tasking_init();
void create_task(void (*func)());
void switch_task();