#pragma once
#include <stdint.h>
#include <stddef.h>

uint32_t strtol(const char* str, const char** out_rest, int base);
void* memset(void* dest, int value, size_t len);