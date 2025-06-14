#include "../../include/keyboard.h"

#define KEYBOARD_BUFFER_SIZE 256
char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_buffer_head = 0;
int keyboard_buffer_tail = 0;


void keyboard_buffer_enqueue(char character) {
    keyboard_buffer[keyboard_buffer_head] = character;
    keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

char keyboard_buffer_dequeue() {
    if (keyboard_buffer_head == keyboard_buffer_tail) {
        return 0;  // Buffer is empty
    }
    char c = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}
