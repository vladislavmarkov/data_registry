#!/usr/bin/env python3
"""Compare section sizes of two ELF executables using `size`.

Usage
-----
$ ./compare_size.py ./app1 ./app2

Output (example)
----------------
        text   data   bss   dec   hex
app1    8048   224    0     8272  2050
app2    7520   224    0     7744  1e40
Δ(dec)  +528

Only the numeric columns reported by the classic BSD `size` layout are
compared.  A positive Δ means *app1* is larger.
"""
from __future__ import annotations

import subprocess
import sys
from pathlib import Path
from typing import Dict, List
from tabulate import tabulate


def run_size(path: str | Path) -> Dict[str, str]:
    """Return a mapping {column → value} from the `size` output."""
    try:
        out = subprocess.check_output(["size", str(path)], text=True)
    except (OSError, subprocess.CalledProcessError) as exc:
        sys.exit(f"error: cannot execute `size` on {path!s}: {exc}")

    lines = out.strip().splitlines()
    if len(lines) < 2:
        sys.exit(f"error: unexpected `size` output for {path!s}")

    header: List[str] = lines[0].split()
    values: List[str] = lines[1].split()
    return dict(zip(header, values))


def main() -> None:
    if len(sys.argv) != 3:
        sys.exit("Usage: size_comparator.py <app1> <app2>")

    app1, app2 = sys.argv[1:3]
    a = run_size(app1)
    b = run_size(app2)
    headers = list(a.keys())
    values_a = list(a.values())
    values_b = list(b.values())
    headers.pop()
    values_a.pop()
    file_b = values_b.pop()
    headers.insert(0, "")
    values_a.insert(0, "Plain C++ statics")
    values_b.insert(0, "data::registry")
    print(headers)
    print(values_a)
    print(values_b)
    rows = [values_a, values_b]
    md = tabulate(rows, headers=headers, tablefmt="github", stralign="right", numalign="right")
    print(md)

if __name__ == "__main__":
    main()

