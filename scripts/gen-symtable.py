#!/usr/bin/env python3

# Read `nm -n` output from stdin and write ksymtable.c to stdout.
# Keeps text symbols (T/t) only, sorted by address.

import sys

HEADER = """\
/* Auto generated, don't modify */

#include <kernel/symtable.h>

const struct ksym ksymtable[] = {
"""

FOOTER = """\
};

size_t nksyms = sizeof(ksymtable) / sizeof(ksymtable[0]);
"""

EMPTY = """\
/* Auto generated, don't modify */

#include <kernel/symtable.h>

const struct ksym ksymtable[] = {
    { 0, 0, 0 },
};

size_t nksyms = 0;
"""


def c_string(s):
    out = []
    for ch in s:
        o = ord(ch)
        if ch == "\\":
            out.append("\\\\")
        elif ch == '"':
            out.append('\\"')
        elif 32 <= o < 127:
            out.append(ch)
        else:
            out.append("\\x%02x" % o)
    return "".join(out)


def parse_nm(lines):
    syms = []
    for line in lines:
        line = line.strip()
        if not line:
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        addr, kind, name = parts[0], parts[1], parts[2]
        if kind not in ("T", "t"):
            continue
        if name.startswith(".L") or name.startswith("$"):
            continue
        try:
            val = int(addr, 16)
        except ValueError:
            continue
        syms.append((val, name, kind))
    syms.sort(key=lambda s: s[0])
    return syms


def main():
    syms = parse_nm(sys.stdin)
    if not syms:
        sys.stdout.write(EMPTY)
        return
    sys.stdout.write(HEADER)
    for addr, name, kind in syms:
        sys.stdout.write('    { 0x%xUL, "%s", \'%s\' },\n' %
                         (addr, c_string(name), kind))
    sys.stdout.write(FOOTER)


if __name__ == "__main__":
    main()
