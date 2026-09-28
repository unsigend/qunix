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

#ifndef _QUNIX_PRINTK_H_
#define _QUNIX_PRINTK_H_

#include <kernel/compiler.h>

#define LOG_LEVEL_DEBUG "[DEBUG  ]"
#define LOG_LEVEL_INFO "[INFO   ]"
#define LOG_LEVEL_WARN "[WARNING]"
#define LOG_LEVEL_FATAL "[FATAL  ]"

#define LOG(level, fmt, ...) printk(level " " fmt "\n", ##__VA_ARGS__)
#define LOGM(level, m, fmt, ...)                                               \
    printk(level "[%-8s] " fmt "\n", m, ##__VA_ARGS__)

extern __printf(1, 2) int printk(const char *fmt, ...);

#endif /* _QUNIX_PRINTK_H_ */