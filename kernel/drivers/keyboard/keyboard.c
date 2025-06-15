#include "../../include/keyboard.h"
#include "../../include/port.h"

const char scancode_to_ascii[128] = {
    0,  '\x1B', '1', '2', '3', '4', '5', '6', '7', '8', // 0x00 - 0x09
    '9', '0', '-', '=', '\b', '\t',                   // 0x0A - 0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  // 0x10 - 0x19
    '[', ']', '\n', 0,                                // Enter, Control (0x1C–0x1D)
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  // 0x1E - 0x26
    '\'', '`', 0, '\\',                               // LShift (0x2A), Backslash (0x2B)
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',  // 0x2C - 0x35
    0, '*', 0, ' ',                                   // RShift (0x36), Space (0x39)
    0, 0, 0, 0, 0, 0, 0,                              // F1–F10 (0x3A–0x43)
    0, 0, 0, 0, 0, 0,                                 // Unused / extended
};
const char scancode_to_ascii_shift[128] = {
    0,  '\x1B', '!', '@', '#', '$', '%', '^', '&', '*', // 0x00 - 0x09
    '(', ')', '_', '+', '\b', '\t',                     // 0x0A - 0x0F
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',    // 0x10 - 0x19
    '{', '}', '\n', 0,                                  // Enter, Control (0x1C–0x1D)
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',    // 0x1E - 0x26
    '"', '~', 0, '|',                                   // LShift (0x2A), Backslash (0x2B)
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',    // 0x2C - 0x35
    0, '*', 0, ' ',                                     // RShift (0x36), Space (0x39)
    0, 0, 0, 0, 0, 0, 0,                                // F1–F10 (0x3A–0x43)
    0, 0, 0, 0, 0, 0,                                   // Unused / extended
};


#define KEYBOARD_BUFFER_SIZE 256

int shift_pressed = 0;
char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_buffer_head = 0;
int keyboard_buffer_tail = 0;

void keyboard_interrupt_handler() {
    uint8_t scancode = inb(0x60);

    // Shift key press/release detection
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        outb(0x20, 0x20);
        return;
    } else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        outb(0x20, 0x20);
        return;
    }

    if (scancode & 0x80) {
        // Key release (ignore non-shift releases)
        outb(0x20, 0x20);
        return;
    }

    char c = shift_pressed
           ? scancode_to_ascii_shift[scancode]
           : scancode_to_ascii[scancode];

    if (c) {
        keyboard_buffer_enqueue(c);
    }

    outb(0x20, 0x20);  // Send EOI
}



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
