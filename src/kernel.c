#include <stdint.h>

#include "serial.h"
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"

__attribute__((noreturn, section(".text.entry")))
void _start(void) {
    cpu_disable_interrupts();
    serial_init();
    interrupts_initialize();
    serial_write("M2: IDT installed; CPU exceptions have fatal diagnostic handlers.\r\n");

#ifdef CHATOS_EXCEPTION_SELF_TEST
    serial_write("M2 test: triggering INT3 to verify exception dispatch.\\r\\n");
    __asm__ volatile ("int3");
#endif

    serial_write("\r\n");
    serial_write("ChatOS kernel starting...\r\n");
    serial_write("Architecture : x86_64\r\n");
    serial_write("Boot protocol: Limine (base revision 3)\r\n");
    serial_write("Language     : freestanding C11\r\n");
    serial_write("\r\n");

    serial_write("M1: validating boot information and memory...\r\n");
    memory_initialize();
    serial_write("Physical frames: online.\r\n");
    serial_write("HHDM: online.\r\n");
    serial_write("Virtual memory: current page tables discovered.\r\n");
    serial_write("Kernel bootstrap complete. CPU halted with interrupts disabled.\r\n");

    for (;;) {
        cpu_halt();
    }
}
