#pragma once

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS   0x18 | 0x3
#define USER_DS   0x20 | 0x3

#include <stdint.h>

void gdt_init(void);