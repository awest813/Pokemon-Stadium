#!/usr/bin/env python3
"""
Splat emits one object per [0x20470, c] subsegment. Gfx lives in src/20470_data.c for IDO heap;
this script inserts build/src/20470_data.o next to 20470.o in the linker script (idempotent).
Run after splat during `make extract`.
"""

from __future__ import annotations

import sys
from pathlib import Path

LD_PATH = Path(__file__).resolve().parents[1] / "linker_scripts" / "us" / "pokestadium.ld"

TEXT_BEFORE = "        build/src/20330.o(.text);\n        build/src/20470.o(.text);"
TEXT_AFTER = (
    "        build/src/20330.o(.text);\n"
    "        build/src/20470_data.o(.text);\n"
    "        build/src/20470.o(.text);"
)

DATA_BEFORE = "        _20470 = .;\n        build/src/20470.o(.data);"
DATA_AFTER = (
    "        _20470 = .;\n"
    "        build/src/20470_data.o(.data);\n"
    "        build/src/20470.o(.data);"
)

RODATA_BEFORE = "        build/src/20330.o(.rodata);\n        build/src/20470.o(.rodata);"
RODATA_AFTER = (
    "        build/src/20330.o(.rodata);\n"
    "        build/src/20470_data.o(.rodata);\n"
    "        build/src/20470.o(.rodata);"
)


def main() -> int:
    if not LD_PATH.is_file():
        print(f"patch_ld_20470_split: skip (no {LD_PATH})", file=sys.stderr)
        return 0

    text = LD_PATH.read_text(encoding="utf-8")
    orig = text

    if "20470_data.o" not in text:
        if TEXT_BEFORE not in text:
            print("patch_ld_20470_split: expected .text block not found; linker script changed?", file=sys.stderr)
            return 1
        if DATA_BEFORE not in text:
            print("patch_ld_20470_split: expected .data block not found; linker script changed?", file=sys.stderr)
            return 1
        if RODATA_BEFORE not in text:
            print("patch_ld_20470_split: expected .rodata block not found; linker script changed?", file=sys.stderr)
            return 1
        text = text.replace(TEXT_BEFORE, TEXT_AFTER, 1)
        text = text.replace(DATA_BEFORE, DATA_AFTER, 1)
        text = text.replace(RODATA_BEFORE, RODATA_AFTER, 1)

    if text != orig:
        LD_PATH.write_text(text, encoding="utf-8", newline="\n")
        print(f"patch_ld_20470_split: updated {LD_PATH}")
    else:
        print("patch_ld_20470_split: already applied or no changes needed")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
