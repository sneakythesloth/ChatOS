#ifndef CHATOS_X86_64_TIMER_H
#define CHATOS_X86_64_TIMER_H

#include <stdint.h>

void pit_initialize(uint32_t frequency_hz);
void timer_irq_handler(void);
uint64_t timer_ticks_read(void);

#endif
