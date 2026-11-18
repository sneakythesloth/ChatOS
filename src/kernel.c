#include <stdint.h>

#include "serial.h"
#include "cpu.h"

#define LIMINE_REQUESTS_START \
    { 0xf6b8f4b39de7d1aeULL, 0xfab91a6940fcb9cfULL, \
      0x785c6ed015d3e316ULL, 0x181e920a7852b9d9ULL }
#define LIMINE_BASE_REVISION \
    { 0xf9562b2d5c95a6c8ULL, 3, 0 }
#define LIMINE_REQUESTS_END \
    { 0xadc0e0531bb10d03ULL, 0x9572709f31764c62ULL }

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start[4] = LIMINE_REQUESTS_START;

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[3] = LIMINE_BASE_REVISION;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end[2] = LIMINE_REQUESTS_END;

__attribute__((noreturn, section(".text.entry")))
void _start(void) {
    cpu_disable_interrupts();
    serial_init();

    serial_write("\r\n");
    serial_write("ChatOS kernel starting...\r\n");
    serial_write("Architecture : x86_64\r\n");
    serial_write("Boot protocol: Limine\r\n");
    serial_write("Language     : freestanding C11\r\n");
    serial_write("\r\n");
    serial_write("Milestone 0: kernel entry reached.\r\n");
    serial_write("Next: boot responses, memory map, and physical page allocator.\r\n");

    for (;;) {
        cpu_halt();
    }
}
