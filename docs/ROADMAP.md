# ChatOS Development Roadmap

Milestones require a successful build and a repeatable test or boot demonstration. Files existing in the repository do not by themselves complete a milestone.

## M0 — C kernel bootstrap (scaffold committed; verification pending)
- [x] Freestanding C11 kernel entry point and Limine request markers.
- [x] x86_64 interrupt-disable and halt primitives.
- [x] COM1 serial console and panic routine.
- [x] Linker script, Limine configuration, and build/ISO/QEMU scripts.
- [ ] Build with Clang/LLD and verify the ELF.
- [ ] Boot under QEMU and save the serial log.
- [ ] Verify UEFI path with OVMF.

## M1 — Boot information and memory
- [ ] Add ABI-correct Limine response structures.
- [ ] Validate bootloader base revision and memory-map response.
- [ ] Parse memory regions and reserve kernel/bootloader-owned ranges.
- [ ] Implement a physical frame allocator.
- [ ] Add page-table inspection and kernel virtual-memory management.
- [ ] Add guarded kernel heap allocation.

## M2 — CPU and interrupts
- [ ] GDT and TSS setup.
- [ ] IDT and exception diagnostics.
- [ ] APIC interrupt-controller support.
- [ ] Timer and monotonic clock.
- [ ] SMP discovery and safe per-CPU state.

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
- [ ] IPC, process lifecycle, and signals/events.

## M5 — Userspace and storage
- [ ] VFS and initial writable filesystem.
- [ ] Init and service manager.
- [ ] Shell, terminal, and basic utilities.
- [ ] App package format, install/update/uninstall.
- [ ] User and permission model.

## M6 — Desktop
- [ ] Compositor and window surfaces.
- [ ] Tiling window manager and keyboard navigation.
- [ ] Workspaces, launcher, status bar, and notifications.
- [ ] Design system, typography, motion, and settings.
- [ ] First-party applications and terminal.

## M7 — AI computer use
- [ ] Secure ChatGPT integration.
- [ ] Capability-based tool permissions.
- [ ] Distinct second cursor rendered by the compositor.
- [ ] Visual targeting and controlled input actions.
- [ ] Narration, action history, pause/stop, and user takeover.
- [ ] NO AI, FULL AI, and HYBRID mode enforcement.

## M8 — Compatibility and hardware
- [ ] Broader x86_64 hardware and firmware support.
- [ ] ARM64 architecture port.
- [ ] Linux ELF and syscall ABI compatibility.
- [ ] Linux application installation and dependency handling.
- [ ] Investigate Apple Silicon platform support.

## Release gates

Every major subsystem should include build instructions, compiler/toolchain versions, tests where possible, QEMU or hardware reproduction steps, serial/debug logs for boot-critical changes, and architecture notes for non-obvious decisions.
