#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

LIMINE_DIR="${LIMINE_DIR:-.}"
KERNEL="target/x86_64-unknown-none/release/chatos-kernel"
ISO_ROOT="iso_root"

for tool in cargo xorriso; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "Missing required tool: $tool" >&2
        exit 1
    }
done

for file in "$LIMINE_DIR/limine-bios-cd.bin" "$LIMINE_DIR/limine-uefi-cd.bin" "$LIMINE_DIR/limine"; do
    test -f "$file" || {
        echo "Missing Limine file: $file" >&2
        echo "Set LIMINE_DIR to the directory containing the Limine binaries and utility." >&2
        exit 1
    }
done

cargo build --release

rm -rf "$ISO_ROOT"
mkdir -p "$ISO_ROOT/boot"
cp "$KERNEL" "$ISO_ROOT/boot/chatos.elf"
cp limine.conf "$ISO_ROOT/limine.conf"
cp "$LIMINE_DIR/limine-bios-cd.bin" "$ISO_ROOT/"
cp "$LIMINE_DIR/limine-uefi-cd.bin" "$ISO_ROOT/"

xorriso -as mkisofs \
    -b limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    "$ISO_ROOT" -o ChatOS.iso

"$LIMINE_DIR/limine" bios-install ChatOS.iso

echo "Built ChatOS.iso"
