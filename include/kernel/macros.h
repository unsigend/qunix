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

#ifndef _QUNIX_MACROS_H_
#define _QUNIX_MACROS_H_

#include <kernel/compiler.h>

#define ALIGN(n, align)                                                        \
    (((n) + ((__typeof__(n))(align) - 1)) & ~((__typeof__(n))(align) - 1))
#define IS_ALIGNED(n, align) (((n) & ((align) - 1)) == 0)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ROUND_UP(n, ceil) ALIGN(n, ceil)
#define ROUND_DOWN(n, a) ((n) & ~((__typeof__(n))(a) - 1))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr) - __offsetof(type, member)))

#endif /* _QUNIX_MACROS_H_ */