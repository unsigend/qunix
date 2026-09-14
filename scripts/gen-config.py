#!/usr/bin/env python3

# generate mk/config.mk and include/qunix/config.h from config/config.cfg

import os

CONFIG_IN = os.path.join(os.path.dirname(__file__), "..", "config", "config.cfg")
CONFIG_MK = os.path.join(os.path.dirname(__file__), "..", "mk",     "config.mk")
CONFIG_H  = os.path.join(os.path.dirname(__file__), "..", "include", "qunix", "config.h")

MK_HEADER = """\
# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.

# Do not modify this file directly, edit config/config.cfg instead
"""

H_HEADER = """\
/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

/* do not modify this file directly, edit config/config.cfg instead */

#ifndef QUNIX_CONFIG_H
#define QUNIX_CONFIG_H
"""

H_FOOTER = """\
#endif /* QUNIX_CONFIG_H */
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
