#include "vga.h"

char* video = (char*) VGA_MEMORY;

void vga_put_char_at(char c, int col, int row, char attr){
    int index = (row * VGA_WIDTH + col) * 2;
    video[index] = c;
    video[index + 1] = attr;
}
void vga_clear_screen(){
    for (int i = 0; i < 2000; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x07;  // White on black
    }
}