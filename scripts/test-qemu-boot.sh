#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

QEMU="${QEMU:-qemu-system-x86_64}"
TIMEOUT_SECONDS="${TIMEOUT_SECONDS:-20}"
LOG_FILE="${QEMU_LOG_FILE:-build/qemu-serial.log}"
DEBUG_FILE="${QEMU_DEBUG_FILE:-build/qemu-debug.log}"

QEMU_FIRMWARE_ARGS=()
if [[ -n "${OVMF_CODE:-}" ]]; then
    test -f "$OVMF_CODE" || {
        echo "OVMF_CODE does not point to a firmware image: $OVMF_CODE" >&2
        exit 1
    }

    # OVMF is a UEFI flash image, not a legacy PC BIOS image. QEMU needs
    # the read-only code flash and a separate writable variables flash.
    OVMF_VARS_PATH="${OVMF_VARS:-}"
    if [[ -z "$OVMF_VARS_PATH" ]]; then
        case "$OVMF_CODE" in
            *_CODE_4M.fd) OVMF_VARS_PATH="${OVMF_CODE%_CODE_4M.fd}_VARS_4M.fd" ;;
            *_CODE.fd) OVMF_VARS_PATH="${OVMF_CODE%_CODE.fd}_VARS.fd" ;;
            *)
                echo "Set OVMF_VARS to the matching writable variables image." >&2
                exit 1
                ;;
        esac
    fi
    test -f "$OVMF_VARS_PATH" || {
        echo "OVMF_VARS does not point to a firmware variables image: $OVMF_VARS_PATH" >&2
        exit 1
    }

    mkdir -p build
    OVMF_VARS_COPY="$ROOT_DIR/build/OVMF_VARS.fd"
    cp "$OVMF_VARS_PATH" "$OVMF_VARS_COPY"
    QEMU_FIRMWARE_ARGS=(
        -drive "if=pflash,format=raw,unit=0,readonly=on,file=$OVMF_CODE"
        -drive "if=pflash,format=raw,unit=1,file=$OVMF_VARS_COPY"
    )
fi

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
    "${QEMU_FIRMWARE_ARGS[@]}" \
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
