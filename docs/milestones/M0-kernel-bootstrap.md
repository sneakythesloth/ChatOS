# Milestone 0: Kernel Bootstrap

**Status:** Scaffold committed; boot verification pending.  
**Target:** x86_64, Limine boot protocol, QEMU.

## Objective

Create the smallest useful independent ChatOS kernel foundation: a freestanding binary with a defined entry point, basic diagnostics, and a reproducible path toward a bootable ISO.

## Delivered files

- Cargo manifest and pinned Rust target configuration.
- x86_64 kernel entry point.
- Limine request start/end markers and base revision declaration.
- COM1 serial driver and formatted kernel console.
- Panic handler.
- Linker script and Limine boot menu configuration.
- ISO build script and QEMU launch script, including optional OVMF firmware selection.
- GitHub Actions workflow for formatting and release compilation.
- Architecture and milestone documentation.

## Current kernel behaviour

The entry point disables interrupts, initializes the first PC serial port (COM1), prints a short startup report, and halts the CPU.

This is deliberately not presented as a functioning general-purpose OS. Memory management, exception/interrupt handling, drivers, scheduling, storage, userspace, graphics, and networking are not implemented yet.

## Acceptance criteria

- [ ] Rust release build completes for x86_64-unknown-none.
- [ ] ISO is generated with Limine BIOS and UEFI boot files.
- [ ] QEMU reaches the ChatOS kernel entry point.
- [ ] Serial output contains the kernel startup report.
- [ ] Optional OVMF boot path is verified.
- [ ] Build/boot logs are attached to the milestone or CI run.

## Verification record

No local build or QEMU execution has been performed as part of this GitHub upload. Do not mark this milestone complete until the acceptance criteria have been tested. The CI workflow is committed and should provide the first compiler feedback when GitHub Actions runs.
