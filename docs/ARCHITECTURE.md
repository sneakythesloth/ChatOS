# ChatOS Architecture

## Direction

ChatOS is an independent operating system. Its kernel, userspace, system services, and desktop are ChatOS components. Linux is not the host kernel or base distribution.

Initial target: x86_64, freestanding C11, Clang/LLD, and Limine boot protocol. QEMU is the planned first emulator target.

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

The allocator supports single-frame allocation and free. It does not yet provide:
- HHDM conversion helpers or virtual mappings.
- Zero-on-allocation.
- Page-table management or a kernel heap.
- SMP-safe locking.
- Dynamic bitmap growth above 64 GiB.

The allocator is single-CPU bootstrap code. Do not enable concurrent allocation until synchronization is implemented.

## C kernel rules

- Compile freestanding; do not depend on hosted libc or a C runtime.
- Keep stack protection, PIC/PIE, and the red zone disabled until explicitly supported.
- Keep architecture-specific inline assembly behind narrow interfaces.
- Do not enable interrupts before exception handlers and interrupt controllers are ready.
- Do not enable heap allocation before physical and virtual memory management exist.
- Keep kernel/userspace privilege boundaries explicit.
- Treat external input, including AI-generated actions, as untrusted.
- Keep Linux compatibility in a future userspace compatibility subsystem.

## Desktop target

The intended desktop combines a macOS-level finish with a keyboard-driven workflow: tiling, workspaces, fast launch/search, terminal-first tools, and a restrained cohesive visual system. This is a design target, not an existing feature.

## AI cursor architecture

The AI pointer belongs in the future compositor/input model as a separately rendered pointer identity. The input router must tag event origin, enforce capabilities, expose pause/cancel, and require confirmation for high-impact operations.
