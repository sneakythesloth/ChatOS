OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

KERNEL_VMA = 0xffffffff80000000;
KERNEL_PHYSICAL_BASE = 0x00200000;

PHDRS
{
    text   PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data   PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = KERNEL_VMA;
    _kernel_start = .;

    /*
     * Keep virtual addresses in the higher half, but give every PT_LOAD a
     * low physical load address.  Limine uses p_paddr when placing ELF
     * segments; leaving p_paddr equal to the high-half p_vaddr would ask the
     * bootloader to load the kernel at an impossible physical address.
     */
    .text : AT(ADDR(.text) - KERNEL_VMA + KERNEL_PHYSICAL_BASE)
    {
        KEEP(*(.text.entry))
        *(.text .text.*)
    } :text

    . = ALIGN(4K);
    .limine_requests : AT(ADDR(.limine_requests) - KERNEL_VMA + KERNEL_PHYSICAL_BASE)
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :rodata

    . = ALIGN(4K);
    .rodata : AT(ADDR(.rodata) - KERNEL_VMA + KERNEL_PHYSICAL_BASE)
    {
        *(.rodata .rodata.*)
    } :rodata

    . = ALIGN(4K);
    .data : AT(ADDR(.data) - KERNEL_VMA + KERNEL_PHYSICAL_BASE)
    {
        *(.data .data.*)
    } :data

    . = ALIGN(4K);
    .bss : AT(ADDR(.bss) - KERNEL_VMA + KERNEL_PHYSICAL_BASE)
    {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    _kernel_end = .;

    ASSERT(ADDR(.text) == KERNEL_VMA, "Kernel virtual base changed unexpectedly")
    ASSERT(LOADADDR(.text) == KERNEL_PHYSICAL_BASE, "Kernel physical load base changed unexpectedly")
    ASSERT((_kernel_end - KERNEL_VMA) <= 0x04000000, "Kernel image exceeds 64 MiB")

    /DISCARD/ :
    {
        *(.eh_frame*)
        *(.comment)
    }
}
