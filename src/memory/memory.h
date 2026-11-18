#ifndef CHATOS_MEMORY_H
#define CHATOS_MEMORY_H

#include <stdint.h>

int memory_initialize(void);
uint64_t memory_map_entry_count(void);
uint64_t memory_usable_bytes(void);

#endif
