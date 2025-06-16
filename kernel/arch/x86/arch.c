#include "../../include/arch.h"
#include "../../include/pic.h"
#include "../../include/terminal.h"
#include "../../include/port.h"
#include "../../network/nic.h"
#include "../../include/fat.h"
#include "../../task/task.h"
#include "../../timer/pit.h"


void arch_init() {
    gdt_init();
    idt_init();
    nic_init();
    paging_init();
    fat16_init();
    tasking_init();
    pit_init(100);  // 100Hz = 10ms slice
}
