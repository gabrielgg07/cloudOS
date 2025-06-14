#include "../../include/terminal.h"
#include "../../include/port.h"


static const char* scancode_to_string[128] = {
    [0x00] = "?",
    [0x01] = "ESC",
    [0x02] = "1",
    [0x03] = "2",
    [0x04] = "3",
    [0x05] = "4",
    [0x06] = "5",
    [0x07] = "6",
    [0x08] = "7",
    [0x09] = "8",
    [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",
    [0x0D] = "=",
    [0x0E] = "\b",
    [0x0F] = "\t",
    [0x10] = "Q",
    [0x11] = "W",
    [0x12] = "E",
    [0x13] = "R",
    [0x14] = "T",
    [0x15] = "Y",
    [0x16] = "U",
    [0x17] = "I",
    [0x18] = "O",
    [0x19] = "P",
    [0x1A] = "[",
    [0x1B] = "]",
    [0x1C] = "\n",
    [0x1D] = "Ctrl",
    // Here’s the special one you asked for:
    [0x1E] = "A",    // Scancode 0x1E => "A"
    [0x1F] = "S",
    [0x20] = "D",
    [0x21] = "F",
    [0x22] = "G",
    [0x23] = "H",
    [0x24] = "J",
    [0x25] = "K",
    [0x26] = "L",
    [0x27] = ";",
    [0x28] = "'",
    [0x29] = "`",
    [0x2A] = "LeftShift",
    [0x2B] = "\\",
    [0x2C] = "Z",
    [0x2D] = "X",
    [0x2E] = "C",
    [0x2F] = "V",
    [0x30] = "B",
    [0x31] = "N",
    [0x32] = "M",
    [0x33] = ",",
    [0x34] = ".",
    [0x35] = "/",
    [0x36] = "RightShift",
    [0x37] = "*",
    [0x38] = "Alt",
    [0x39] = " ",
    [0x4B] = "<",
    [0x4D] = ">",
    [0x48] = "^",
    [0x50] = "Down"
    // ...
    // Fill in the rest as needed, up to 0x7F
};
void irq_handler(int int_num, int err_code) {
    uint8_t scancode = inb(0x60);

    // Send End of Interrupt (EOI) to PIC
    outb(0x20, 0x20);

    // Handle scancode (convert to character or store raw scancode)
    if (!(scancode & 0x80) && int_num == 0x21) {  // Key down event (ignore key up)
        const char *character = scancode_to_string[scancode];
        if (character) {
            //keyboard_buffer_enqueue(character);  // Store character in a buffer
            terminal_print(character);
        }
    }
}
