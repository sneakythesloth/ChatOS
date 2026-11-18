/*
 * Reload the CPU's descriptor tables and segment selectors.
 * SysV AMD64: rdi points to a packed 10-byte GDTR operand.
 */
.text
.code64

.global gdt_load
.type gdt_load, @function
gdt_load:
    lgdt (%rdi)

    /* Reload CS using the new ring-0 code descriptor. */
    pushq $0x08
    leaq .Lreload_segments(%rip), %rax
    pushq %rax
    lretq

.Lreload_segments:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss

    xorw %ax, %ax
    movw %ax, %fs
    movw %ax, %gs
    ret

.section .note.GNU-stack,"",@progbits
