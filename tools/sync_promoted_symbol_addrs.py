#!/usr/bin/env python3
"""Rewrite linker symbol names to match promoted C function names.

Aligns src/**/*.c function definitions to linker_scripts/us/symbol_addrs_code.txt
using:
  1. Identity anchors (still-named func_XXXXXXXX)
  2. Main-text VRAM from yamls/us/rom.yaml when a file is fully promoted

Stops a file at the first identity mismatch so missing GLOBAL_ASM bodies
cannot shift the map.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
ADDRS = ROOT / "linker_scripts" / "us" / "symbol_addrs_code.txt"

DEF_RE = re.compile(
    r"^(?:[A-Za-z_][\w\s\*]*?)\s+([A-Za-z_][\w]*)\s*\([^;{]*\)\s*\{",
    re.M,
)
ADDR_RE = re.compile(r"^(\S+)\s*=\s*(0x[0-9A-Fa-f]+);\s*(//.*)?$")
SKIP = {"if", "for", "while", "switch", "return", "else", "do"}


def c_func_defs(path: Path) -> list[str]:
    names = []
    for m in DEF_RE.finditer(path.read_text(errors="replace")):
        name = m.group(1)
        if name not in SKIP:
            names.append(name)
    return names


def parse_text_c_vrams() -> dict[str, int]:
    """stem (lowercase) -> first VRAM of that TU in the main text segment."""
    yaml = (ROOT / "yamls/us/rom.yaml").read_text()
    in_text = False
    start = vram0 = None
    out: dict[str, int] = {}
    for line in yaml.splitlines():
        s = line.strip()
        if s.startswith("- name: text"):
            in_text = True
            continue
        if in_text and s.startswith("- name:"):
            break
        if not in_text:
            continue
        m = re.search(r"start:\s*(0x[0-9A-Fa-f]+)", line)
        if m:
            start = int(m.group(1), 16)
        m = re.search(r"vram:\s*(0x[0-9A-Fa-f]+)", line)
        if m:
            vram0 = int(m.group(1), 16)
        m = re.search(r"-\s*\[(0x[0-9A-Fa-f]+),\s*(c|hasm)(?:,\s*([^,\]]+))?", line)
        if m and start is not None and vram0 is not None:
            rom = int(m.group(1), 16)
            stem = (m.group(3) or f"{rom:X}").strip().lower()
            if "/" in stem:
                stem = stem.split("/")[-1]
            out[stem] = vram0 + (rom - start)
    return out


def load_addrs() -> tuple[list[tuple[str, int, str]], dict[str, int], dict[int, int]]:
    rows: list[tuple[str, int, str]] = []
    by_name: dict[str, int] = {}
    by_va: dict[int, int] = {}
    for line in ADDRS.read_text().splitlines():
        m = ADDR_RE.match(line)
        if not m or "type:func" not in (m.group(3) or ""):
            continue
        name, va_s, comment = m.group(1), m.group(2), m.group(3) or ""
        va = int(va_s, 16)
        by_name[name] = len(rows)
        by_va[va] = len(rows)
        rows.append((name, va, comment))
    return rows, by_name, by_va


def apply_run(defs, start_idx, rows) -> dict[str, tuple[str, str]]:
    mapping: dict[str, tuple[str, str]] = {}
    if start_idx < 0:
        return mapping
    # Skip alias rows that share a VA with the previous func entry
    unique = []
    prev_va = None
    for row in rows:
        if prev_va == row[1]:
            continue
        unique.append(row)
        prev_va = row[1]
    # Remap start_idx from `rows` into `unique` via VA
    start_va = rows[start_idx][1]
    try:
        start = next(i for i, r in enumerate(unique) if r[1] == start_va)
    except StopIteration:
        return mapping
    for i, name in enumerate(defs):
        idx = start + i
        if idx >= len(unique):
            break
        orig, va, _comment = unique[idx]
        if name.startswith("func_"):
            if name != orig:
                break
            continue
        mapping[orig] = (name, f"0x{va:08X}")
    return mapping


def align_file(defs, rows, by_name, by_va, file_vram: int | None) -> dict[str, tuple[str, str]]:
    for i, name in enumerate(defs):
        if name.startswith("func_") and name in by_name:
            return apply_run(defs, by_name[name] - i, rows)
    if file_vram is not None:
        # Exact TU start in KSEG0 code, never boot IPL (0xA4xxxxxx)
        if file_vram in by_va:
            return apply_run(defs, by_va[file_vram], rows)
        start = None
        for i, (_n, va, _c) in enumerate(rows):
            if 0x80000000 <= va < 0x80100000 and va >= file_vram:
                start = i
                break
        if start is not None:
            return apply_run(defs, start, rows)
    for i, name in enumerate(defs):
        if name in by_name:
            return apply_run(defs, by_name[name] - i, rows)
    return {}


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()

    rows, by_name, by_va = load_addrs()
    vrams = parse_text_c_vrams()
    combined: dict[str, tuple[str, str]] = {}
    per_file = []
    skip_bits = ("libnaudio", "libleo", "libnumus", "ultralib", "asm-processor")
    for path in sorted(SRC.rglob("*.c")):
        if any(b in str(path) for b in skip_bits):
            continue
        defs = c_func_defs(path)
        if not defs:
            continue
        stem = path.stem.lower()
        mapping = align_file(defs, rows, by_name, by_va, vrams.get(stem))
        if mapping:
            per_file.append((path.relative_to(ROOT), len(mapping)))
            combined.update(mapping)

    print(f"promoted mappings: {len(combined)}")
    for rel, n in per_file:
        print(f"  {rel}: {n}")

    if not args.write:
        return

    out_lines = []
    replaced = 0
    for line in ADDRS.read_text().splitlines():
        m = ADDR_RE.match(line)
        if m and m.group(1) in combined:
            new_name, va = combined[m.group(1)]
            orig = m.group(1)
            comment = m.group(3) or "// type:func"
            if f"orig:{orig}" not in comment:
                comment = comment.rstrip() + f" orig:{orig}"
            out_lines.append(f"{new_name} = {va}; {comment}")
            replaced += 1
        else:
            out_lines.append(line)
    ADDRS.write_text("\n".join(out_lines) + "\n")
    print(f"rewrote {replaced} symbol_addrs_code.txt entries")


if __name__ == "__main__":
    main()
