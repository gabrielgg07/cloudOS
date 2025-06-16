#include "../../include/gdt.h"

extern void tss_flush(void);
// GDT entry structure
struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));


// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


// GDT with 3 entries: null, code, data
static struct gdt_entry gdt[6];
static struct gdt_ptr gdtp;


//tss stuff
struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t unused[23]; // Fill to 104 bytes (sizeof TSS)
} __attribute__((packed));

struct tss_entry tss;

extern void gdt_flush(uint32_t);

// Helper to set one GDT entry
static void gdt_set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].base_low    = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   = limit & 0xFFFF;
    gdt[i].granularity = (limit >> 16) & 0x0F;

    gdt[i].granularity |= gran & 0xF0;
    gdt[i].access = access;
}

void tss_set(uint32_t kernel_stack) {
    uint32_t base = (uint32_t)&tss;
    uint32_t limit = base + sizeof(tss);

    gdt_set_entry(5, base, limit, 0x89, 0x40); // type 0x89 = 32-bit TSS (available)
    tss.esp0 = kernel_stack;
    tss.ss0  = 0x10; // Kernel data segment selector
}

void gdt_init(void) {
    gdtp.limit = sizeof(gdt) - 1;
    gdtp.base = (uint32_t)&gdt;

    // Null descriptor
    gdt_set_entry(0, 0, 0, 0, 0);

    // Kernel code segment: base=0, limit=4GB, access=0x9A, gran=0xCF
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Kernel data segment: base=0, limit=4GB, access=0x92, gran=0xCF
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

        // User code segment: base=0, limit=4GB, access=0xFA, gran=0xCF
    gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

    // User data segment: base=0, limit=4GB, access=0xF2, gran=0xCF
    gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
    gdt_set_entry(5, (uint32_t)&tss, sizeof(tss), 0x89, 0x00);  // 0x89 = TSS (32-bit)
    gdt_flush((uint32_t)&gdtp);

    tss_set(0x90000); // some free kernel-mode stack
    tss_flush();      // install TSS into TR register
    

}


