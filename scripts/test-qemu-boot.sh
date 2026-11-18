#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

QEMU="${QEMU:-qemu-system-x86_64}"
TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-20}"
LOG_FILE="${QEMU_LOG_FILE:-build/qemu-serial.log}"

command -v "$QEMU" >/dev/null 2>&1 || {
    echo "Missing QEMU executable: $QEMU" >&2
    exit 1
}
test -f ChatOS.iso || {
    echo "ChatOS.iso not found; build the ISO first." >&2
    exit 1
}

mkdir -p "$(dirname "$LOG_FILE")"
rm -f "$LOG_FILE"

set +e
timeout --signal=TERM "$TIMEOUT_SECONDS" "$QEMU" \
    -machine q35 \
    -cpu qemu64 \
    -m 512M \
    -display none \
    -monitor none \
    -serial "file:$LOG_FILE" \
    -no-reboot \
    -no-shutdown \
    -cdrom ChatOS.iso
qemu_status=$?
set -e

cat "$LOG_FILE"

# QEMU is expected to remain halted after the kernel reports successful
# bootstrap; timeout (124) is therefore a normal test termination.
if [[ "$qemu_status" -ne 124 && "$qemu_status" -ne 0 ]]; then
    echo "QEMU exited unexpectedly with status $qemu_status" >&2
    exit "$qemu_status"
fi

grep -Fq "Kernel bootstrap complete." "$LOG_FILE" || {
    echo "Kernel did not reach the bootstrap-complete marker." >&2
    exit 1
}

echo "QEMU boot smoke test passed."
