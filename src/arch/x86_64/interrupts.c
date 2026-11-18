#include <stdint.h>

#include "interrupts.h"
#include "serial.h"
#include "panic.h"
#include "pic.h"
#include "timer.h"

#define IDT_ENTRY_COUNT 256U
#define IDT_INTERRUPT_GATE 0x8EU
#define IDT_KERNEL_CODE_SELECTOR 0x08U
#define GDT_TSS_SELECTOR 0x18U
#define DOUBLE_FAULT_STACK_SIZE (16U * 1024U)

struct idt_gate {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t attributes;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct descriptor_pointer {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct tss64 {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed));

struct exception_frame {
    uint64_t vector;
    uint64_t error_code;
    uint64_t instruction_pointer;
    uint64_t code_segment;
    uint64_t flags;
};

static struct idt_gate idt[IDT_ENTRY_COUNT] __attribute__((aligned(16)));
static uint64_t gdt[5] __attribute__((aligned(16)));
static struct tss64 tss __attribute__((aligned(16)));
static uint8_t double_fault_stack[DOUBLE_FAULT_STACK_SIZE] __attribute__((aligned(16)));

extern void (*exception_stub_table[32])(void);
extern void isr255(void);
extern void irq0_stub(void);
extern void gdt_load(const struct descriptor_pointer *descriptor);

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

static void gdt_initialize(void) {
    const uint64_t tss_base = (uint64_t)&tss;
    const uint32_t tss_limit = (uint32_t)(sizeof(tss) - 1U);

    /* Ring-0 64-bit code and data descriptors. */
    gdt[0] = 0;
    gdt[1] = 0x00AF9A000000FFFFULL;
    gdt[2] = 0x00CF92000000FFFFULL;

    tss.ist1 = (uint64_t)&double_fault_stack[DOUBLE_FAULT_STACK_SIZE];
    tss.iomap_base = (uint16_t)sizeof(tss);

    /* 64-bit available TSS system descriptor occupies two GDT slots. */
    gdt[3] = ((uint64_t)tss_limit & 0xFFFFULL)
           | ((tss_base & 0xFFFFFFULL) << 16)
           | (0x89ULL << 40)
           | (((uint64_t)tss_limit & 0xF0000ULL) << 32)
           | (((tss_base >> 24) & 0xFFULL) << 56);
    gdt[4] = tss_base >> 32;

    const struct descriptor_pointer descriptor = {
        .limit = (uint16_t)(sizeof(gdt) - 1U),
        .base = (uint64_t)&gdt[0]
    };
    gdt_load(&descriptor);

    const uint16_t tss_selector = GDT_TSS_SELECTOR;
    __asm__ volatile ("ltr %0" : : "r"(tss_selector) : "memory");

    uint16_t active_tss;
    __asm__ volatile ("str %0" : "=r"(active_tss));
    if (active_tss != tss_selector) {
        kernel_panic("The x86_64 task-state segment did not load.");
    }

    serial_write("GDT/TSS: ring-0 segments loaded; IST1 reserved for double faults.\r\n");
}

static void idt_set_gate(uint8_t vector, uint64_t handler, uint8_t ist) {
    struct idt_gate *gate = &idt[vector];
    gate->offset_low = (uint16_t)(handler & 0xFFFFU);
    gate->selector = IDT_KERNEL_CODE_SELECTOR;
    gate->ist = (uint8_t)(ist & 0x07U);
    gate->attributes = IDT_INTERRUPT_GATE;
    gate->offset_middle = (uint16_t)((handler >> 16) & 0xFFFFU);
    gate->offset_high = (uint32_t)((handler >> 32) & 0xFFFFFFFFU);
    gate->reserved = 0;
}

void interrupts_initialize(void) {
    gdt_initialize();

    /*
     * Leave a valid fatal fallback in every vector slot. CPU exceptions get
     * their own stubs so hardware-pushed error codes are decoded correctly.
     * Double fault (#DF) uses IST1, independent of the current stack.
     */
    for (uint16_t vector = 0; vector < IDT_ENTRY_COUNT; ++vector) {
        idt_set_gate((uint8_t)vector, (uint64_t)isr255, 0);
    }

    for (uint8_t vector = 0; vector < 32; ++vector) {
        idt_set_gate(vector, (uint64_t)exception_stub_table[vector],
                     vector == 8 ? 1U : 0U);
    }

    /* Legacy PIC IRQ0 is remapped to vector 32. */
    idt_set_gate(32U, (uint64_t)irq0_stub, 0U);

    const struct descriptor_pointer descriptor = {
        .limit = (uint16_t)(sizeof(idt) - 1U),
        .base = (uint64_t)&idt[0]
    };
    __asm__ volatile ("lidt %0" : : "m"(descriptor) : "memory");

    serial_write("IDT: 256 gates installed; CPU exception stubs and IRQ0 handler online.\r\n");
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

    if (frame->vector == 14) {
        uint64_t fault_address;
        __asm__ volatile ("mov %%cr2, %0" : "=r"(fault_address));
        serial_write("Page-fault address: ");
        serial_write_u64(fault_address);
        serial_write("\r\n");
    }

    kernel_panic("CPU exception is fatal; system halted.");
}

/*
 * IRQ dispatch is deliberately separate from fatal exception dispatch.
 * At this stage only IRQ0 is unmasked; all other legacy PIC lines stay masked.
 */
void interrupt_irq_dispatch(const struct exception_frame *frame) {
    if (frame == 0 || frame->vector != 32U) {
        kernel_panic("Unexpected or malformed hardware IRQ frame.");
    }

    timer_irq_handler();
    pic_send_eoi(0U);
}
