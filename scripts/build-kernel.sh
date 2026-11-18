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
    -Isrc/boot
    -Isrc/memory
    -Isrc/arch/x86_64
)

SOURCES=(
    src/kernel.c
    src/boot/limine.c
    src/memory/memory.c
    src/memory/frame_allocator.c
    src/memory/hhdm.c
    src/memory/vm.c
    src/arch/x86_64/serial.c
    src/arch/x86_64/interrupts.c
    src/panic.c
)

OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$OBJECT_DIR/${source//\//_}.o"
    "$CC" "${CFLAGS[@]}" -c "$source" -o "$object"
    OBJECTS+=("$object")
done

ASM_OBJECT="$OBJECT_DIR/src_arch_x86_64_interrupts.o"
"$CC" --target=x86_64-unknown-none-elf -m64 -c src/arch/x86_64/interrupts.S -o "$ASM_OBJECT"
OBJECTS+=("$ASM_OBJECT")

"$LD" -nostdlib -static -z max-page-size=0x1000 \
    -T linker.ld \
    -o "$BUILD_DIR/chatos.elf" \
    "${OBJECTS[@]}"

echo "Built $BUILD_DIR/chatos.elf"
