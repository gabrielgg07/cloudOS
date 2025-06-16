#pragma once

#include <stdint.h>

void pit_init(uint32_t frequency);
void pit_irq_handler();