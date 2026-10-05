#!/usr/bin/env python3

# Count lines of kernel source by module.
# Runs cloc on each module and reprints its totals.
# Usage: linecount.py [module ...]
# With no arguments, counts: arch driver include kernel lib

import json
import os
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODULES = ("arch", "driver", "include", "kernel", "lib")

def panic(msg):
    sys.stderr.write("panic: %s\n" % msg)
    sys.exit(1)

def module_dir(name):
    path = name if os.path.isabs(name) else os.path.join(ROOT, name)
    if not os.path.isdir(path):
        panic("%s: no such directory" % name)
    return path

def cloc_sum(path):
    proc = subprocess.run(
        ["cloc", "--json", "--quiet", "--include-ext=c,h,S", path],
        capture_output=True, text=True)
    if proc.returncode != 0:
        err = proc.stderr.strip() or proc.stdout.strip() or "cloc failed"
        panic(err)
    try:
        data = json.loads(proc.stdout or "{}")
    except json.JSONDecodeError:
        panic("cloc returned no counts")
    total = data.get("SUM") or {}
    return (
        int(total.get("nFiles", 0)),
        int(total.get("blank", 0)),
        int(total.get("comment", 0)),
        int(total.get("code", 0)),
    )

def print_table(rows):
    # Same column layout as cloc's text report.
    head = "{:<19} {:>14} {:>14} {:>14} {:>14}"
    row = "{:<27} {:>6} {:>14} {:>14} {:>14}"
    bar = "-" * 79

    total = [0, 0, 0, 0]
    print(bar)
    print(head.format("Module", "files", "blank", "comment", "code"))
    print(bar)
    for name, files, blank, comment, code in rows:
        print(row.format(name, files, blank, comment, code))
        total[0] += files
        total[1] += blank
        total[2] += comment
        total[3] += code
    print(bar)
    print(head.format("SUM:", *total))
    print(bar)

def main(argv):
    if shutil.which("cloc") is None:
        panic("cloc is not installed")
    names = argv[1:] or list(MODULES)
    rows = []
    for name in names:
        path = module_dir(name)
        label = os.path.basename(path.rstrip(os.sep))
        rows.append((label,) + cloc_sum(path))
    print_table(rows)

if __name__ == "__main__":
    main(sys.argv)
