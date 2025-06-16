#include "../../include/arch.h"
#include "../../include/pic.h"
#include "../../include/terminal.h"
#include "../../include/port.h"
#include "../../network/nic.h"


void arch_init() {
    gdt_init();
    idt_init();
    nic_init();
}
