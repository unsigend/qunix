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

#ifndef _QUNIX_UTILS_H_
#define _QUNIX_UTILS_H_

#include <kernel/stddef.h>

extern void swap(void *lhs, void *rhs, size_t sz);
extern int sort(void *base, size_t n, size_t sz, int (*cmp)(void *, void *));
extern void *bsearch(const void *key, const void *ptr, size_t n, size_t size,
                     int (*cmp)(const void *, const void *));

#endif /* _QUNIX_UTILS_H_ */