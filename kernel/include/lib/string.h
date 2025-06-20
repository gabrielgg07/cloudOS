#pragma once

#include <stdint.h>
#include <stddef.h>

int strcmp(const char* s1, const char* s2);
size_t strlen(const char* str);
void strcpy(char* dst, const char* src);
int strncmp(const char* s1, const char* s2, size_t n); 
