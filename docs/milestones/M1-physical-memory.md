# Milestone 1: Limine Boot Information and Physical Frames

**Status:** C implementation builds and host-side tests pass; QEMU/runtime verification pending.  
**Target:** x86_64, freestanding C11, Limine base revision 3.

## Scope delivered

- Local C declarations for Limine memory-map request, response, and entry structures.
- Correct three-word Limine base revision marker.
- Memory-map request ID and request delimiters.
- Validation of base revision support, response pointer, entry count, entry pointers, nonzero lengths, and address overflow.
- Usable physical-memory accounting.
- Static bitmap physical frame allocator covering the first 64 GiB of physical address space.
- 4 KiB frame allocation and free operations.
- Serial reporting of memory-map entry count, usable MiB, and currently free frame count.
- Host-side tests for allocation order, exhaustion, invalid frees, double frees, and reserved-region exclusion.

## Allocator contract and limitations

- Bitmap starts with every frame marked unavailable.
- Only pages wholly contained in Limine USABLE ranges are made allocatable.
- Physical frame zero is reserved.
- frame_alloc() returns a physical address; zero means allocation failure.
- frame_free() accepts only aligned, currently allocated, nonzero frames in the managed range.
- Allocator is single-CPU only. It has no lock and must not be used concurrently.
- The bitmap is statically sized for 64 GiB; RAM above that limit is ignored.
- Allocated pages are not zeroed and are not automatically mapped into the kernel virtual address space.
- Page tables, HHDM conversion, virtual memory, and heap allocation are not part of this milestone.

## Verification record

GitHub Actions run [44253625685](https://github.com/sneakythesloth/ChatOS/actions/runs/44253625685) completed successfully on commit 445218fa413088bfb094a44cc3e969b69bcb0279. Both the host-side physical frame allocator tests and the freestanding C kernel build passed.

QEMU boot, serial runtime memory-map output, and live allocator calls from the kernel remain unverified. No hardware boot claim is made.

## Acceptance criteria

- [x] GitHub Actions build succeeds on the M1 CI commit.
- [x] Host-side frame allocator tests pass.
- [ ] QEMU boots through Limine and reaches memory_initialize().
- [ ] Serial log reports a nonzero memory map and plausible usable-memory total.
- [ ] Runtime allocation and free smoke test passes using the actual boot memory map.
- [x] Invalid, double-free, misaligned, and out-of-range address tests pass in the host-side harness.

## Next

Validate the full QEMU boot path. Then add HHDM helpers, page-table management, and virtual-memory tests before building a kernel heap.
