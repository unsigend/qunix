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

#ifndef _QUNIX_STDDEF_H_
#define _QUNIX_STDDEF_H_

#define NULL ((void *)0)

#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

typedef struct {
    long long __ll __attribute__((aligned(__alignof__(long long))));
    long double __ld __attribute__((aligned(__alignof__(long double))));
} max_align_t;

#endif /* _QUNIX_STDDEF_H_ */