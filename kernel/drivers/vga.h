#pragma once

#define VGA_MEMORY (char*)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern char* video;



void vga_put_char_at(char c, int col, int row, char attr);
void vga_clear_screen();
