#pragma once

#include <stdint.h>

void keyboard_interrupt_handler();


void keyboard_buffer_enqueue(char character);

char keyboard_buffer_dequeue();