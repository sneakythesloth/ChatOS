#include <stdint.h>

#include "timer.h"
#include "panic.h"

#define PIT_INPUT_HZ 1193182U
#define PIT_CHANNEL0_DATA 0x40U
#define PIT_COMMAND 0x43U
#define PIT_CHANNEL0_RATE_GENERATOR 0x36U
#define PIT_MIN_FREQUENCY 19U
#define PIT_MAX_FREQUENCY 1000U

static volatile uint64_t timer_ticks;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void pit_initialize(uint32_t frequency_hz) {
    if (frequency_hz < PIT_MIN_FREQUENCY || frequency_hz > PIT_MAX_FREQUENCY) {
        kernel_panic("PIT frequency is outside the supported range.");
    }

    const uint32_t divisor = PIT_INPUT_HZ / frequency_hz;
    if (divisor == 0U || divisor > 0xFFFFU) {
        kernel_panic("PIT divisor is invalid.");
    }

    timer_ticks = 0;
    outb(PIT_COMMAND, PIT_CHANNEL0_RATE_GENERATOR);
    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFFU));
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFFU));
}

void timer_irq_handler(void) {
    ++timer_ticks;
}

uint64_t timer_ticks_read(void) {
    return timer_ticks;
}
