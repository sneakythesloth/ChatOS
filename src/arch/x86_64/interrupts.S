/*
 * x86_64 exception entry stubs.
 *
 * The common stack layout passed to C is:
 *   vector, error_code, RIP, CS, RFLAGS
 * Error-code exceptions have their error code pushed by the CPU.
 */

.text
.code64

.extern interrupt_exception_dispatch

.macro ISR_NO_ERROR number
    .global isr\number
    .type isr\number, @function
isr\number:
    pushq $0
    pushq $\number
    jmp isr_common
.endm

.macro ISR_ERROR number
    .global isr\number
    .type isr\number, @function
isr\number:
    pushq $\number
    jmp isr_common
.endm

ISR_NO_ERROR 0
ISR_NO_ERROR 1
ISR_NO_ERROR 2
ISR_NO_ERROR 3
ISR_NO_ERROR 4
ISR_NO_ERROR 5
ISR_NO_ERROR 6
ISR_NO_ERROR 7
ISR_ERROR     8
ISR_NO_ERROR 9
ISR_ERROR    10
ISR_ERROR    11
ISR_ERROR    12
ISR_ERROR    13
ISR_ERROR    14
ISR_NO_ERROR 15
ISR_NO_ERROR 16
ISR_ERROR    17
ISR_NO_ERROR 18
ISR_NO_ERROR 19
ISR_NO_ERROR 20
ISR_ERROR    21
ISR_NO_ERROR 22
ISR_NO_ERROR 23
ISR_NO_ERROR 24
ISR_NO_ERROR 25
ISR_NO_ERROR 26
ISR_NO_ERROR 27
ISR_NO_ERROR 28
ISR_ERROR    29
ISR_ERROR    30
ISR_NO_ERROR 31

.global isr255
.type isr255, @function
isr255:
    pushq $0
    pushq $255
    jmp isr_common

.type isr_common, @function
isr_common:
    cld
    movq %rsp, %rdi
    andq $-16, %rsp
    call interrupt_exception_dispatch

1:
    cli
    hlt
    jmp 1b

.section .rodata
.align 8
.global exception_stub_table
.type exception_stub_table, @object
exception_stub_table:
    .quad isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
    .quad isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
    .quad isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
    .quad isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
.size exception_stub_table, .-exception_stub_table

.section .note.GNU-stack,"",@progbits

/*
 * IRQ0 (PIT channel 0) interrupt entry. Unlike fatal exception stubs,
 * this path preserves every general-purpose register and returns with iretq.
 */
.text
.code64

.extern interrupt_irq_dispatch

.global irq0_stub
.type irq0_stub, @function
irq0_stub:
    pushq $0
    pushq $32
    jmp irq_common

.type irq_common, @function
irq_common:
    cld
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    /* Preserve the complete saved-register frame across the C call. */
    movq %rsp, %r12
    leaq 120(%rsp), %rdi
    andq $-16, %rsp
    call interrupt_irq_dispatch
    movq %r12, %rsp

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax

    addq $16, %rsp
    iretq

.section .note.GNU-stack,"",@progbits
