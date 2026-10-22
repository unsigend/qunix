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

#ifndef _QUNIX_ASSERT_H_
#define _QUNIX_ASSERT_H_

#include <kernel/config.h>
#include <kernel/panic.h>

#if CONFIG_DEBUG
#define assert(expr)                                                           \
    do {                                                                       \
        if (!(expr)) {                                                         \
            panic("ASSERT %s:%d: %s failed", __FILE__, __LINE__, #expr);       \
        }                                                                      \
    } while (0)

#define BUG_ON(expr)                                                           \
    do {                                                                       \
        if (expr) {                                                            \
            panic("BUG %s:%d: %s", __FILE__, __LINE__, #expr);                 \
        }                                                                      \
    } while (0)
#else
#define assert(expr) ((void)0)
#define BUG_ON(expr) ((void)0)
#endif /* CONFIG_DEBUG */

#endif /* _QUNIX_ASSERT_H_ */