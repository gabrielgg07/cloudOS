#include "../../include/idt.h"
#include "../../include/terminal.h"
#include "../../include/port.h"
#include "../../include/pic.h"
#include "../../include/keyboard.h"
#include "../../syscall/syscall.h"
#include "../../timer/pit.h"

extern void isr_stub_0();
extern void isr_stub_1();
extern void isr_stub_2();
extern void isr_stub_3();
extern void isr_stub_4();
extern void isr_stub_5();
extern void isr_stub_6();
extern void isr_stub_7();
extern void isr_stub_8();
extern void isr_stub_9();
extern void isr_stub_10();
extern void isr_stub_11();
extern void isr_stub_12();
extern void isr_stub_13();
extern void isr_stub_14S();
extern void isr_stub_15();
extern void isr_stub_16();
extern void isr_stub_17();  
extern void isr_stub_18();
extern void isr_stub_19();
extern void isr_stub_20();
extern void isr_stub_21();
extern void isr_stub_22();
extern void isr_stub_23();
extern void isr_stub_24();
extern void isr_stub_25();  
extern void isr_stub_26();
extern void isr_stub_27();
extern void isr_stub_28();
extern void isr_stub_29();
extern void isr_stub_30();
extern void isr_stub_31();

extern void irq_stub_0();
extern void irq_stub_1();
extern void irq_stub_2();
extern void irq_stub_3();
extern void irq_stub_4();
extern void irq_stub_5();
extern void irq_stub_6();
extern void irq_stub_7();
extern void irq_stub_8();
extern void irq_stub_9();
extern void irq_stub_10();
extern void irq_stub_11();
extern void irq_stub_12();
extern void irq_stub_13();
extern void irq_stub_14();
extern void irq_stub_15();

extern void pit_handler_stub();

extern void isr80_stub();


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

extern void isr_handler();


static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

static void set_idt_gate(int n, uint32_t handler,  uint16_t selector, uint8_t flags) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = selector;
    idt[n].zero = 0;
    idt[n].type_attr = flags;
    idt[n].offset_high = (handler >> 16);
}

extern void idt_load(uint32_t); // defined in assembly


void init_keyboard() {
    // Enable the keyboard by writing to port 0x64
    // Command: 0xAE enables the keyboard
    outb(0x64, 0xAE);

    // Clear any pending output in the keyboard buffer
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }

}

void isr80_handler() {
    uint32_t id, arg1, arg2;

    asm volatile (
        "mov %%eax, %0\n"
        "mov %%ebx, %1\n"
        "mov %%ecx, %2\n"
        : "=r"(id), "=r"(arg1), "=r"(arg2)
        :
        : "memory"
    );

    syscall_dispatcher(id, arg1, arg2);
}


void idt_init() {
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;

    pic_remap();
    init_keyboard();
    // Set up the IDT for all ISRs  
    set_idt_gate(0,  (uint32_t)isr_stub_0, 0x08, 0x8E);
    set_idt_gate(1,  (uint32_t)isr_stub_1, 0x08, 0x8E);
    set_idt_gate(2,  (uint32_t)isr_stub_2, 0x08, 0x8E);
    set_idt_gate(3,  (uint32_t)isr_stub_3, 0x08, 0x8E);
    set_idt_gate(4,  (uint32_t)isr_stub_4, 0x08, 0x8E);
    set_idt_gate(5,  (uint32_t)isr_stub_5, 0x08, 0x8E);
    set_idt_gate(6,  (uint32_t)isr_stub_6, 0x08, 0x8E);
    set_idt_gate(7,  (uint32_t)isr_stub_7, 0x08, 0x8E);
    set_idt_gate(8,  (uint32_t)isr_stub_8, 0x08, 0x8E);
    set_idt_gate(9,  (uint32_t)isr_stub_9, 0x08, 0x8E);
    set_idt_gate(10, (uint32_t)isr_stub_10, 0x08, 0x8E);
    set_idt_gate(11, (uint32_t)isr_stub_11, 0x08, 0x8E);
    set_idt_gate(12, (uint32_t)isr_stub_12, 0x08, 0x8E);
    set_idt_gate(13, (uint32_t)isr_stub_13, 0x08, 0x8E);
    set_idt_gate(14, (uint32_t)isr_stub_14S, 0x08, 0x8E);
    set_idt_gate(15, (uint32_t)isr_stub_15, 0x08, 0x8E);
    set_idt_gate(16, (uint32_t)isr_stub_16, 0x08, 0x8E);
    set_idt_gate(17, (uint32_t)isr_stub_17, 0x08, 0x8E);
    set_idt_gate(18, (uint32_t)isr_stub_18, 0x08, 0x8E);
    set_idt_gate(19, (uint32_t)isr_stub_19, 0x08, 0x8E);
    set_idt_gate(20, (uint32_t)isr_stub_20, 0x08, 0x8E);
    set_idt_gate(21, (uint32_t)isr_stub_21, 0x08, 0x8E);
    set_idt_gate(22, (uint32_t)isr_stub_22, 0x08, 0x8E);
    set_idt_gate(23, (uint32_t)isr_stub_23, 0x08, 0x8E);
    set_idt_gate(24, (uint32_t)isr_stub_24, 0x08, 0x8E);
    set_idt_gate(25, (uint32_t)isr_stub_25, 0x08, 0x8E);
    set_idt_gate(26, (uint32_t)isr_stub_26, 0x08, 0x8E);
    set_idt_gate(27, (uint32_t)isr_stub_27, 0x08, 0x8E);
    set_idt_gate(28, (uint32_t)isr_stub_28, 0x08, 0x8E);
    set_idt_gate(29, (uint32_t)isr_stub_29, 0x08, 0x8E);
    set_idt_gate(30, (uint32_t)isr_stub_30, 0x08, 0x8E);
    set_idt_gate(31, (uint32_t)isr_stub_31, 0x08, 0x8E);

    set_idt_gate(32, (uint32_t)pit_handler_stub, 0x08, 0x8E);
    set_idt_gate(33, (uint32_t)irq_stub_1, 0x08, 0x8E);
    set_idt_gate(34, (uint32_t)irq_stub_2, 0x08, 0x8E);
    set_idt_gate(35, (uint32_t)irq_stub_3, 0x08, 0x8E);
    set_idt_gate(36, (uint32_t)irq_stub_4, 0x08, 0x8E);
    set_idt_gate(37, (uint32_t)irq_stub_5, 0x08, 0x8E);
    set_idt_gate(38, (uint32_t)irq_stub_6, 0x08, 0x8E);
    set_idt_gate(39, (uint32_t)irq_stub_7, 0x08, 0x8E);
    set_idt_gate(40, (uint32_t)irq_stub_8, 0x08, 0x8E);
    set_idt_gate(41, (uint32_t)irq_stub_9, 0x08, 0x8E);
    set_idt_gate(42, (uint32_t)irq_stub_10, 0x08, 0x8E);
    set_idt_gate(43, (uint32_t)irq_stub_11, 0x08, 0x8E);
    set_idt_gate(44, (uint32_t)irq_stub_12, 0x08, 0x8E);
    set_idt_gate(45, (uint32_t)irq_stub_13, 0x08, 0x8E);
    set_idt_gate(46, (uint32_t)irq_stub_14, 0x08, 0x8E);
    set_idt_gate(47, (uint32_t)irq_stub_15, 0x08, 0x8E);


    //default syscall handler
    set_idt_gate(0x80, (uint32_t)isr80_stub, 0x08, 0xEE);  // syscall handler
    outb(0x21, 0xFD); // Master PIC: Unmask IRQ1 (bit 1) and IRQ2 (bit 2, needed for Slave PIC)
    outb(0xA1, 0xEF); // Slave PIC: Unmask IRQ12 (bit 4)
    outb(0x21, 0xFC);  // 11111110 — enable only IRQ0 (for testing)
    // Load the IDT into the CPU
    
    idt_load((uint32_t)&idtp);
    asm volatile("sti");
    terminal_print("IDT loaded.\n");
}