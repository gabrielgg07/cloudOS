#include "../../include/idt.h"
#include "../../include/terminal.h"


#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void isr_handler() {
    terminal_print("Interrupt!\n");
}

extern void isr_stub_0(); // You'll define this in assembly

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

static void set_idt_gate(int n, uint32_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08;  // kernel code segment
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E; // interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

extern void idt_load(uint32_t); // defined in assembly

void idt_init() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++)
        set_idt_gate(i, (uint32_t)isr_stub_0); // temp: use same stub

    idt_load((uint32_t)&idtp);

    terminal_print("IDT loaded.\n");
}