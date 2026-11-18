#include <stdint.h>

#include "frame_allocator.h"
#include "hhdm.h"
#include "vm.h"

#define MEMORY_TEST_VIRTUAL_BASE 0xFFFFC00000000000ULL
#define MEMORY_TEST_PATTERN_A 0x434841544F534D31ULL
#define MEMORY_TEST_PATTERN_B 0x9CB7BEBAB0ACB2CEULL

/*
 * Exercise the live Limine HHDM and active kernel page tables. The test uses
 * one freshly allocated frame, maps it at a currently unused higher-half
 * virtual address, verifies aliasing and offset translation, then unmaps and
 * returns the data frame. Intermediate page-table frames intentionally stay
 * allocated because page-table pruning/reclamation is not implemented yet.
 */
int memory_runtime_self_test(void) {
    const uint64_t virtual_base = MEMORY_TEST_VIRTUAL_BASE;

    if (vm_translate(virtual_base) != 0) {
        return 0;
    }

    const uint64_t frame = frame_alloc();
    if (frame == 0) {
        return 0;
    }

    volatile uint64_t *direct = (volatile uint64_t *)phys_to_virt(frame);
    if (direct == 0 || virt_to_phys((const void *)direct) != frame) {
        (void)frame_free(frame);
        return 0;
    }

    if (!vm_map_page(virtual_base, frame, VM_PAGE_WRITE)) {
        (void)frame_free(frame);
        return 0;
    }

    volatile uint64_t *mapped = (volatile uint64_t *)(uintptr_t)virtual_base;
    mapped[0] = MEMORY_TEST_PATTERN_A;
    mapped[511] = MEMORY_TEST_PATTERN_B;

    const int alias_ok =
        direct[0] == MEMORY_TEST_PATTERN_A &&
        direct[511] == MEMORY_TEST_PATTERN_B;
    const int translation_ok =
        vm_translate(virtual_base) == frame &&
        vm_translate(virtual_base + 0xABCULL) == frame + 0xABCULL;

    const uint64_t unmapped_frame = vm_unmap_page(virtual_base);
    const int unmapped_ok =
        unmapped_frame == frame && vm_translate(virtual_base) == 0;

    const int freed_ok = frame_free(frame);
    return alias_ok && translation_ok && unmapped_ok && freed_ok;
}
