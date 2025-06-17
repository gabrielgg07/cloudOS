#pragma once
#include <stdint.h>

void paging_init();
//void map_page(uint32_t virt_addr, uint32_t phys_addr);


void page_fault_handler(uint32_t error_code, uint32_t eip);