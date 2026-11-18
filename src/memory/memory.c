#include <stdint.h>
#include "limine.h"
#include "memory.h"
#include "frame_allocator.h"
#include "serial.h"
#include "panic.h"

static uint64_t map_entries;
static uint64_t usable_bytes;

int memory_initialize(void) {
    if (limine_base_revision[2] != 0) {
        kernel_panic("Limine base revision 3 is not supported by the bootloader.");
    }

    struct limine_memmap_response *response = limine_memmap_request.response;
    if (response == 0 || response->entries == 0 ||
        response->entry_count == 0 || response->entry_count > 4096) {
        kernel_panic("Limine did not provide a valid memory map response.");
    }

    map_entries = response->entry_count;
    usable_bytes = 0;

    serial_write("Memory map entries: ");
    serial_write_u64(map_entries);
    serial_write("\r\n");

    for (uint64_t i = 0; i < response->entry_count; ++i) {
        const struct limine_memmap_entry *entry = response->entries[i];
        if (entry == 0 || entry->length == 0 ||
            entry->base > UINT64_MAX - entry->length) {
            kernel_panic("Invalid memory map entry supplied by Limine.");
        }

        if (entry->type == LIMINE_MEMMAP_USABLE) {
            if (usable_bytes <= UINT64_MAX - entry->length) {
                usable_bytes += entry->length;
            } else {
                kernel_panic("Usable memory total overflow.");
            }
        }
    }

    if (!frame_allocator_init(response)) {
        kernel_panic("Physical frame allocator initialization failed.");
    }

    serial_write("Usable memory reported: ");
    serial_write_u64(usable_bytes / (1024ULL * 1024ULL));
    serial_write(" MiB\r\n");
    serial_write("Free 4 KiB frames: ");
    serial_write_u64(frame_free_count());
    serial_write("\r\n");

    return 1;
}

uint64_t memory_map_entry_count(void) {
    return map_entries;
}

uint64_t memory_usable_bytes(void) {
    return usable_bytes;
}
