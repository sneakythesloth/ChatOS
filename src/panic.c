#include "serial.h"
#include "cpu.h"
#include "panic.h"

__attribute__((noreturn))
void kernel_panic(const char *reason) {
    cpu_disable_interrupts();
    serial_write("\r\n!!! CHATOS KERNEL PANIC !!!\r\n");
    serial_write(reason != 0 ? reason : "Unknown panic");
    serial_write("\r\nSystem halted.\r\n");

    for (;;) {
        cpu_halt();
    }
}
