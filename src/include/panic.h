#ifndef CHATOS_PANIC_H
#define CHATOS_PANIC_H

__attribute__((noreturn))
void kernel_panic(const char *reason);

#endif
