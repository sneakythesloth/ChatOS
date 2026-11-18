# ChatOS Development Roadmap

The project advances through verifiable milestones. A milestone is not complete merely because files exist: it needs a successful build and a repeatable test or boot demonstration.

## M0 — Kernel bootstrap (scaffold committed)
- [x] Rust freestanding project structure.
- [x] x86_64 entry point and Limine request markers.
- [x] COM1 serial console and panic handler.
- [x] Linker script and Limine boot configuration.
- [x] ISO and QEMU helper scripts.
- [ ] Build and boot under QEMU; save the serial log.

## M1 — Boot information and memory
- [ ] Add typed Limine response structures.
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

Every major subsystem should include:
- Build instructions and compiler/toolchain versions.
- Unit or integration tests where possible.
- QEMU or hardware reproduction steps.
- Serial/debug logs for boot-critical changes.
- A short architecture note for non-obvious design decisions.
