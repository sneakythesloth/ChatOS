#include <stdint.h>

#include "serial.h"
#include "cpu.h"
#include "memory.h"
#include "interrupts.h"
#include "pic.h"
#include "timer.h"

__attribute__((noreturn, section(".text.entry")))
void _start(void) {
    cpu_disable_interrupts();
    serial_init();
    interrupts_initialize();
    serial_write("M2: IDT installed; CPU exceptions have fatal diagnostic handlers.\r\n");

#ifdef CHATOS_EXCEPTION_SELF_TEST
    serial_write("M2 test: triggering INT3 to verify exception dispatch.\r\n");
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

    pic_initialize();
    pit_initialize(100U);
    pic_enable_irq(0U);
    serial_write("PIC: IRQ vectors remapped; PIT channel 0 programmed at 100 Hz.\r\n");

#ifdef CHATOS_TIMER_SELF_TEST
    serial_write("M2 timer test: waiting for five PIT interrupts.\r\n");
    __asm__ volatile ("sti" : : : "memory");
    while (timer_ticks_read() < 5U) {
        __asm__ volatile ("hlt" : : : "memory");
    }
    __asm__ volatile ("cli" : : : "memory");
    serial_write("PIT timer: 5 ticks received.\r\n");
#endif
    serial_write("Physical frames: online.\r\n");
    serial_write("HHDM: online.\r\n");
    serial_write("Virtual memory: current page tables discovered.\r\n");
    serial_write("Kernel bootstrap complete. CPU halted with interrupts disabled.\r\n");

    for (;;) {
        cpu_halt();
    }
}
