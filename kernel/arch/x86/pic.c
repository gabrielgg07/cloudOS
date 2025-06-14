#include <stdint.h>
#include "../../include/pic.h"
#include "../../include/port.h"  // for outb()

void pic_remap() {
    // Save the original interrupt masks
    uint8_t mask1 = inb(0x21);  // Master PIC mask
    uint8_t mask2 = inb(0xA1);  // Slave PIC mask

    // Start the initialization sequence (cascade mode)
    outb(0x20, 0x11);  // Start init for Master PIC
    outb(0xA0, 0x11);  // Start init for Slave PIC

    // Set vector offsets for remapping
    outb(0x21, 0x20);  // Remap Master PIC to offset1 (e.g., 0x20)
    outb(0xA1, 0x28);  // Remap Slave PIC to offset2 (e.g., 0x28)

    // Tell Master PIC about the Slave PIC at IRQ2 (cascade mode)
    outb(0x21, 0x04);

    // Tell Slave PIC its cascade identity
    outb(0xA1, 0x02);

    // Set both PICs to 8086/88 (x86) mode
    outb(0x21, 0x01);  // Master PIC
    outb(0xA1, 0x01);  // Slave PIC

    // Restore the saved masks (unmask only what you want to handle)
    outb(0x21, mask1);  // Restore Master PIC mask
    outb(0xA1, mask2);  // Restore Slave PIC mask

}
