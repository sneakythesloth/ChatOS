#include <stdint.h>

#include "pic.h"

#define PIC1_COMMAND 0x20U
#define PIC1_DATA    0x21U
#define PIC2_COMMAND 0xA0U
#define PIC2_DATA    0xA1U
#define PIC_EOI      0x20U
#define PIC1_OFFSET  0x20U
#define PIC2_OFFSET  0x28U

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void io_wait(void) {
    outb(0x80U, 0U);
}

void pic_initialize(void) {
    const uint8_t master_mask = inb(PIC1_DATA);
    const uint8_t slave_mask = inb(PIC2_DATA);

    /* ICW1: begin initialization, expect ICW4. */
    outb(PIC1_COMMAND, 0x11U);
    io_wait();
    outb(PIC2_COMMAND, 0x11U);
    io_wait();

    /* ICW2: relocate IRQ vectors outside CPU exception vectors. */
    outb(PIC1_DATA, PIC1_OFFSET);
    io_wait();
    outb(PIC2_DATA, PIC2_OFFSET);
    io_wait();

    /* ICW3: master IRQ2 is the slave cascade input; slave identity is 2. */
    outb(PIC1_DATA, 0x04U);
    io_wait();
    outb(PIC2_DATA, 0x02U);
    io_wait();

    /* ICW4: 8086 mode. */
    outb(PIC1_DATA, 0x01U);
    io_wait();
    outb(PIC2_DATA, 0x01U);
    io_wait();

    /* Keep every line masked until its handler and device are initialized. */
    outb(PIC1_DATA, 0xFFU);
    outb(PIC2_DATA, 0xFFU);

    (void)master_mask;
    (void)slave_mask;
}

void pic_enable_irq(uint8_t irq) {
    if (irq >= 16U) {
        return;
    }

    const uint16_t data_port = (irq < 8U) ? PIC1_DATA : PIC2_DATA;
    const uint8_t line = (irq < 8U) ? irq : (uint8_t)(irq - 8U);
    uint8_t mask = inb(data_port);
    mask = (uint8_t)(mask & (uint8_t)~(1U << line));
    outb(data_port, mask);

    if (irq >= 8U) {
        uint8_t master_mask = inb(PIC1_DATA);
        master_mask = (uint8_t)(master_mask & (uint8_t)~(1U << 2U));
        outb(PIC1_DATA, master_mask);
    }
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8U) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
