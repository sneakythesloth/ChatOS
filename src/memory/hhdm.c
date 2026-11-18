#include <stdint.h>
#include "limine.h"
#include "hhdm.h"
#include "panic.h"
#include "serial.h"

static uint64_t direct_map_offset;

static int is_canonical(uint64_t address) {
    uint64_t upper = address >> 48;
    return ((address >> 47) & 1ULL) ? upper == 0xffffULL : upper == 0;
}

int hhdm_initialize(void) {
    struct limine_hhdm_response *response = limine_hhdm_request.response;
    if (response == 0 || !is_canonical(response->offset) ||
        (response->offset & 0xfffULL) != 0) {
        kernel_panic("Limine did not provide a valid HHDM response.");
    }

    direct_map_offset = response->offset;
    serial_write("HHDM offset: 0x");
    /* Print hexadecimal in a later console extension; retain decimal for now. */
    serial_write_u64(direct_map_offset);
    serial_write("\r\n");
    return 1;
}

uint64_t hhdm_offset(void) {
    return direct_map_offset;
}

void *phys_to_virt(uint64_t physical_address) {
    if (direct_map_offset == 0 ||
        physical_address > UINT64_MAX - direct_map_offset) {
        return 0;
    }
    uint64_t address = direct_map_offset + physical_address;
    if (!is_canonical(address)) {
        return 0;
    }
    return (void *)(uintptr_t)address;
}

uint64_t virt_to_phys(const void *virtual_address) {
    uint64_t address = (uint64_t)(uintptr_t)virtual_address;
    if (direct_map_offset == 0 || address < direct_map_offset) {
        return UINT64_MAX;
    }
    return address - direct_map_offset;
}
