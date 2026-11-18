#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

command -v qemu-system-x86_64 >/dev/null 2>&1 || {
    echo "Missing qemu-system-x86_64. Install QEMU system emulation first." >&2
    exit 1
}

test -f ChatOS.iso || {
    echo "ChatOS.iso not found. Run scripts/build-iso.sh first." >&2
    exit 1
}

QEMU_ARGS=(
    -machine q35
    -cpu qemu64
    -m 512M
    -serial stdio
    -no-reboot
    -no-shutdown
    -cdrom ChatOS.iso
)

if [[ -n "${OVMF_CODE:-}" ]]; then
    QEMU_ARGS=(-bios "$OVMF_CODE" "${QEMU_ARGS[@]}")
fi

exec qemu-system-x86_64 "${QEMU_ARGS[@]}"
