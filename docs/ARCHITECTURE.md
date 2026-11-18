# ChatOS Architecture

## Direction

ChatOS is an independent operating system. Its kernel, userspace, system services, and desktop are ChatOS components. Linux is not the host kernel or base distribution.

Initial target: x86_64, freestanding C11, Clang/LLD, and Limine boot protocol. The Limine BIOS path boots in QEMU and is exercised in GitHub Actions; UEFI/OVMF remains unverified.

## Layers

1. Boot and architecture: Entry point, CPU setup, memory maps, page tables, interrupt handling, and architecture-specific code.
2. Kernel core: Physical/virtual memory, scheduling, processes, system calls, IPC, synchronization, and time.
3. Drivers: Device discovery, storage, input, graphics, networking, and hardware buses.
4. Userspace: Init, service manager, libraries, shell, utilities, and applications.
5. Desktop: Compositor, window manager, launcher, workspaces, status surfaces, and settings.
6. AI runtime: ChatGPT connection, permission-scoped tools, task execution, and AI cursor.

## Current boot contract

The kernel requests Limine base protocol revision 3 and the memory-map feature. The Limine structures are kept in a local minimal header matching the published protocol layout. Bootloader-provided pointers are consumed as virtual pointers, as specified by Limine.

The base-revision marker is three 64-bit words: the two protocol magic values and requested revision. The kernel checks that the bootloader changed the third word to zero, indicating support.

## Physical memory

The current bootstrap allocator uses a static bitmap covering physical addresses below 64 GiB, with 4 KiB frames. All frames begin marked unavailable; only complete pages in Limine memory-map entries of type USABLE are released into the allocator. Physical frame zero remains reserved.

The allocator supports single-frame allocation and free. It is single-CPU bootstrap code and does not yet provide zero-on-allocation, SMP-safe locking, or dynamic bitmap growth above 64 GiB.

Limine HHDM discovery provides a direct-map offset and basic physical/virtual conversion helpers. The early x86_64 VM layer discovers the active CR3 and provides 4 KiB map/unmap/translate operations, including translation through existing 1 GiB and 2 MiB pages. It deliberately rejects user mappings and refuses to widen permissions on existing page-table ancestors. It is not yet a complete VM subsystem: there is no address-space isolation, page-table pruning/reclamation, general page ownership, or SMP/TLB-shootdown support. These paths have compile/CI coverage but still require QEMU runtime tests.

The allocator is single-CPU bootstrap code. Do not enable concurrent allocation until synchronization is implemented.

## CPU tables and exception handling

The kernel loads a private ring-0 GDT with 64-bit code/data descriptors and an available 64-bit TSS. The TSS is loaded with `ltr`; the active selector is read back and checked. A 16 KiB, 16-byte-aligned stack is assigned to TSS IST1 and IDT vector 8 (double fault). The 256-entry IDT provides dedicated assembly stubs for vectors 0–31 and a fatal fallback for other vectors. Stubs normalize error-code/no-error-code exception frames before dispatch. Fatal diagnostics include vector, reason, error code, and instruction pointer; page faults additionally report CR2. A compile-time `INT3` self-test verifies the breakpoint path and panic halt in QEMU. This does not yet verify a genuine double fault or non-fatal hardware IRQ delivery. IF remains disabled at the end of bootstrap; the PIC/APIC and timer path are not yet implemented.

## C kernel rules

- Compile freestanding; do not depend on hosted libc or a C runtime.
- Keep stack protection, PIC/PIE, and the red zone disabled until explicitly supported.
- Keep architecture-specific inline assembly behind narrow interfaces.
- Do not enable interrupts before exception handlers, an interrupt controller, and handlers/EOI paths for every unmasked IRQ are ready.
- Do not enable heap allocation before physical and virtual memory management exist.
- Keep kernel/userspace privilege boundaries explicit.
- Treat external input, including AI-generated actions, as untrusted.
- Keep Linux compatibility in a future userspace compatibility subsystem.

## Distribution editions

The two planned editions share the same kernel and userspace. ChatOS Window is a macOS-hosted VM application with an isolated virtual disk and no host bootloader/partition changes. ChatOS Native is a bare-metal installer for supported hardware, with destructive installation guarded by explicit confirmation. Host integration, installers, and both release packages remain unimplemented. Apple Silicon support is not claimed. See [Edition architecture](EDITIONS.md).

## Desktop target

The intended desktop combines a macOS-level finish with a keyboard-driven workflow: tiling, workspaces, fast launch/search, terminal-first tools, and a restrained cohesive visual system. This is a design target, not an existing feature.

## AI cursor architecture

The AI pointer belongs in the future compositor/input model as a separately rendered pointer identity. The input router must tag event origin, enforce capabilities, expose pause/cancel, and require confirmation for high-impact operations.
