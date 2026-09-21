/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_FMT_H
#define QUNIX_FMT_H

#include <stddef.h>
#include <stdint.h>

/* Format memory size in bytes to a human-readable string, only keep the first
 * unit and drop the rest. Return the number of characters written. */
extern int fmt_mem(char *buf, size_t bufsz, uint64_t bytes);

#endif