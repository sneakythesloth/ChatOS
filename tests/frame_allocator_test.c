#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "frame_allocator.h"

int main(void) {
    struct limine_memmap_entry region_a = {
        .base = 0x1000,
        .length = 4 * CHATOS_PAGE_SIZE,
        .type = LIMINE_MEMMAP_USABLE
    };
    struct limine_memmap_entry region_b = {
        .base = 0x10000,
        .length = 2 * CHATOS_PAGE_SIZE,
        .type = LIMINE_MEMMAP_USABLE
    };
    struct limine_memmap_entry reserved = {
        .base = 0x20000,
        .length = 4 * CHATOS_PAGE_SIZE,
        .type = LIMINE_MEMMAP_RESERVED
    };
    struct limine_memmap_entry *entries[] = {
        &region_a, &region_b, &reserved
    };
    struct limine_memmap_response map = {
        .revision = 0,
        .entry_count = 3,
        .entries = entries
    };

    assert(frame_allocator_init(&map) == 1);
    assert(frame_free_count() == 6);

    const uint64_t expected[] = {
        0x1000, 0x2000, 0x3000, 0x4000, 0x10000, 0x11000
    };

    for (uint64_t i = 0; i < 6; ++i) {
        assert(frame_alloc() == expected[i]);
    }

    assert(frame_alloc() == 0);
    assert(frame_free_count() == 0);

    assert(frame_free(0x1001) == 0);
    assert(frame_free(0) == 0);
    assert(frame_free(CHATOS_MAX_PHYSICAL_MEMORY) == 0);

    assert(frame_free(0x2000) == 1);
    assert(frame_free_count() == 1);
    assert(frame_free(0x2000) == 0); /* Double free is rejected. */
    assert(frame_alloc() == 0x2000);
    assert(frame_free(0x2000) == 1);

    puts("frame allocator tests: PASS");
    return 0;
}
