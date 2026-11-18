#include <stdint.h>
#include "limine.h"

__attribute__((used, section(".limine_requests_start"), aligned(8)))
static volatile uint64_t limine_requests_start[4] = {
    0xf6b8f4b39de7d1aeULL,
    0xfab91a6940fcb9cfULL,
    0x785c6ed015d3e316ULL,
    0x181e920a7852b9d9ULL
};

/* Request protocol base revision 3. */
__attribute__((used, section(".limine_requests"), aligned(8)))
volatile uint64_t limine_base_revision[3] = {
    0xf9562b2d5c95a6c8ULL,
    0x6a7b384944536bdcULL,
    3
};

__attribute__((used, section(".limine_requests"), aligned(8)))
volatile struct limine_memmap_request limine_memmap_request = {
    .id = {
        0xc7b1dd30df4c8b88ULL,
        0x0a82e883a194f07bULL,
        0x67cf3d9d378a806fULL,
        0xe304acdfc50c3c62ULL
    },
    .revision = 0,
    .response = 0
};

__attribute__((used, section(".limine_requests_end"), aligned(8)))
static volatile uint64_t limine_requests_end[2] = {
    0xadc0e0531bb10d03ULL,
    0x9572709f31764c62ULL
};
