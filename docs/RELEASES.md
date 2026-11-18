# ChatOS release channels

ChatOS will publish two independent release products. They share the kernel/userspace source tree, but their packages, installation paths, release notes, and acceptance tests must remain distinct.

## 1. ChatOS Window — host application

**Release asset:** a signed/notarized macOS application bundle or disk image, only when the host launcher is implemented.

Purpose:
- Runs ChatOS in a resizable window on macOS.
- Creates and manages an isolated virtual disk and VM configuration.
- Does not write to the Mac's bootloader, partition table, or internal disk.
- Provides start, stop, pause, reset, and VM resource controls.
- Uses the same ChatOS guest kernel and userspace as Native.

The initial development backend may be QEMU. A native Apple virtualization backend is a later option and must not be implied until implemented and tested. The Window application is not currently implemented or releasable.

## 2. ChatOS Native — complete operating system

**Release asset:** bootable x86_64 installer ISO, and later verified USB/disk images where appropriate.

Purpose:
- Boots directly on explicitly supported x86_64 hardware.
- Installs the complete ChatOS system to a user-selected target.
- Clearly identifies the target disk and warns that installation is destructive.
- Requires a second explicit confirmation before erasing or repartitioning.
- Provides backup guidance and refuses ambiguous disk selection.
- Never advertises Apple Silicon support until an ARM64 boot and driver path is implemented and tested.

A bootable kernel ISO is not yet a complete operating-system installer. Native releases must wait for a usable userspace, storage/filesystem support, input, graphics, installer, and hardware validation.

## Release naming and repository layout

Use separate GitHub Releases rather than combining both products into one download:

- `window-vX.Y.Z` — ChatOS Window host application only.
- `native-vX.Y.Z` — ChatOS Native bootable OS image only.

Each release must include:
- A product-specific changelog and supported-host/hardware matrix.
- SHA-256 checksums for every downloadable asset.
- Build provenance and the exact source commit.
- Known limitations and recovery/uninstall instructions.
- Test evidence appropriate to the product.

Do not create a public release tag or attach placeholder files before the corresponding product builds and passes its release acceptance tests. Do not label development ISOs as stable Native releases.

## Release acceptance gates

### Window
- Clean install and launch on each declared supported macOS version.
- VM starts, displays the guest, and shuts down cleanly.
- VM storage remains isolated from host disks.
- Pause/resume, reset, and failure recovery work.
- No host disk/bootloader modification.

### Native
- Reproducible boot on every declared hardware target.
- Installer correctly identifies disks and requires destructive-action confirmation.
- Install, first boot, reboot, shutdown, and recovery tested.
- Keyboard, pointer, display, and storage drivers work on the supported matrix.
- Checksums and installation documentation verified.

Until these gates pass, both channels remain planned products, not downloadable releases.
