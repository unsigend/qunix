#!/usr/bin/env python3

# generate mk/config.mk and include/qunix/config.h from config/config.cfg

import os

CONFIG_IN = os.path.join(os.path.dirname(__file__), "..", "config", "config.cfg")
CONFIG_MK = os.path.join(os.path.dirname(__file__), "..", "mk",     "config.mk")
CONFIG_H  = os.path.join(os.path.dirname(__file__), "..", "include", "kernel", "config.h")

MK_HEADER = """\
# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

H_HEADER = """\
/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* Auto generated, don't modify */

#ifndef _QUNIX_CONFIG_H_
#define _QUNIX_CONFIG_H_
"""

H_FOOTER = """\
#endif /* _QUNIX_CONFIG_H_ */
"""


def parse_config(path):
    entries = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            key, _, val = line.partition("=")
            entries.append((key.strip(), val.strip()))
    return entries


def write_mk(entries):
    os.makedirs(os.path.dirname(CONFIG_MK), exist_ok=True)
    with open(CONFIG_MK, "w") as f:
        f.write(MK_HEADER + "\n")
        for key, val in entries:
            bare = key.removeprefix("CONFIG_")
            f.write(f"{bare} = {val}\n")


def h_define(key, val):
    """
    Return the appropriate #define line for a config entry.

    Numeric values  ->  #define CONFIG_KEY val
    String values   ->  #define CONFIG_KEY_VAL 1
    """
    try:
        int(val, 0)
        return f"#define {key} {val}"
    except ValueError:
        return f"#define {key}_{val.upper()} 1"


def write_h(entries):
    """Only emit entries whose key starts with CONFIG_ — the name is the signal."""
    os.makedirs(os.path.dirname(CONFIG_H), exist_ok=True)
    with open(CONFIG_H, "w") as f:
        f.write(H_HEADER + "\n")
        for key, val in entries:
            if key.startswith("CONFIG_"):
                f.write(h_define(key, val) + "\n")
        f.write("\n" + H_FOOTER)


def main():
    entries = parse_config(CONFIG_IN)
    write_mk(entries)
    write_h(entries)


if __name__ == "__main__":
    main()
