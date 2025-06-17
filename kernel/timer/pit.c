#include "pit.h"
#include "../include/port.h"  // for outb()
#include "../include/terminal.h"  // optional, for debug
#include "../task/task.h"  // for task_switch()

void pit_irq_handler() {
    schedule();  // Preempt current task and switch to the next one
    //terminal_print("PIT IRQ Handler called.\n");
    outb(0x20, 0x20);  // Send EOI to PIC
}

void pit_init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;

    // Command port for PIT
    outb(0x43, 0x36);                  // Channel 0, low/high, mode 3, binary

    // Send low byte and high byte of divisor to channel 0
    outb(0x40, divisor & 0xFF);        // Low byte
    outb(0x40, (divisor >> 8) & 0xFF); // High byte

    // Optional debug
    terminal_print("PIT initialized.\n");
}
// This function initializes the Programmable Interval Timer (PIT) to generate
// interrupts at a specified frequency. The divisor is calculated based on the
// desired frequency, and the PIT is configured to operate in mode 3 (square wave
// generator) with a binary count. The low and high bytes of the divisor are sent
// to the PIT's channel 0 data port. The function also prints a debug message to
// the terminal to confirm initialization.