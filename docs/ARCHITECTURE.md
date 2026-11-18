# ChatOS Architecture

## Direction

ChatOS is an independent operating system. The kernel, userspace, system services, and desktop are ChatOS components. Linux is not the host kernel or the base distribution.

The initial target is x86_64 UEFI, with QEMU as the first development machine. Limine is used as a bootloader/protocol implementation; it loads the ChatOS kernel but is not the ChatOS kernel itself.

## Layers

1. **Boot and architecture:** Entry point, CPU setup, interrupt handling, memory maps, page tables, and architecture-specific code.
2. **Kernel core:** Physical and virtual memory, scheduling, processes, system calls, IPC, synchronization, and time.
3. **Drivers:** Device discovery, storage, input, graphics, networking, and hardware buses.
4. **Userspace:** Init, service manager, libraries, shell, system utilities, and applications.
5. **Desktop:** Compositor, window manager, launcher, workspaces, status surfaces, and settings.
6. **AI runtime:** ChatGPT connection, permission-scoped tools, task execution, and the AI cursor.

## Initial implementation

The current bootstrap is a freestanding Rust binary compiled for x86_64-unknown-none. It uses the Limine boot protocol and writes diagnostics to the first PC serial port (COM1).

The kernel currently:
- Declares the Limine request markers and base revision.
- Enters at the x86_64 kernel entry point.
- Disables CPU interrupts until an IDT and interrupt-controller setup exist.
- Initializes COM1 and reports that the entry point was reached.
- Halts the CPU.

It does **not** yet parse bootloader responses, initialize memory management, handle interrupts, enumerate devices, or start user processes.

## Design constraints

- Keep architecture-specific code behind narrow interfaces.
- Do not enable interrupts before exception handlers and interrupt controllers are ready.
- Do not enable heap allocation before a physical allocator and kernel heap exist.
- Keep kernel and userspace privilege boundaries explicit.
- Treat all external input, including AI-generated actions, as untrusted.
- Require explicit authorization for destructive or externally visible actions.
- Keep Linux compatibility in a userspace compatibility subsystem rather than coupling the kernel to Linux internals.

## Desktop design target

The intended desktop should combine a macOS-level finish with an Omarchy-inspired keyboard-driven workflow: tiling, workspaces, fast launch/search, terminal-first tools, and a restrained cohesive visual system. This is a design target, not an existing feature.

## AI cursor architecture

The AI pointer belongs in the future compositor/input model as a separately rendered pointer identity. It must not be conflated with physical user input. The input router should tag event origin, enforce capabilities, expose pause/cancel, and require confirmation for high-impact operations.
