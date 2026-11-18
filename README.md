# ChatOS

**An independent operating system built in C from the ground up, with a distinctive desktop and ChatGPT as a first-class computer-use agent.**

ChatOS is not a Linux distribution, desktop theme, or macOS application. It is intended to have its own kernel, system services, application environment, desktop shell, and visual identity.

The product target is a polished, approachable experience for people accustomed to macOS, combined with an Omarchy-inspired keyboard-driven workflow: tiling, workspaces, terminal-first tools, and a cohesive visual system. Omarchy is inspiration only; ChatOS will not copy it or depend on its Linux base.

> **Status:** Early kernel development, not yet a usable desktop operating system. The Limine BIOS image boots in QEMU and CI confirms the kernel bootstrap, memory-map accounting, GDT/TSS/IDT installation, and a deliberate `INT3` exception/panic path. The kernel has not yet reached userspace, graphics, storage, networking, or desktop functionality. UEFI boot and runtime HHDM/page-table operations remain unverified.

## Screenshots

There is not yet a genuine ChatOS desktop screenshot to show: the repository does not currently contain a graphical shell or a booted desktop. We will add screenshots captured from the running QEMU system as soon as the framebuffer/console path is implemented and verified. **No concept renders are presented as product screenshots.**

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

## Editions

ChatOS has two intended distribution editions sharing the same kernel and userspace:

- **ChatOS Window:** a resizable macOS-hosted virtual-machine window. The host app will launch a ChatOS VM (QEMU is the initial development path); its virtual disk is isolated from the Mac by default. It must never alter the host bootloader or partitions. This is a planned product, not yet packaged or runnable.
- **ChatOS Native:** a bootable installer for explicitly supported x86_64 PCs. It installs ChatOS directly to hardware and can replace the existing operating system only after clear destructive-install warnings and explicit confirmation. Dual-boot support will be offered only where partition handling is proven safe. Apple Silicon is not currently supported.

Both editions use the same ChatOS kernel, services, and desktop. The Window edition adds a host-side VM launcher and virtual devices; Native uses firmware boot and physical device drivers. See [Product edition requirements](docs/PRODUCT_EDITIONS.md).

## Platform direction

The first kernel target is x86_64 PCs and virtual machines. QEMU is the intended initial test environment. Limine is the boot protocol/bootloader interface; it is not the ChatOS kernel.

ARM64 is a later architecture effort. Apple Silicon support is a long-term investigation requiring platform-specific boot, firmware, and driver work; it is not currently supported. ChatOS Native must not be advertised as replacing macOS on Apple Silicon until that hardware path is implemented and tested.

## Repository layout

- `src/kernel.c`: kernel entry point.
- `src/boot/`: Limine request structures and declarations.
- `src/arch/x86_64/`: x86_64 CPU, serial, GDT/TSS, IDT, and exception-entry primitives.
- `src/memory/`: memory map handling, physical frame allocator, HHDM helpers, and early page-table primitives.
- `docs/PRODUCT_EDITIONS.md`: Window and Native edition architecture and safety requirements.
- `docs/RELEASES.md`: separate host-app and full-OS release channels and acceptance gates.
- `src/include/`: shared kernel headers.
- `src/panic.c`: fatal kernel diagnostics.
- `linker.ld`: kernel ELF layout.
- `scripts/`: kernel build, tests, ISO creation, QEMU launch, and QEMU boot smoke test.
- `tests/`: host-side kernel subsystem tests.
- `docs/`: architecture, roadmap, and milestone records.

## Build and test

Prerequisites: Clang with the x86_64-unknown-none-elf target, LLD, a host C compiler, and Bash.

```sh
bash scripts/test-frame-allocator.sh
bash scripts/build-kernel.sh
```

The kernel output is `build/chatos.elf`.

For a bootable ISO, install xorriso and obtain Limine's BIOS/UEFI image files and executable. Then:

```sh
LIMINE_DIR=/path/to/limine bash scripts/build-iso.sh
bash scripts/run-qemu.sh
```

For an automated headless boot check (after building the ISO):

```sh
bash scripts/test-qemu-boot.sh
```

Set `OVMF_CODE` to an OVMF firmware image to test the UEFI path.

## Current kernel capabilities

- Freestanding C11 x86_64 entry point and Limine base revision 3 boot requests; Limine BIOS boot verified in QEMU CI.
- Validated Limine memory-map response and usable-memory accounting.
- Fixed-capacity bitmap physical-frame allocator for the first 64 GiB, with host-side allocation tests.
- Limine HHDM discovery and basic physical/virtual conversion helpers.
- Early x86_64 active-page-table discovery and 4 KiB map/unmap/translate primitives.
- COM1 serial diagnostics and a fatal panic path.
- Ring-0 GDT and 64-bit TSS setup, including a dedicated IST1 double-fault stack.
- 256-entry IDT with CPU exception stubs, error-code frame normalization, page-fault CR2 diagnostics, and fatal fallback handlers.
- QEMU CI boot test and an `INT3` exception self-test; host-side physical allocator tests and ELF load-layout validation.

Compilation and host allocator tests do not prove physical-hardware operation. HHDM and page-table code still require dedicated runtime validation; only successful kernel bootstrap and memory-map accounting have been verified under QEMU. The memory subsystem is early bootstrap code, not a complete virtual-memory manager. It has no address-space isolation, page-table reclamation, general page ownership, guard-page policy, or SMP/TLB-shootdown support. User mappings are intentionally rejected until privilege-separated address spaces are implemented. The physical-frame bitmap is static and allocation is single-CPU only.

There is no scheduler, non-fatal IRQ/timer path, filesystem, user process, graphics stack, desktop, networking, or Linux compatibility layer yet. Do not treat planned capabilities as implemented.

## Roadmap

1. M0 — Kernel bootstrap: C entry, Limine BIOS, serial output, linker and build foundation; QEMU boot verified, UEFI remains to be tested.
2. M1 — Boot information and physical memory: validate Limine responses, parse memory map, allocate/free physical frames.
3. M2 — CPU and interrupts: GDT/TSS/IDT and fatal exceptions implemented; PIC/APIC, IRQ dispatch, timer and SMP remain.
4. M3 — Drivers: PCI, storage, keyboard, pointer, framebuffer/graphics.
5. M4 — Processes: privilege separation, scheduler, syscalls, ELF loader, IPC.
6. M5 — Userspace: VFS, init, service manager, shell, terminal, package management.
7. M6 — Desktop: compositor, tiling, workspaces, launcher, settings, first-party apps.
8. M7 — AI computer use: secure ChatGPT integration, permissions, second cursor, pause/stop, takeover.
9. M8 — Compatibility and hardware: Linux ABI compatibility, broader x86_64 support, ARM64, Apple Silicon investigation.

## Engineering policy

A milestone is not complete just because files have been committed. It needs successful builds and repeatable tests or boot evidence. Each major milestone will have a GitHub record, build instructions, test evidence, and known limitations.

## License

No license has been specified. Until one is added, do not assume the code is available for reuse, modification, or redistribution.
