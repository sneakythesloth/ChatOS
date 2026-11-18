#ifndef CHATOS_HHDM_H
#define CHATOS_HHDM_H

#include <stdint.h>

int hhdm_initialize(void);
uint64_t hhdm_offset(void);
void *phys_to_virt(uint64_t physical_address);
uint64_t virt_to_phys(const void *virtual_address);

#endif
