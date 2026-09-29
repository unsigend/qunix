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

#include <kernel/fmt.h>
#include <kernel/sprintf.h>

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