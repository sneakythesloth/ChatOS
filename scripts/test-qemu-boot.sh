#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

QEMU="${QEMU:-qemu-system-x86_64}"
TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-20}"
LOG_FILE="${QEMU_LOG_FILE:-build/qemu-serial.log}"
DEBUG_FILE="${QEMU_DEBUG_FILE:-build/qemu-debug.log}"

command -v "$QEMU" >/dev/null 2>&1 || {
    echo "Missing QEMU executable: $QEMU" >&2
    exit 1
}
test -f ChatOS.iso || {
    echo "ChatOS.iso not found; build the ISO first." >&2
    exit 1
}

mkdir -p "$(dirname "$LOG_FILE")" "$(dirname "$DEBUG_FILE")"
rm -f "$LOG_FILE" "$DEBUG_FILE"

set +e
timeout --signal=TERM --kill-after=3s "$TIMEOUT_SECONDS" "$QEMU" \
    -machine q35 \
    -cpu qemu64 \
    -m 512M \
    -display none \
    -monitor none \
    -serial "file:$LOG_FILE" \
    -debugcon "file:$DEBUG_FILE" \
    -global isa-debugcon.iobase=0xe9 \
    -d guest_errors,int,cpu_reset \
    -D build/qemu-machine.log \
    -boot order=d,menu=on \
    -no-reboot \
    -no-shutdown \
    -cdrom ChatOS.iso
qemu_status=$?
set -e

echo "===== ChatOS serial output ====="
cat "$LOG_FILE" 2>/dev/null || true
echo "===== QEMU debug console ====="
cat "$DEBUG_FILE" 2>/dev/null || true
echo "===== QEMU machine diagnostics (last 160 lines) ====="
tail -n 160 build/qemu-machine.log 2>/dev/null || true

# QEMU is expected to remain halted after the kernel reports successful
# bootstrap; timeout (124) is therefore a normal test termination.
if [[ "$qemu_status" -ne 124 && "$qemu_status" -ne 0 ]]; then
    echo "QEMU exited unexpectedly with status $qemu_status" >&2
    exit "$qemu_status"
fi

grep -Fq "${EXPECTED_SERIAL_MARKER:-Kernel bootstrap complete.}" "$LOG_FILE" || {
    echo "Expected serial marker was not observed: ${EXPECTED_SERIAL_MARKER:-Kernel bootstrap complete.}" >&2
    exit 1
}

if [[ -n "${EXPECTED_PANIC_MARKER:-}" ]]; then
    grep -Fq "$EXPECTED_PANIC_MARKER" "$LOG_FILE" || {
        echo "Expected panic marker was not observed: $EXPECTED_PANIC_MARKER" >&2
        exit 1
    }
fi

echo "QEMU boot smoke test passed."
