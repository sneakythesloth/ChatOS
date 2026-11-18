#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ELF="${1:-$ROOT_DIR/build/chatos.elf}"
command -v readelf >/dev/null 2>&1 || {
    echo "Missing readelf (install binutils)." >&2
    exit 1
}
test -f "$ELF" || {
    echo "Kernel ELF not found: $ELF" >&2
    exit 1
}

headers="$(readelf -lW "$ELF")"
printf '%s\n' "$headers"

load_count=0
while read -r virtual physical flags; do
    [[ -n "$virtual" ]] || continue
    load_count=$((load_count + 1))

    case "$virtual" in
        0xffffffff*) ;;
        *)
            echo "PT_LOAD has a non-higher-half virtual address: $virtual" >&2
            exit 1
            ;;
    esac

    case "$physical" in
        0xffffffff*|0xffffffffffff*)
            echo "PT_LOAD has an invalid high physical load address: $physical" >&2
            exit 1
            ;;
    esac

    physical_value=$((physical))
    if (( physical_value < 0x00200000 || physical_value >= 0x04200000 )); then
        echo "PT_LOAD physical address is outside the reserved 2 MiB..66 MiB kernel window: $physical" >&2
        exit 1
    fi

    if [[ "$flags" == *W* && "$flags" == *E* ]]; then
        echo "PT_LOAD is both writable and executable (W^X violation)." >&2
        exit 1
    fi
done < <(readelf -lW "$ELF" | awk '$1 == "LOAD" { print $3, $4, $7 $8 }')

if (( load_count < 3 )); then
    echo "Expected at least 3 PT_LOAD segments, found $load_count." >&2
    exit 1
fi

echo "Kernel ELF layout passed: $load_count higher-half PT_LOAD segments use low physical addresses."
