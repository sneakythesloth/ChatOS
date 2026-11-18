# ChatOS Development Roadmap

A milestone is complete only when its acceptance tests have evidence. Committing source files alone does not complete a milestone.

## M0 — C kernel bootstrap (QEMU BIOS boot verified)
- [x] Freestanding C11 entry point and x86_64 CPU primitives.
- [x] COM1 serial output and panic path.
- [x] Limine boot configuration and linker layout.
- [x] Freestanding kernel compile/link in GitHub Actions.
- [x] Boot the kernel under QEMU using the Limine BIOS path; serial bootstrap marker observed in CI.
- [x] Verify Limine BIOS boot under QEMU.
- [ ] Verify UEFI boot under OVMF.

## M1 — Boot information and physical memory (initial QEMU runtime boot verified)
- [x] Define Limine base revision 3 and memory-map request structures.
- [x] Validate bootloader base revision and memory-map response.
- [x] Validate memory-map entry pointers, lengths, and overflow.
- [x] Parse usable-memory totals.
- [x] Implement fixed 64 GiB bitmap-based 4 KiB physical frame allocator.
- [x] Add single-frame allocate/free API.
- [x] Add host-side tests for allocation order, exhaustion, reserved ranges, and invalid/double frees.
- [x] GitHub Actions kernel build and allocator test job pass.
- [x] Boot under QEMU and verify memory-map response and usable-memory totals (18 entries, 508 MiB in the CI guest).
- [ ] Exercise frame allocate/free against the actual boot memory map in QEMU.
- [x] Add Limine HHDM discovery and physical/virtual conversion helpers (compile/CI verified; runtime validation pending).
- [x] Add initial x86_64 page-table discovery and 4 KiB map/unmap/translate primitives (compile/CI verified; QEMU runtime test pending).
- [ ] Add VM host-side tests with a page-table model or suitable harness.
- [ ] Add QEMU runtime validation of HHDM and page-table operations.
- [ ] Add address-space ownership, safe reclamation, and SMP/TLB synchronization.

## Product delivery tracks (parallel to kernel milestones)

- [ ] ChatOS Window: macOS launcher/application shell, QEMU VM lifecycle, resizable display, isolated virtual disk, and explicit host-sharing permissions.
- [ ] ChatOS Native: bootable installer for validated x86_64 hardware, safe install/rollback guidance, and explicit destructive-install confirmation.
- [ ] Keep both editions on the same kernel/userspace source tree with distinct packaging and boot targets.

## M2 — CPU and interrupts (GDT/TSS/IDT and fatal exception path QEMU verified)
- [x] Load ring-0 GDT code/data descriptors and a 64-bit TSS.
- [x] Install a 256-entry IDT with CPU exception stubs and fatal serial diagnostics.
- [x] Reserve TSS IST1 as the double-fault stack and assign it to vector 8.
- [x] Exercise the breakpoint exception (INT3) in QEMU and verify vector/reason/panic diagnostics.
- [ ] Deliberately exercise the double-fault IST path under QEMU.
- [x] Remap and mask the legacy PIC; unmask only IRQ0 after installing its handler.
- [x] Add register-preserving IRQ0 dispatch and PIC EOI acknowledgement.
- [x] Program PIT channel 0 at 100 Hz and verify five real timer interrupts in QEMU using `sti; hlt`.
- [ ] Add a general monotonic clock API and calibrate against a more stable clock source.
- [ ] Test the dedicated double-fault IST path under QEMU.
- [ ] APIC interrupt-controller support.
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

## M8 — Distribution editions and compatibility
- [ ] ChatOS Window: macOS host launcher, resizable VM window, isolated virtual disk, safe shutdown and snapshots.
- [ ] ChatOS Window: evaluate QEMU first, then native macOS virtualization APIs where licensing/entitlements and device support permit.
- [ ] ChatOS Native: bootable installer for explicitly supported x86_64 hardware.
- [ ] Native installer: verified partition workflow, backup warnings, destructive-action confirmation, and recovery path.
- [ ] Shared release pipeline for Window and Native editions.

## M9 — Compatibility and hardware
- [ ] Broader x86_64 hardware and firmware support.
- [ ] ARM64 architecture port.
- [ ] Linux ELF and syscall ABI compatibility.
- [ ] Linux application packaging and dependency handling.
- [ ] Investigate Apple Silicon platform support.

## Release gates

Every major subsystem needs build instructions, toolchain details, tests where possible, QEMU or hardware reproduction steps, boot/debug logs, and notes for non-obvious architecture decisions.
