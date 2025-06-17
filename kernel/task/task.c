
#include "task.h"

#include "../include/terminal.h"
#include "../memory/paging.h"

#define MAX_TASKS 10
#define STACK_SIZE 4096


task_t tasks[MAX_TASKS];
int task_count = 0;

task_t* current_task = 0;

uint8_t task_stacks[MAX_TASKS][STACK_SIZE];

// External assembly function (defined later)
extern void switch_to_task(task_t* next);


void create_task(void (*entry)()) {
    if (task_count >= MAX_TASKS) return;

    task_t* t = &tasks[task_count];
    uint32_t* stack = (uint32_t*)&task_stacks[task_count][STACK_SIZE];

    terminal_printf("Creating task %d at EIP=0x%x\n", task_count, (uint32_t)entry);
    for (uint32_t i = 0; i < STACK_SIZE; i += 0x1000) {
        //map_page((uint32_t)&task_stacks[task_count][i], (uint32_t)&task_stacks[task_count][i]);
    }

    // Build an interrupt-like stack
    *(--stack) = 0x202;                 // EFLAGS (IF = 1)
    *(--stack) = 0x08;                  // CS (kernel code segment)
    *(--stack) = (uint32_t)entry;       // EIP (start at this function)

    // Dummy pusha values (edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax)
    for (int i = 0; i < 8; i++) *(--stack) = 0;

    t->id = task_count;
    t->esp = (uint32_t)stack;
    t->ebp = 0;  // optional
    t->eip = (uint32_t)entry;  // optional
    t->next = current_task;  // make it circular

    if (task_count > 0) {
        tasks[task_count - 1].next = t;
    } else {
        current_task = t;
    }
    task_count++;
}

void tasking_init() {
    terminal_print("Tasking initialized.\n");
}

void schedule() {
    if (!current_task || !current_task->next) return;
    current_task = current_task->next;
    switch_to_task(current_task);
}
