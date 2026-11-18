# ChatOS Product Editions

ChatOS is one operating system with two distribution and execution models. Both editions share the ChatOS kernel, userspace, system services, desktop, and security model; neither edition is a Linux distribution or a macOS theme.

## ChatOS Window

ChatOS Window runs ChatOS inside a separate, resizable window on macOS.

### Initial implementation

- Package a macOS host application that manages a ChatOS virtual machine.
- Use QEMU as the first virtualization backend because the kernel's first target is x86_64 and QEMU is the planned development/test environment.
- Keep the guest disk in an app-managed image. Never repartition, rewrite, or change the Mac's boot configuration.
- Start with conservative virtual hardware: serial diagnostics, virtual display, keyboard, pointer, and virtual block storage as drivers become available.
- Make shared folders, clipboard, networking, and host-device passthrough explicit opt-in capabilities.
- Provide pause, resume, graceful shutdown, reset, and clear VM storage controls.
- Treat the VM display as a guest desktop surface, not as native macOS window-manager integration until that host application exists.

### Later macOS virtualization investigation

Evaluate Apple's Virtualization.framework/Hypervisor.framework for a more integrated backend only after confirming guest architecture compatibility, signing/entitlement requirements, and supported macOS versions. Do not imply that this backend exists today.

## ChatOS Native

ChatOS Native boots directly on supported hardware and is the edition that can replace an existing operating system.

### Initial implementation

- Target x86_64 PCs with firmware and devices that are individually tested.
- Produce a bootable installer only after the kernel, storage, input, graphics, and recovery paths exist.
- Clearly identify supported and unsupported hardware before installation.
- Default to a live/test mode where possible; do not write to internal disks merely by booting the installer.
- Any destructive install must show the exact target disk, explain that its contents will be erased, require explicit confirmation, and provide backup guidance.
- Do not claim that it can replace macOS on Apple Silicon. Apple Silicon requires a separate architecture/firmware/driver effort and remains unsupported.

## Shared source, separate delivery

The two editions must not fork the operating system implementation. They should share kernel and userspace sources while keeping separate:
- boot configuration and device profiles;
- packaging and launch/install workflows;
- virtual hardware versus physical hardware drivers and defaults;
- release validation and supported-hardware matrices.

## Current status

These are product requirements and architecture decisions, not completed deliverables. The repository currently contains early x86_64 kernel and memory-management code. It does not yet contain a macOS VM application, a desktop environment, or a native installer.