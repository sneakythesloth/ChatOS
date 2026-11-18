#ifndef CHATOS_VM_H
#define CHATOS_VM_H

#include <stdint.h>

#define VM_PAGE_PRESENT 0x001ULL
#define VM_PAGE_WRITE   0x002ULL
/* Reserved; vm_map_page rejects user mappings until address spaces exist. */
#define VM_PAGE_USER    0x004ULL

int vm_initialize(void);
int vm_map_page(uint64_t virtual_address, uint64_t physical_address, uint64_t flags);
uint64_t vm_unmap_page(uint64_t virtual_address);
uint64_t vm_translate(uint64_t virtual_address);

#endif
