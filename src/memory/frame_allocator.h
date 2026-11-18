#ifndef CHATOS_FRAME_ALLOCATOR_H
#define CHATOS_FRAME_ALLOCATOR_H

#include <stdint.h>
#include "limine.h"

#define CHATOS_PAGE_SIZE 4096ULL
#define CHATOS_MAX_PHYSICAL_MEMORY (64ULL * 1024ULL * 1024ULL * 1024ULL)
#define CHATOS_MAX_PHYSICAL_FRAMES (CHATOS_MAX_PHYSICAL_MEMORY / CHATOS_PAGE_SIZE)

int frame_allocator_init(const struct limine_memmap_response *memory_map);
uint64_t frame_alloc(void);
int frame_free(uint64_t physical_address);
uint64_t frame_free_count(void);
uint64_t frame_capacity(void);

#endif
