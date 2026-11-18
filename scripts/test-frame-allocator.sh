#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

CC="${HOST_CC:-cc}"
command -v "$CC" >/dev/null 2>&1 || {
    echo "Missing host C compiler: $CC" >&2
    exit 1
}

mkdir -p build
"$CC" -std=c11 -Wall -Wextra -Werror \
    -Isrc/boot -Isrc/memory \
    tests/frame_allocator_test.c src/memory/frame_allocator.c \
    -o build/frame_allocator_test

build/frame_allocator_test
