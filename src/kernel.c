#include <stdint.h>

#include "serial.h"
#include "cpu.h"
#include "memory.h"

__attribute__((noreturn, section(".text.entry")))
void _start(void) {
    cpu_disable_interrupts();
    serial_init();

    serial_write("\r\n");
    serial_write("ChatOS kernel starting...\r\n");
    serial_write("Architecture : x86_64\r\n");
    serial_write("Boot protocol: Limine (base revision 3)\r\n");
    serial_write("Language     : freestanding C11\r\n");
    serial_write("\r\n");

    serial_write("Milestone 1: validating boot information...\r\n");
    memory_initialize();
    serial_write("Physical frame allocator: online (64 GiB address window).\r\n");
    serial_write("Kernel bootstrap complete. CPU halted pending M2.\r\n");

    for (;;) {
        cpu_halt();
    }
}
