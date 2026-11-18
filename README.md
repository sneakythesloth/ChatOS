# ChatOS

**An independent operating system built in C from the ground up, with a distinctive desktop and ChatGPT as a first-class computer-use agent.**

ChatOS is not a Linux distribution, desktop theme, or macOS application. It is intended to have its own kernel, system services, application environment, desktop shell, and visual identity.

The product target is a polished, approachable experience for people accustomed to macOS, combined with an Omarchy-inspired keyboard-driven workflow: tiling, workspaces, terminal-first tools, and a cohesive visual system. Omarchy is inspiration only; ChatOS will not copy it or depend on its Linux base.

> **Status:** Early kernel development. The C kernel builds in GitHub Actions and the physical-frame allocator tests pass. QEMU boot verification remains outstanding.

## Principles

- **Independent kernel:** ChatOS owns its kernel and system architecture. It does not use the Linux kernel.
- **C kernel:** The kernel is freestanding C11, initially targeting x86_64.
- **macOS-quality experience:** Thoughtful typography, fluid interactions, excellent defaults, coherent settings, and a modern desktop.
- **Keyboard-first workflow:** Tiling, workspaces, fast launch/search, and a strong terminal experience.
- **First-party platform:** Kernel, core services, desktop shell, and first-party applications are ChatOS components.
- **Linux portability later:** Linux application support will be a compatibility layer, not the OS foundation.
- **AI as a native capability:** ChatGPT should operate the computer through a distinct second cursor, with visible actions and explicit user control.

## Operating modes

- **NO AI:** Core system functions and applications remain useful with AI services disabled.
- **FULL AI:** ChatGPT can perform supported tasks through permission-scoped computer-use tools.
- **HYBRID:** The conventional desktop remains primary, with ChatGPT available on demand.

The AI pointer must be visually distinct from the user pointer. Users must be able to observe, pause, stop, and take over. Consequential actions require confirmation.

## Platform direction

The first target is x86_64 PCs and virtual machines. QEMU is the intended initial test environment. Limine is the boot protocol/bootloader interface; it is not the ChatOS kernel.

ARM64 is a later architecture effort. Apple Silicon support is a long-term goal requiring platform-specific boot, firmware, and driver work; it is not currently supported.

## Repository layout

- src/kernel.c: kernel entry point.
- src/boot/: Limine request structures and declarations.
- src/arch/x86_64/: x86_64 CPU and serial primitives.
- src/memory/: memory map handling and physical frame allocator.
- src/include/: shared kernel headers.
- src/panic.c: fatal kernel diagnostics.
- linker.ld: kernel ELF layout.
- scripts/: kernel build, tests, ISO creation, and QEMU launch.
- tests/: host-side kernel subsystem tests.
- docs/: architecture, roadmap, and milestone records.

## Build and test

Prerequisites: Clang with the x86_64-unknown-none-elf target, LLD, a host C compiler, and Bash.

    bash scripts/test-frame-allocator.sh
    bash scripts/build-kernel.sh

The kernel output is build/chatos.elf.

For a bootable ISO, install xorriso and obtain Limine's BIOS/UEFI image files and executable. Then:

    LIMINE_DIR=/path/to/limine bash scripts/build-iso.sh
    bash scripts/run-qemu.sh

Set OVMF_CODE to an OVMF firmware image to test the UEFI path.

## Current kernel capabilities

- Freestanding C11 x86_64 entry point.
- Limine base revision 3 request and memory-map response.
- Validation of memory-map response and entries.
- A fixed-capacity bitmap physical frame allocator for the first 64 GiB of physical address space.
- COM1 serial diagnostics and a fatal panic path.
- Host-side allocator tests run by GitHub Actions.

The M1 CI run passed: host-side allocator tests and the freestanding kernel build both succeeded. This is compile/test evidence, not proof that the kernel boots under QEMU or on physical hardware.

The frame allocator returns physical addresses; it does not yet map or zero allocated pages. It is an early bootstrap allocator, not a complete virtual-memory manager. The bitmap is static, allocations are currently single-CPU only, and SMP locking is not implemented.

There is no scheduler, IDT/exception handling, filesystem, user process, graphics stack, desktop, networking, or Linux compatibility layer yet. Do not treat planned capabilities as implemented.

## Roadmap

1. M0 — Kernel bootstrap: C entry, Limine, serial output, linker and build foundation; prove boot.
2. M1 — Boot information and physical memory: validate Limine responses, parse memory map, allocate/free physical frames.
3. M2 — CPU and interrupts: GDT, TSS, IDT, exceptions, interrupt controllers, timers.
4. M3 — Drivers: PCI, storage, keyboard, pointer, framebuffer, driver model.
5. M4 — Processes: privilege separation, scheduler, syscalls, ELF loader, IPC.
6. M5 — Userspace: VFS, init, service manager, shell, terminal, package management.
7. M6 — Desktop: compositor, tiling, workspaces, launcher, settings, first-party apps.
8. M7 — AI computer use: secure ChatGPT integration, permissions, second cursor, pause/stop, takeover.
9. M8 — Compatibility and hardware: Linux ABI compatibility, broader x86_64 support, ARM64, Apple Silicon investigation.

## Engineering policy

A milestone is not complete just because files have been committed. It needs successful builds and repeatable tests or boot evidence. Each major milestone will have a GitHub record, build instructions, test evidence, and known limitations.

## License

No license has been specified. Until one is added, do not assume the code is available for reuse, modification, or redistribution.
