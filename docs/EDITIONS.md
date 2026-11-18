# ChatOS Distribution Editions

ChatOS is one operating system with two distribution and launch paths. The editions share the same kernel, userspace ABI, system services, and desktop; they differ in how the machine is booted and which devices are presented.

## ChatOS Window

**Goal:** run ChatOS in a separate, resizable window on macOS without replacing or modifying macOS.

- A signed/notarized macOS host application will own VM lifecycle and present the guest display in a normal resizable window.
- QEMU is the initial development and compatibility path. Native macOS virtualization APIs may be evaluated later; neither implementation is complete.
- The guest boots the same ChatOS x86_64 image used for virtual-machine testing.
- Use an isolated virtual disk image by default. The application must not edit host partitions, boot configuration, or startup settings.
- Shared folders, clipboard, networking, USB passthrough, and host filesystem access must be explicit opt-ins with visible permission boundaries.
- Provide graceful shutdown, suspend/resume where supported, snapshots, and clear recovery/reset controls before calling the edition ready.

## ChatOS Native

**Goal:** boot ChatOS directly on supported hardware and allow users to install it as their primary operating system.

- Initial target: explicitly qualified x86_64 PCs with supported firmware and devices.
- Deliver a bootable installer and a recovery path. A bootable ISO alone is not a finished installer.
- Replacing an existing OS is destructive. The installer must identify the target disk, explain data loss, require explicit confirmation, and verify backups are recommended before writing.
- Dual boot is a separate feature and must not be offered until partition discovery, resizing, boot entry management, and rollback are tested on supported hardware.
- Hardware compatibility must be published by device/model and component. Do not imply universal PC support.
- Apple Silicon is not currently supported. Its boot chain, firmware interfaces, and drivers require a separate investigation and implementation effort.

## Shared engineering contract

- Both editions consume the same versioned kernel/userspace build artifacts.
- VM-specific devices and host integration belong in the Window host/virtual-device layer, not as a forked kernel.
- Physical drivers and firmware boot paths belong in the Native platform layer.
- CI must build and test shared components once, then validate each edition's packaging/boot path independently as those targets become real.
- Product status must distinguish design, implementation, build success, emulator boot, and physical-hardware validation. Mockups are not screenshots of a running system.
