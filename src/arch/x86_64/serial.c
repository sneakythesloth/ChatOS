#include <stdint.h>
#include "serial.h"

#define COM1 0x3F8
#define COM1_DATA (COM1 + 0)
#define COM1_INTERRUPT_ENABLE (COM1 + 1)
#define COM1_FIFO_CONTROL (COM1 + 2)
#define COM1_LINE_CONTROL (COM1 + 3)
#define COM1_MODEM_CONTROL (COM1 + 4)
#define COM1_LINE_STATUS (COM1 + 5)

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void serial_init(void) {
    outb(COM1_INTERRUPT_ENABLE, 0x00);
    outb(COM1_LINE_CONTROL, 0x80);
    outb(COM1_DATA, 0x01);
    outb(COM1_INTERRUPT_ENABLE, 0x00);
    outb(COM1_LINE_CONTROL, 0x03);
    outb(COM1_FIFO_CONTROL, 0xC7);
    outb(COM1_MODEM_CONTROL, 0x0B);
}

void serial_write_char(char character) {
    while ((inb(COM1_LINE_STATUS) & 0x20) == 0) {
        __asm__ volatile ("pause");
    }
    outb(COM1_DATA, (uint8_t)character);
}

void serial_write(const char *text) {
    if (text == 0) {
        return;
    }

    while (*text != '\0') {
        if (*text == '\n') {
            serial_write_char('\r');
        }
        serial_write_char(*text++);
    }
}
