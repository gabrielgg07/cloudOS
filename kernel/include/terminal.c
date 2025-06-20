#include "./terminal.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include "../drivers/vga.h"
// Or however you get your cursor position
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
// terminal.c
int terminal_row = 0;
int terminal_column = 0;
char terminal_color = 0;


void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = VGA_COLOR_WHITE | VGA_COLOR_BLACK << 4;
    vga_clear_screen();
}


void terminal_put_char(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        vga_put_char_at(c, terminal_column, terminal_row, 0x07);
        terminal_column++;
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    if (terminal_row >= VGA_HEIGHT) {
        // Optional: scroll screen or clear
        terminal_row = 0;
    }
}

void terminal_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        terminal_put_char(str[i]);
}

void terminal_print_int(int num) {
    char buf[32];
    int i = 0;
    if (num == 0) {
        terminal_put_char('0');
        return;
    }

    if (num < 0) {
        terminal_put_char('-');
        num = -num;
    }

    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--)
        terminal_put_char(buf[j]);
}

void terminal_print_hex(uint32_t num) {
    const char* hex_digits = "0123456789ABCDEF";
    for (int i = 7; i >= 0; i--) {
        terminal_put_char(hex_digits[(num >> (i * 4)) & 0xF]);
    }
}

void terminal_clear() {
    terminal_row = 0;
    terminal_column = 0;
    vga_clear_screen();
}


void terminal_set_cursor(int row, int col) {
    terminal_column = col;
    terminal_row = row;
}

void terminal_backspace() {
    if (terminal_column == 0 && terminal_row == 0) return; // Top-left, can't backspace

    if (terminal_column == 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    } else {
        terminal_column--;
    }

    // Clear character at current location
    vga_put_char_at(' ', terminal_column, terminal_row, 0x07);
    // Move cursor to updated position
    terminal_set_cursor(terminal_row, terminal_column);
}

void terminal_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (size_t i = 0; fmt[i]; i++) {
        if (fmt[i] == '%' && fmt[i + 1]) {
            i++;
            switch (fmt[i]) {
                case 'd':
                case 'i':
                    terminal_print_int(va_arg(args, uint32_t));
                    break;
                case 'x':
                    terminal_print_hex(va_arg(args, uint32_t));
                    break;
                case 's':
                    terminal_print(va_arg(args, const char*));
                    break;
                case 'c':
                    {
                        char c = (char)va_arg(args, int);
                        terminal_put_char(c);
                    }
                    break;
                case '%':
                    terminal_put_char('%');
                    break;
                default:
                    terminal_put_char('%');
                    terminal_put_char(fmt[i]);
                    break;
            }
        } else {
            terminal_put_char(fmt[i]);
        }
    }

    va_end(args);
}


