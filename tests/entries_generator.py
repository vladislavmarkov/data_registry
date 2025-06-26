#!/usr/bin/env python3
"""Generate .inl snippet files for C++ registration & tests.

Usage
-----
$ python entries_generator.py 32            # writes files to current dir
$ python entries_generator.py 64 -o build/  # writes files into ./build

This creates three files per run:
    generated_<N>_entries.inl  –   reg_e(tagX, uint64_t);
    generated_<N>_store.inl    –   reg_store_e(tagX, dist(rd));
    generated_<N>_check.inl    –   CHECK(reg::get<tagX>() > uint64_t{0});

Each file contains N lines, where X runs from 1 to N.
"""
from __future__ import annotations

import argparse
from pathlib import Path


def generate_files(n: int, out_dir: str | Path = ".") -> None:
    """Create the *_entries.inl, *_store.inl, and *_check.inl files.

    Parameters
    ----------
    n : int
        Number of tag lines to generate (tag1 … tagN).
    out_dir : str | Path, default="."
        Directory where the files will be written. Created if absent.
    """
    out_path = Path(out_dir)
    out_path.mkdir(parents=True, exist_ok=True)

    entries_file = out_path / f"generated_{n}_entries.inl"
    store_file = out_path / f"generated_{n}_store.inl"
    check_file = out_path / f"generated_{n}_check.inl"

    with entries_file.open("w", encoding="utf-8") as f_entries,\
         store_file.open("w", encoding="utf-8") as f_store,\
         check_file.open("w", encoding="utf-8") as f_check:
        for i in range(1, n + 1):
            f_entries.write(f"reg_e(tag{i}, uint64_t);\n")
            f_store.write(f"reg_store_e(tag{i}, dist(rd));\n")
            f_check.write(f"CHECK(reg::get<tag{i}>() > uint64_t{{0}});\n")

    print(f"✓ Generated:\n • {entries_file}\n • {store_file}\n • {check_file}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate .inl registration and test snippets.")
    parser.add_argument("N", type=int, help="Number of tag entries to emit (positive integer)")
    parser.add_argument(
        "-o",
        "--out-dir",
        default=".",
        help="Destination directory for generated files (default: current directory)",
    )

    args = parser.parse_args()
    if args.N < 1:
        parser.error("N must be a positive integer ≥ 1")

    generate_files(args.N, args.out_dir)

