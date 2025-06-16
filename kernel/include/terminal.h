#pragma once


#include <stdint.h>
#include <stddef.h>

#define VGA_COLOR_WHITE 0
#define VGA_COLOR_BLACK 0


extern int terminal_row;
extern int terminal_column;
extern char terminal_color;

void terminal_initialize();

void terminal_put_char(char c);

void terminal_print(const char* str);

void terminal_print_int(int num);

void terminal_print_hex(uint32_t num);

void terminal_clear();

void terminal_printf(const char* fmt, ...);





