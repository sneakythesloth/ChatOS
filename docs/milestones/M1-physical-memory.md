# Milestone 1: Limine Boot Information and Physical Frames

**Status:** Implementation scaffold committed; build and runtime verification pending.  
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

## Verification

The GitHub Actions workflow is configured to compile the freestanding kernel on pushes and pull requests. The workflow run triggered by these commits must be inspected before claiming the compile passed. No QEMU boot or runtime allocator test is claimed yet.

## Acceptance criteria

- [ ] GitHub Actions build succeeds on the final M1 commit.
- [ ] QEMU boots through Limine and reaches memory_initialize().
- [ ] Serial log reports a nonzero memory map and plausible usable-memory total.
- [ ] Runtime allocation and free smoke test passes.
- [ ] Invalid/double free and out-of-range address tests pass in a host-side unit harness.

## Next

Add host-side allocator tests, then validate the full boot path. Only after this milestone is verified should work begin on page tables and kernel virtual-memory management.
