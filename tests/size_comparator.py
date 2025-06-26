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


def print_table(app1: str, app2: str, a: Dict[str, str], b: Dict[str, str]) -> None:
    keys = [k for k in a.keys() if k != "filename"]  # skip last column
    # Width calculation per column (header ≤ values ≤ 12 digits is enough)
    widths = {k: max(len(k), len(a[k]), len(b[k])) for k in keys}

    # Header line
    header_line = "                      " + "  ".join(k.ljust(widths[k]) for k in keys)
    print(header_line)
    # Row for app1
    print(app1.ljust(20) + "  " + "  ".join(a[k].rjust(widths[k]) for k in keys))
    # Row for app2
    print(app2.ljust(20) + "  " + "  ".join(b[k].rjust(widths[k]) for k in keys))

    # Difference in decimal column (dec = text+data+bss)
    if "dec" in a and "dec" in b:
        diff = int(a["dec"]) - int(b["dec"])
        sign = "+" if diff >= 0 else ""
        print("Δ(dec)".ljust(20) + f"  {sign}{diff}")


def main() -> None:
    if len(sys.argv) != 3:
        sys.exit("Usage: compare_size.py <app1> <app2>")

    app1, app2 = sys.argv[1:3]
    a = run_size(app1)
    b = run_size(app2)
    print_table(Path(app1).name, Path(app2).name, a, b)


if __name__ == "__main__":
    main()

