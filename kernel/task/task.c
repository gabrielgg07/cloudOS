
#include "task.h"

#include "../include/terminal.h"

#define MAX_TASKS 10
#define STACK_SIZE 4096


task_t tasks[MAX_TASKS];
int task_count = 0;

task_t* current_task = 0;

uint8_t task_stacks[MAX_TASKS][STACK_SIZE];

// External assembly function (defined later)
extern void switch_to_task(task_t* next);


void create_task(void (*func)()) {
    if (task_count >= MAX_TASKS) {
        terminal_print("Max task count reached.\n");
        return;
    }

    task_t* t = &tasks[task_count];

    // Setup stack pointer to end of stack
    uint32_t stack_top = (uint32_t)&task_stacks[task_count][STACK_SIZE];

    // Fake stack: push return address (EIP)
    stack_top -= sizeof(uint32_t);
    *(uint32_t*)stack_top = (uint32_t)func;

    // Setup task
    t->id = task_count;
    t->esp = stack_top;
    t->ebp = stack_top;
    t->next = 0;

    // Add to circular list
    if (task_count > 0) {
        tasks[task_count - 1].next = t;
    }

    task_count++;
    if (task_count == 1) {
        current_task = t;
    }

    terminal_printf("Created task %d at 0x%x\n", t->id, func);
}

void tasking_init() {
    terminal_print("Tasking initialized.\n");
}

void switch_task() {
    if (!current_task || !current_task->next) return;
    current_task = current_task->next;
    switch_to_task(current_task);
}
