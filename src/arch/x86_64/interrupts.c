#include <stdint.h>

#include "interrupts.h"
#include "serial.h"
#include "panic.h"

#define IDT_ENTRY_COUNT 256U
#define IDT_INTERRUPT_GATE 0x8EU

struct idt_gate {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct exception_frame {
    uint64_t vector;
    uint64_t error_code;
    uint64_t instruction_pointer;
    uint64_t code_segment;
    uint64_t flags;
};

static struct idt_gate idt[IDT_ENTRY_COUNT] __attribute__((aligned(16)));

extern void (*exception_stub_table[32])(void);
extern void isr255(void);

static const char *const exception_names[32] = {
    "Division Error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun (reserved)",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

static void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t code_segment) {
    struct idt_gate *gate = &idt[vector];
    gate->offset_low = (uint16_t)(handler & 0xFFFFU);
    gate->selector = code_segment;
    gate->ist = 0;
    gate->attributes = IDT_INTERRUPT_GATE;
    gate->offset_middle = (uint16_t)((handler >> 16) & 0xFFFFU);
    gate->offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFFU);
    gate->reserved = 0;
}

void interrupts_initialize(void) {
    uint16_t code_segment;
    __asm__ volatile ("mov %%cs, %0" : "=r"(code_segment));

    /*
     * Leave a valid fatal fallback in every vector slot. CPU exceptions get
     * their own stubs so hardware-pushed error codes are decoded correctly.
     */
    for (uint16_t vector = 0; vector < IDT_ENTRY_COUNT; ++vector) {
        idt_set_gate((uint8_t)vector, (uint64_t)isr255, code_segment);
    }

    for (uint8_t vector = 0; vector < 32; ++vector) {
        idt_set_gate(vector, (uint64_t)exception_stub_table[vector], code_segment);
    }

    const struct idtr descriptor = {
        .limit = (uint16_t)(sizeof(idt) - 1U),
        .base = (uint64_t)&idt[0]
    };
    __asm__ volatile ("lidt %0" : : "m"(descriptor) : "memory");

    serial_write("IDT: 256 interrupt gates installed; CPU exception stubs online.\r\n");
}

__attribute__((noreturn))
void interrupt_exception_dispatch(const struct exception_frame *frame) {
    if (frame == 0) {
        kernel_panic("Exception dispatcher received a null frame.");
    }

    serial_write("\r\n!!! CHATOS CPU EXCEPTION !!!\r\n");
    serial_write("Vector: ");
    serial_write_u64(frame->vector);
    serial_write("\r\nReason: ");
    if (frame->vector < 32) {
        serial_write(exception_names[frame->vector]);
    } else {
        serial_write("Unhandled interrupt");
    }
    serial_write("\r\nError code: ");
    serial_write_u64(frame->error_code);
    serial_write("\r\nInstruction pointer: ");
    serial_write_u64(frame->instruction_pointer);
    serial_write("\r\n");

    kernel_panic("CPU exception is fatal; system halted.");
}
