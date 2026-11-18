# ChatOS

**An independent operating system built from the ground up, with a distinctive desktop and ChatGPT as a first-class computer-use agent.**

ChatOS is a new operating system project—not a Linux distribution, desktop theme, or macOS application. It will have its own kernel, system services, application environment, desktop shell, and visual identity.

The product should feel polished and approachable to people accustomed to macOS, while taking inspiration from Omarchy's keyboard-driven workflow, tiling, workspaces, terminal-friendly tools, and cohesive visual design. Omarchy is inspiration only; ChatOS will not copy it or depend on its Linux base.

> **Project status:** Very early development. The current milestone is a minimal freestanding C kernel targeting x86_64 and the Limine boot protocol. Build and boot verification are still pending.

## Product principles

- **Independent kernel:** ChatOS owns its kernel and system architecture. It does not use the Linux kernel.
- **macOS-quality experience:** Thoughtful typography, fluid interactions, excellent defaults, coherent settings, and a modern desktop.
- **Omarchy-inspired workflow:** Keyboard-first navigation, tiling, workspaces, a strong terminal experience, and a unified aesthetic—without copying Omarchy or depending on its Linux base.
- **Build the platform ourselves:** Develop the kernel, core services, desktop shell, and first-party applications as ChatOS components.
- **Linux application portability:** Linux compatibility is a later compatibility layer objective, not the foundation of the OS.
- **AI as a native capability:** ChatGPT should be able to understand and visually operate the computer through a distinct second cursor, with explicit user control.

## Operating modes

- **NO AI:** Core system functions and applications remain useful with AI services disabled.
- **FULL AI:** ChatGPT can perform supported tasks through the ChatOS computer-use system when authorized.
- **HYBRID:** The conventional desktop remains primary, with ChatGPT available on demand. Users can watch, pause, stop, or take over AI-operated tasks.

## The second cursor

ChatOS is intended to support two visually distinct pointers: a user-controlled pointer and a separate AI-controlled pointer. AI actions must be visible and understandable. Users must be able to pause, stop, and take control immediately. Consequential actions require explicit confirmation. This belongs in the future compositor and input architecture.

## Platform direction

The first development target is x86_64 PCs and virtual machines. QEMU is the initial test environment; Limine provides the boot protocol and is not the ChatOS kernel.

ARM64 is a later architecture effort. Apple Silicon support is a long-term goal requiring platform-specific boot, firmware, and device-driver work; it is not currently supported.

ChatOS will not be Linux-based. Linux application support requires a deliberate compatibility layer, including an ELF loader, Linux ABI/syscall compatibility, process and filesystem semantics, and packaging.

## Repository layout

- `src/kernel.c`: kernel entry point and Limine request markers.
- `src/arch/x86_64/`: x86_64 CPU and serial primitives.
- `src/include/`: kernel headers.
- `src/panic.c`: fatal kernel diagnostics.
- `linker.ld`: kernel ELF layout.
- `scripts/`: freestanding build, ISO creation, and QEMU launch.
- `docs/`: architecture, roadmap, and milestone records.

The kernel is written in **freestanding C11**. It does not use a hosted C runtime or standard library.

## Build the kernel

Prerequisites: Clang with the x86_64-unknown-none-elf target, LLD, and Bash.

    bash scripts/build-kernel.sh

The output is `build/chatos.elf`.

## Build a bootable ISO

Install xorriso and obtain the Limine BIOS/UEFI files plus the Limine utility. Set `LIMINE_DIR` to their directory:

    LIMINE_DIR=/path/to/limine bash scripts/build-iso.sh

Run in QEMU:

    bash scripts/run-qemu.sh

Set `OVMF_CODE` to an OVMF firmware image to exercise UEFI boot. Otherwise QEMU uses its default firmware path. The kernel currently reports startup through COM1 serial output and halts.

## Roadmap

1. **M0 — Kernel bootstrap:** C entry point, Limine request markers, serial output, linker and boot scripts; then prove build and QEMU boot.
2. **M1 — Memory:** validate boot responses, parse the memory map, implement physical frames and virtual memory.
3. **M2 — CPU and interrupts:** GDT, TSS, IDT, exceptions, interrupt controllers, and timers.
4. **M3 — Drivers:** PCI, storage, keyboard, pointer, framebuffer, and driver model.
5. **M4 — Processes:** privilege separation, scheduler, system calls, ELF loader, and IPC.
6. **M5 — Userspace:** VFS, init, service manager, shell, terminal, and package management.
7. **M6 — Desktop:** compositor, tiling, workspaces, launcher, settings, and first-party apps.
8. **M7 — AI computer use:** secure ChatGPT integration, permissions, second cursor, pause/stop, and takeover.
9. **M8 — Compatibility and hardware:** Linux ABI compatibility, broader x86_64 support, ARM64, and Apple Silicon investigation.

## Current implementation status

The current kernel only disables interrupts, initializes COM1, prints a startup report, and halts. There is no scheduler, memory manager, filesystem, user process, graphics stack, desktop, or Linux compatibility layer yet. No successful build or boot is claimed until the checks have actually run.

## License

No license has been specified. Until one is added, do not assume the code is available for reuse, modification, or redistribution.
