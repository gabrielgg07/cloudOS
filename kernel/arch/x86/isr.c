#include "../../include/terminal.h"

const char* exception_messages[] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment check",
    "Machine check",
    "SIMD Floating Point",
    "Virtualization",
    "Security exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(int int_num, int err_code) {
    terminal_print("Exception: ");
    terminal_print(exception_messages[int_num]);
    terminal_print(" (INT ");
    terminal_print_int(int_num);
    terminal_print(")\n");
}
