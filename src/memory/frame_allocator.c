#include <stdint.h>
#include "frame_allocator.h"

#define BITMAP_WORD_BITS 64ULL
#define BITMAP_WORD_COUNT (CHATOS_MAX_PHYSICAL_FRAMES / BITMAP_WORD_BITS)

static uint64_t frame_bitmap[BITMAP_WORD_COUNT];
static uint64_t free_count;
static uint64_t allocation_hint = 1;

static void mark_used(uint64_t frame) {
    frame_bitmap[frame / BITMAP_WORD_BITS] |=
        1ULL << (frame % BITMAP_WORD_BITS);
}

static void mark_free(uint64_t frame) {
    frame_bitmap[frame / BITMAP_WORD_BITS] &=
        ~(1ULL << (frame % BITMAP_WORD_BITS));
}

static int is_used(uint64_t frame) {
    return (frame_bitmap[frame / BITMAP_WORD_BITS] &
            (1ULL << (frame % BITMAP_WORD_BITS))) != 0;
}

int frame_allocator_init(const struct limine_memmap_response *memory_map) {
    if (memory_map == 0 || memory_map->entries == 0 ||
        memory_map->entry_count == 0 || memory_map->entry_count > 4096) {
        return 0;
    }

    for (uint64_t i = 0; i < BITMAP_WORD_COUNT; ++i) {
        frame_bitmap[i] = UINT64_MAX;
    }

    free_count = 0;
    allocation_hint = 1;

    for (uint64_t i = 0; i < memory_map->entry_count; ++i) {
        const struct limine_memmap_entry *entry = memory_map->entries[i];
        if (entry == 0 || entry->length == 0 ||
            entry->base > UINT64_MAX - entry->length) {
            return 0;
        }

        if (entry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        uint64_t start = entry->base;
        uint64_t end = entry->base + entry->length;

        if (start > UINT64_MAX - (CHATOS_PAGE_SIZE - 1)) {
            continue;
        }
        start = (start + CHATOS_PAGE_SIZE - 1) & ~(CHATOS_PAGE_SIZE - 1);
        end &= ~(CHATOS_PAGE_SIZE - 1);

        if (end <= start) {
            continue;
        }

        uint64_t first_frame = start / CHATOS_PAGE_SIZE;
        uint64_t end_frame = end / CHATOS_PAGE_SIZE;

        if (first_frame >= CHATOS_MAX_PHYSICAL_FRAMES) {
            continue;
        }
        if (end_frame > CHATOS_MAX_PHYSICAL_FRAMES) {
            end_frame = CHATOS_MAX_PHYSICAL_FRAMES;
        }

        for (uint64_t frame = first_frame; frame < end_frame; ++frame) {
            if (is_used(frame)) {
                mark_free(frame);
                ++free_count;
            }
        }
    }

    /* Physical address zero is never returned as an allocation. */
    if (!is_used(0)) {
        mark_used(0);
        --free_count;
    }

    return free_count != 0;
}

uint64_t frame_alloc(void) {
    if (free_count == 0) {
        return 0;
    }

    for (uint64_t pass = 0; pass < 2; ++pass) {
        uint64_t begin = pass == 0 ? allocation_hint : 1;
        uint64_t limit = pass == 0 ? CHATOS_MAX_PHYSICAL_FRAMES : allocation_hint;

        for (uint64_t frame = begin; frame < limit; ++frame) {
            if (!is_used(frame)) {
                mark_used(frame);
                --free_count;
                allocation_hint = frame + 1;
                if (allocation_hint >= CHATOS_MAX_PHYSICAL_FRAMES) {
                    allocation_hint = 1;
                }
                return frame * CHATOS_PAGE_SIZE;
            }
        }
    }

    return 0;
}

int frame_free(uint64_t physical_address) {
    if ((physical_address & (CHATOS_PAGE_SIZE - 1)) != 0) {
        return 0;
    }

    uint64_t frame = physical_address / CHATOS_PAGE_SIZE;
    if (frame == 0 || frame >= CHATOS_MAX_PHYSICAL_FRAMES || !is_used(frame)) {
        return 0;
    }

    mark_free(frame);
    ++free_count;
    if (frame < allocation_hint) {
        allocation_hint = frame;
    }
    return 1;
}

uint64_t frame_free_count(void) {
    return free_count;
}

uint64_t frame_capacity(void) {
    return CHATOS_MAX_PHYSICAL_FRAMES;
}
