#ifndef CHATOS_X86_64_CPU_H
#define CHATOS_X86_64_CPU_H

static inline void cpu_disable_interrupts(void) {
    __asm__ volatile ("cli" ::: "memory");
}

static inline void cpu_halt(void) {
    __asm__ volatile ("hlt");
}

#endif
