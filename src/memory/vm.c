#include <stdint.h>
#include "vm.h"
#include "hhdm.h"
#include "frame_allocator.h"
#include "panic.h"
#include "serial.h"

#define PAGE_ADDRESS_MASK 0x000ffffffffff000ULL
#define PAGE_PRESENT VM_PAGE_PRESENT
#define PAGE_SIZE_1G 0x080ULL
#define PAGE_SIZE_2M 0x080ULL
#define VM_ALLOWED_FLAGS (VM_PAGE_WRITE | VM_PAGE_USER)

static uint64_t kernel_page_table_root;

static int is_canonical(uint64_t address) {
    uint64_t upper = address >> 48;
    return ((address >> 47) & 1ULL) ? upper == 0xffffULL : upper == 0;
}

static uint64_t *table_from_physical(uint64_t physical_address) {
    return (uint64_t *)phys_to_virt(physical_address & PAGE_ADDRESS_MASK);
}

static void zero_page(uint64_t physical_address) {
    uint64_t *page = table_from_physical(physical_address);
    if (page == 0) {
        kernel_panic("HHDM cannot access newly allocated page-table frame.");
    }
    for (uint64_t i = 0; i < 512; ++i) {
        page[i] = 0;
    }
}

static uint64_t *next_table(uint64_t *table, uint64_t index, uint64_t flags) {
    uint64_t entry = table[index];

    if ((entry & PAGE_PRESENT) == 0) {
        uint64_t frame = frame_alloc();
        if (frame == 0) {
            return 0;
        }
        zero_page(frame);
        entry = frame | PAGE_PRESENT | VM_PAGE_WRITE;
        if ((flags & VM_PAGE_USER) != 0) {
            entry |= VM_PAGE_USER;
        }
        table[index] = entry;
    } else {
        if ((flags & VM_PAGE_WRITE) != 0) {
            entry |= VM_PAGE_WRITE;
        }
        if ((flags & VM_PAGE_USER) != 0) {
            entry |= VM_PAGE_USER;
        }
        table[index] = entry;
    }

    if ((entry & PAGE_SIZE_2M) != 0) {
        return 0;
    }

    return table_from_physical(entry);
}

int vm_initialize(void) {
    uint64_t cr3;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));
    kernel_page_table_root = cr3 & PAGE_ADDRESS_MASK;

    if (kernel_page_table_root == 0 ||
        table_from_physical(kernel_page_table_root) == 0) {
        kernel_panic("Unable to access current PML4 page table.");
    }

    serial_write("Virtual memory: current page tables discovered.\r\n");
    return 1;
}

int vm_map_page(uint64_t virtual_address, uint64_t physical_address, uint64_t flags) {
    if (kernel_page_table_root == 0 ||
        !is_canonical(virtual_address) ||
        (virtual_address & 0xfffULL) != 0 ||
        (physical_address & 0xfffULL) != 0 ||
        (physical_address & ~PAGE_ADDRESS_MASK) != 0 ||
        (flags & ~VM_ALLOWED_FLAGS) != 0) {
        return 0;
    }

    uint64_t *pml4 = table_from_physical(kernel_page_table_root);
    if (pml4 == 0) {
        return 0;
    }

    uint64_t pml4_index = (virtual_address >> 39) & 0x1ffULL;
    uint64_t pdpt_index = (virtual_address >> 30) & 0x1ffULL;
    uint64_t pd_index = (virtual_address >> 21) & 0x1ffULL;
    uint64_t pt_index = (virtual_address >> 12) & 0x1ffULL;

    uint64_t *pdpt = next_table(pml4, pml4_index, flags);
    if (pdpt == 0) {
        return 0;
    }
    uint64_t *pd = next_table(pdpt, pdpt_index, flags);
    if (pd == 0) {
        return 0;
    }
    uint64_t *pt = next_table(pd, pd_index, flags);
    if (pt == 0 || (pt[pt_index] & PAGE_PRESENT) != 0) {
        return 0;
    }

    pt[pt_index] = physical_address | PAGE_PRESENT | (flags & VM_ALLOWED_FLAGS);
    __asm__ volatile ("invlpg (%0)" : : "r"(virtual_address) : "memory");
    return 1;
}

uint64_t vm_unmap_page(uint64_t virtual_address) {
    if (kernel_page_table_root == 0 ||
        !is_canonical(virtual_address) ||
        (virtual_address & 0xfffULL) != 0) {
        return 0;
    }

    uint64_t *pml4 = table_from_physical(kernel_page_table_root);
    if (pml4 == 0) return 0;
    uint64_t pml4e = pml4[(virtual_address >> 39) & 0x1ffULL];
    if ((pml4e & PAGE_PRESENT) == 0) return 0;

    uint64_t *pdpt = table_from_physical(pml4e);
    if (pdpt == 0) return 0;
    uint64_t pdpt_entry = pdpt[(virtual_address >> 30) & 0x1ffULL];
    if ((pdpt_entry & PAGE_PRESENT) == 0 || (pdpt_entry & PAGE_SIZE_1G) != 0) return 0;

    uint64_t *pd = table_from_physical(pdpt_entry);
    if (pd == 0) return 0;
    uint64_t pd_entry = pd[(virtual_address >> 21) & 0x1ffULL];
    if ((pd_entry & PAGE_PRESENT) == 0 || (pd_entry & PAGE_SIZE_2M) != 0) return 0;

    uint64_t *pt = table_from_physical(pd_entry);
    if (pt == 0) return 0;
    uint64_t *pte = &pt[(virtual_address >> 12) & 0x1ffULL];
    if ((*pte & PAGE_PRESENT) == 0) return 0;

    uint64_t physical_address = *pte & PAGE_ADDRESS_MASK;
    *pte = 0;
    __asm__ volatile ("invlpg (%0)" : : "r"(virtual_address) : "memory");
    return physical_address;
}

uint64_t vm_translate(uint64_t virtual_address) {
    if (kernel_page_table_root == 0 || !is_canonical(virtual_address)) {
        return 0;
    }

    uint64_t *pml4 = table_from_physical(kernel_page_table_root);
    uint64_t pml4e = pml4[(virtual_address >> 39) & 0x1ffULL];
    if ((pml4e & PAGE_PRESENT) == 0) return 0;

    uint64_t *pdpt = table_from_physical(pml4e);
    if (pdpt == 0) return 0;
    uint64_t pdpte = pdpt[(virtual_address >> 30) & 0x1ffULL];
    if ((pdpte & PAGE_PRESENT) == 0) return 0;
    if ((pdpte & PAGE_SIZE_1G) != 0) {
        return (pdpte & 0x000fffffc0000000ULL) |
               (virtual_address & 0x3fffffffULL);
    }

    uint64_t *pd = table_from_physical(pdpte);
    if (pd == 0) return 0;
    uint64_t pde = pd[(virtual_address >> 21) & 0x1ffULL];
    if ((pde & PAGE_PRESENT) == 0) return 0;
    if ((pde & PAGE_SIZE_2M) != 0) {
        return (pde & 0x000fffffffe00000ULL) |
               (virtual_address & 0x1fffffULL);
    }

    uint64_t *pt = table_from_physical(pde);
    if (pt == 0) return 0;
    uint64_t pte = pt[(virtual_address >> 12) & 0x1ffULL];
    if ((pte & PAGE_PRESENT) == 0) return 0;
    return (pte & PAGE_ADDRESS_MASK) | (virtual_address & 0xfffULL);
}
