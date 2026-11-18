# ChatOS Architecture

## Direction

ChatOS is an independent operating system. The kernel, userspace, system services, and desktop are ChatOS components. Linux is not the host kernel or base distribution.

The initial target is x86_64. Limine is the bootloader/protocol implementation; it loads ChatOS but is not the ChatOS kernel.

## Layers

1. **Boot and architecture:** Entry point, CPU setup, interrupt handling, memory maps, page tables, and architecture-specific code.
2. **Kernel core:** Physical and virtual memory, scheduling, processes, system calls, IPC, synchronization, and time.
3. **Drivers:** Device discovery, storage, input, graphics, networking, and hardware buses.
4. **Userspace:** Init, service manager, libraries, shell, system utilities, and applications.
5. **Desktop:** Compositor, window manager, launcher, workspaces, status surfaces, and settings.
6. **AI runtime:** ChatGPT connection, permission-scoped tools, task execution, and AI cursor.

## Initial implementation

The bootstrap is a freestanding C11 binary compiled by Clang for x86_64-unknown-none-elf and linked with LLD. It uses Limine and writes diagnostics to COM1.

The kernel currently:
- Declares Limine request markers and base revision.
- Enters through the x86_64 kernel entry point.
- Disables CPU interrupts until exception and interrupt-controller setup exists.
- Initializes COM1 and reports that the entry point was reached.
- Halts the CPU.

It does **not** yet parse bootloader responses, initialize memory management, handle interrupts, enumerate devices, or start user processes.

## C kernel rules

- Compile freestanding; do not depend on a hosted C runtime or libc.
- Keep compiler-generated stack protection, position-independent code, and the red zone disabled until explicitly supported.
- Avoid compiler builtins that could introduce unresolved runtime calls.
- Keep architecture-specific inline assembly behind narrow interfaces.
- Do not enable interrupts before exception handlers and interrupt controllers are ready.
- Do not enable heap allocation before a physical allocator and kernel heap exist.
- Keep kernel/userspace privilege boundaries explicit.
- Treat external input, including AI-generated actions, as untrusted.
- Keep Linux compatibility in a userspace compatibility subsystem.

## Desktop design target

The intended desktop should combine a macOS-level finish with an Omarchy-inspired keyboard-driven workflow: tiling, workspaces, fast launch/search, terminal-first tools, and a restrained cohesive visual system. This is a design target, not an existing feature.

## AI cursor architecture

The AI pointer belongs in the future compositor/input model as a separately rendered pointer identity. The input router should tag event origin, enforce capabilities, expose pause/cancel, and require confirmation for high-impact operations.
