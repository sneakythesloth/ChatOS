#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

CC="${CC:-clang}"
LD="${LD:-ld.lld}"
BUILD_DIR="build"
OBJECT_DIR="$BUILD_DIR/obj"

command -v "$CC" >/dev/null 2>&1 || { echo "Missing C compiler: $CC" >&2; exit 1; }
command -v "$LD" >/dev/null 2>&1 || { echo "Missing linker: $LD" >&2; exit 1; }

mkdir -p "$OBJECT_DIR"

CFLAGS=(
    --target=x86_64-unknown-none-elf
    -std=c11
    -ffreestanding
    -fno-builtin
    -fno-stack-protector
    -fno-pic
    -fno-pie
    -m64
    -mcmodel=kernel
    -mno-red-zone
    -mgeneral-regs-only
    -Wall
    -Wextra
    -Werror
    -O2
    -Isrc/include
    -Isrc/arch/x86_64
)

"$CC" "${CFLAGS[@]}" -c src/kernel.c -o "$OBJECT_DIR/kernel.o"
"$CC" "${CFLAGS[@]}" -c src/arch/x86_64/serial.c -o "$OBJECT_DIR/serial.o"
"$CC" "${CFLAGS[@]}" -c src/panic.c -o "$OBJECT_DIR/panic.o"

"$LD" -nostdlib -static -z max-page-size=0x1000 \
    -T linker.ld \
    -o "$BUILD_DIR/chatos.elf" \
    "$OBJECT_DIR/kernel.o" "$OBJECT_DIR/serial.o" "$OBJECT_DIR/panic.o"

echo "Built $BUILD_DIR/chatos.elf"
