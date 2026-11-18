#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

LIMINE_DIR="${LIMINE_DIR:-.}"
KERNEL="build/chatos.elf"
ISO_ROOT="iso_root"

for tool in clang ld.lld xorriso; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "Missing required tool: $tool" >&2
        exit 1
    }
done

for file in "$LIMINE_DIR/limine-bios-cd.bin" "$LIMINE_DIR/limine-uefi-cd.bin" "$LIMINE_DIR/limine-bios.sys" "$LIMINE_DIR/BOOTX64.EFI" "$LIMINE_DIR/limine"; do
    test -f "$file" || {
        echo "Missing Limine file: $file" >&2
        echo "Set LIMINE_DIR to the directory containing the Limine binaries and utility." >&2
        exit 1
    }
done

bash scripts/build-kernel.sh

rm -rf "$ISO_ROOT"
mkdir -p "$ISO_ROOT/boot" "$ISO_ROOT/EFI/BOOT"
cp "$KERNEL" "$ISO_ROOT/boot/chatos.elf"
cp limine.conf "$ISO_ROOT/limine.conf"
cp "$LIMINE_DIR/limine-bios-cd.bin" "$ISO_ROOT/"
cp "$LIMINE_DIR/limine-bios.sys" "$ISO_ROOT/"
cp "$LIMINE_DIR/limine-uefi-cd.bin" "$ISO_ROOT/"
cp "$LIMINE_DIR/BOOTX64.EFI" "$ISO_ROOT/EFI/BOOT/BOOTX64.EFI"

xorriso -as mkisofs \
    -R -r -J \
    -b limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    "$ISO_ROOT" -o ChatOS.iso

"$LIMINE_DIR/limine" bios-install ChatOS.iso

echo "Built ChatOS.iso"
