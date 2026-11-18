# Milestone 0: C Kernel Bootstrap

**Status:** C scaffold committed; build and boot verification pending.  
**Target:** x86_64, freestanding C11, Limine, QEMU.

## Objective

Create the smallest useful independent ChatOS kernel foundation: a freestanding C binary with a defined entry point, basic diagnostics, and a reproducible path toward a bootable ISO.

## Delivered files

- Clang/LLD freestanding C build script.
- x86_64 kernel entry point in C.
- Limine request start/end markers and base revision declaration.
- x86_64 CPU primitives for disabling interrupts and halting.
- COM1 serial driver and panic routine.
- Linker script and Limine boot menu configuration.
- ISO build script and QEMU launch script, including optional OVMF firmware selection.
- GitHub Actions workflow for C kernel compilation.
- Architecture and milestone documentation.

## Current kernel behaviour

The entry point disables interrupts, initializes COM1, prints a startup report, and halts the CPU.

This is not a functioning general-purpose OS. Memory management, exception/interrupt handling, drivers, scheduling, storage, userspace, graphics, and networking are not implemented.

## Acceptance criteria

- [ ] Clang compiles all C sources for x86_64-unknown-none-elf.
- [ ] LLD links the freestanding kernel ELF without unresolved runtime symbols.
- [ ] ISO is generated with Limine BIOS and UEFI boot files.
- [ ] QEMU reaches the ChatOS kernel entry point.
- [ ] Serial output contains the kernel startup report.
- [ ] Optional OVMF boot path is verified.
- [ ] Build/boot logs are attached to the milestone or CI run.

## Verification record

The source and build configuration have been committed. No successful compile or QEMU execution is claimed until CI or a local environment completes those checks. Keep this milestone open until the acceptance criteria have evidence.
