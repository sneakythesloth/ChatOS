# ChatOS Development Roadmap

A milestone is complete only when its acceptance tests have evidence. Committing source files alone does not complete a milestone.

## M0 — C kernel bootstrap (implementation scaffold; boot proof pending)
- [x] Freestanding C11 entry point and x86_64 CPU primitives.
- [x] COM1 serial output and panic path.
- [x] Limine boot configuration and linker layout.
- [ ] Build and boot the kernel under QEMU.
- [ ] Verify BIOS and UEFI boot paths.

## M1 — Boot information and physical memory (implementation scaffold; verification pending)
- [x] Define Limine base revision 3 and memory-map request structures.
- [x] Validate bootloader base revision and memory-map response.
- [x] Validate memory-map entry pointers, lengths, and overflow.
- [x] Parse usable-memory totals.
- [x] Implement fixed 64 GiB bitmap-based 4 KiB physical frame allocator.
- [x] Add single-frame allocate/free API.
- [ ] Add unit tests for allocator edge cases.
- [ ] Boot under QEMU and verify reported memory.
- [ ] Verify allocation/free using actual boot memory map.
- [ ] Add HHDM helpers and page-table management in a subsequent memory substage.

## M2 — CPU and interrupts
- [ ] GDT and TSS setup.
- [ ] IDT and exception diagnostics.
- [ ] APIC interrupt-controller support.
- [ ] Timer and monotonic clock.
- [ ] SMP discovery and per-CPU state.

## M3 — Devices
- [ ] PCI enumeration.
- [ ] Block-device abstraction and initial storage driver.
- [ ] Keyboard and pointer input.
- [ ] Framebuffer console and graphics primitives.
- [ ] Driver lifecycle and resource ownership.

## M4 — Processes
- [ ] User/kernel privilege separation.
- [ ] Scheduler and context switching.
- [ ] ELF userspace loader.
- [ ] System-call ABI.
- [ ] IPC and process lifecycle.

## M5 — Userspace and storage
- [ ] VFS and initial writable filesystem.
- [ ] Init and service manager.
- [ ] Shell, terminal, and basic utilities.
- [ ] App package format and package manager.
- [ ] User and permission model.

## M6 — Desktop
- [ ] Compositor and window surfaces.
- [ ] Tiling window manager and keyboard navigation.
- [ ] Workspaces, launcher, status bar, and notifications.
- [ ] Design system, typography, motion, and settings.
- [ ] First-party applications.

## M7 — AI computer use
- [ ] Secure ChatGPT integration.
- [ ] Capability-based tool permissions.
- [ ] Distinct second cursor rendered by compositor.
- [ ] Visual targeting and controlled input actions.
- [ ] Narration, action history, pause/stop, and user takeover.
- [ ] NO AI, FULL AI, and HYBRID mode enforcement.

## M8 — Compatibility and hardware
- [ ] Broader x86_64 hardware and firmware support.
- [ ] ARM64 architecture port.
- [ ] Linux ELF and syscall ABI compatibility.
- [ ] Linux application packaging and dependency handling.
- [ ] Investigate Apple Silicon platform support.

## Release gates

Every major subsystem needs build instructions, toolchain details, tests where possible, QEMU or hardware reproduction steps, boot/debug logs, and notes for non-obvious architecture decisions.
