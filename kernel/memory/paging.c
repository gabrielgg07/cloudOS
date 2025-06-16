#include <stdint.h>
#include "../include/port.h"
#include "./paging.h" // For paging

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2
#define PAGE_SIZE    4096

static uint32_t* page_directory = (uint32_t*)0x1000;  // Just an example spot in free memory
static uint32_t* first_page_table = (uint32_t*)0x2000;

void paging_init() {
    // Zero them
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;  // Supervisor, R/W, not present (until set)
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITE;
    }

    // Map 0x00000000–0x003FFFFF using this page table
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITE;

    // Map 0xB8000 manually (just in case)
    first_page_table[0xB8000 / PAGE_SIZE] = 0xB8000 | PAGE_PRESENT | PAGE_WRITE;

    // Load page directory
    asm volatile("mov %0, %%cr3" :: "r"(page_directory));

    // Enable paging
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set PG bit
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}
