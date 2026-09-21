/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <ext/printf.h>
#include <qunix/fmt.h>

int fmt_mem(char *buf, size_t bufsz, uint64_t bytes)
{
    static const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
    for (size_t i = 0; i < sizeof(units) / sizeof(units[0]); i++) {
        if (bytes < 1024)
            return snprintf(buf, bufsz, "%llu %s", bytes, units[i]);
        bytes /= 1024;
    }

    return 0;
}