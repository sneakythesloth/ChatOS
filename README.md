# ChatOS

**An independent operating system built from the ground up, with a distinctive desktop and ChatGPT as a first-class computer-use agent.**

ChatOS is a new operating system project—not a Linux distribution, desktop theme, or macOS application. It will have its own kernel, system services, application environment, desktop shell, and visual identity.

The product should feel polished and approachable to people accustomed to macOS, while taking inspiration from Omarchy's keyboard-driven workflow, tiling, terminal-friendly tools, and cohesive visual design. The long-term goal is to build the experience ourselves rather than assemble a conventional Linux desktop.

> **Project status:** Very early development. The first milestone is a minimal x86_64 kernel booting under UEFI through the Limine boot protocol. Most operating-system features described below are planned, not yet implemented.

## Product principles

- **Independent kernel:** ChatOS owns its kernel and system architecture. It does not use the Linux kernel.
- **macOS-quality experience:** Thoughtful typography, fluid interactions, excellent defaults, coherent settings, and a modern desktop.
- **Omarchy-inspired workflow:** Keyboard-first navigation, tiling, workspaces, a strong terminal experience, and a unified aesthetic—without copying Omarchy or depending on its Linux base.
- **Build the platform ourselves:** Develop the kernel, core services, desktop shell, and first-party applications as ChatOS components.
- **Linux application portability:** Linux compatibility is a later compatibility layer objective, not the foundation of the OS.
- **AI as a native capability:** ChatGPT should be able to understand and visually operate the computer through a distinct second cursor, with explicit user control.

## Operating modes

### NO AI
A complete operating experience with AI services disabled. Core system functions and applications must remain useful without an AI connection.

### FULL AI
ChatGPT is a first-class assistant. It can understand requests and, when authorized, perform supported tasks through the ChatOS computer-use system.

### HYBRID
The conventional desktop remains primary, with ChatGPT available on demand. Users can watch, pause, stop, or take over AI-operated tasks.

## The second cursor

ChatOS is intended to support two visually distinct pointers:

- **User cursor:** Controlled by the user's mouse or trackpad.
- **AI cursor:** A separate on-screen pointer controlled by ChatGPT during an approved task.

The AI cursor should be able to point, click, type, drag, and scroll through supported system interfaces. Its actions must be visible and understandable. Users must be able to pause, stop, and take control immediately. Consequential actions require explicit confirmation.

This will be implemented as a first-class part of the ChatOS compositor/windowing and input architecture—not as an afterthought. The exact mechanism will depend on the graphics and input stack built for the OS.

## Platform and architecture direction

### Initial target: x86_64 UEFI

The first development target is x86_64 PCs and virtual machines booting through UEFI. QEMU will be the initial test environment. This gives the project a practical route to a real, independently bootable kernel.

### Future hardware targets

- Physical x86_64 PCs after virtual-machine boot and driver foundations are stable.
- ARM64 as a later architecture milestone.
- Apple Silicon support is a long-term goal, not an initial compatibility claim. It requires substantial platform-specific boot, firmware, and device-driver work.

### Linux application compatibility

ChatOS will not be Linux-based. Running Linux applications requires a deliberate compatibility strategy, including an ELF loader, Linux ABI/syscall compatibility, process and filesystem semantics, and a packaging/install workflow. This is a later milestone and will not be treated as automatic compatibility.

## Repository structure

The initial kernel is written in Rust with no_std and targets x86_64. The first boot path uses the Limine boot protocol. The repository will grow into separate kernel, architecture, memory, process, driver, userspace, and desktop components as those systems are implemented.

## Build and run the kernel

The initial milestone targets QEMU on x86_64.

Prerequisites:
- Rust toolchain and the x86_64-unknown-none target.
- QEMU with x86_64 system emulation.
- xorriso.
- Limine bootloader binaries and the Limine utility.

Build the bootable ISO (set LIMINE_DIR to the directory containing the Limine binaries):

    rustup target add x86_64-unknown-none
    LIMINE_DIR=/path/to/limine bash scripts/build-iso.sh

Run it in QEMU:

    bash scripts/run-qemu.sh

The ISO includes Limine BIOS and UEFI boot paths. The QEMU helper uses the default firmware; set OVMF_CODE to an OVMF firmware image to test UEFI boot. The initial kernel reports startup over the emulated serial console. Build and boot instructions will expand as memory management, interrupts, and device support are added.

## Development roadmap

### Milestone 0 — Bootable kernel foundation
- [x] Establish a freestanding Rust kernel project.
- [x] Add x86_64 entry point and Limine request markers.
- [x] Add serial output and panic reporting.
- [x] Add linker script, boot configuration, and QEMU ISO scripts.
- [ ] Verify the first boot in QEMU and capture the boot log.

### Milestone 1 — Boot protocol and physical memory
- [ ] Parse and validate bootloader responses.
- [ ] Read the UEFI/bootloader memory map.
- [ ] Implement physical page-frame allocation.
- [ ] Establish kernel virtual memory and page-table management.
- [ ] Add heap allocation after the page allocator is reliable.

### Milestone 2 — CPU, interrupts, and time
- [ ] Set up the Global Descriptor Table and Task State Segment.
- [ ] Configure the Interrupt Descriptor Table and exception handlers.
- [ ] Add programmable interrupt controller support.
- [ ] Add a monotonic clock and timer interrupts.
- [ ] Improve diagnostics for CPU exceptions and kernel faults.

### Milestone 3 — Drivers and hardware discovery
- [ ] Enumerate PCI/PCIe devices.
- [ ] Add storage and block-device abstractions.
- [ ] Add keyboard and pointer input.
- [ ] Add framebuffer and graphics output.
- [ ] Establish a driver model and device permissions.

### Milestone 4 — Processes and system calls
- [ ] Define user/kernel privilege boundaries.
- [ ] Implement context switching and scheduling.
- [ ] Define a stable system-call ABI.
- [ ] Load and execute user-space ELF programs.
- [ ] Add process lifecycle, signals/events, and IPC foundations.

### Milestone 5 — Storage and userspace
- [ ] Implement a virtual filesystem layer.
- [ ] Add an initial writable filesystem and persistent storage.
- [ ] Build an init process and service manager.
- [ ] Add a shell, terminal, and basic system utilities.
- [ ] Define application packaging, installation, and updates.

### Milestone 6 — Graphics and desktop
- [ ] Build a graphics stack and compositor.
- [ ] Implement windows, surfaces, focus, and input routing.
- [ ] Create a keyboard-driven launcher and tiling window manager.
- [ ] Add workspaces, status bar, notifications, and system settings.
- [ ] Establish a coherent design system with polished motion and typography.
- [ ] Build first-party applications.

### Milestone 7 — ChatGPT and computer use
- [ ] Implement secure AI service integration.
- [ ] Define permission-scoped tools and user approval.
- [ ] Add the separate AI cursor to the compositor.
- [ ] Support visual targeting and controlled input injection.
- [ ] Add action narration, pause/stop, user takeover, and audit history.
- [ ] Implement NO AI, FULL AI, and HYBRID policies end to end.

### Milestone 8 — Compatibility and hardware expansion
- [ ] Improve x86_64 hardware support and test on physical machines.
- [ ] Design an ARM64 architecture port.
- [ ] Implement Linux ELF and ABI compatibility in a controlled environment.
- [ ] Add Linux application installation and dependency management.
- [ ] Evaluate Apple Silicon boot and device support as a separate platform effort.

## Current implementation status

Only the minimal kernel bootstrap is being established. There is not yet a general-purpose OS: no scheduler, user processes, filesystem, graphics compositor, desktop, or Linux compatibility layer. Each milestone should add working, reviewable functionality and include build/test evidence before being marked complete.

## Contributing

Contributions and design discussions are welcome. Please identify the affected architecture, subsystem, and milestone. Kernel changes should include build instructions and reproducible test steps.

## License

No license has been specified yet. Until a license is added, do not assume the code is available for reuse, modification, or redistribution.
