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

#ifndef _QUNIX_ATOMIC_H_
#define _QUNIX_ATOMIC_H_

#include <kernel/compiler.h>
#include <kernel/config.h>
#include <kernel/types.h>

/* Atomic operations, this is a generic layer of the atomic operations,
 * architecture-independent. The core implementation is based on GNU GCC
 * builtin atomic operations. Atomic only guarantees atomicity, where ordering
 * is guaranteed by the barrier.
 *
 * Reference document:
 * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
 */

#define ATOMIC_INIT(val) {(val)}
#define DEFAULT_ORDER __ATOMIC_RELAXED

static __always_inline int atomic_read(const atomic_t *v);
static __always_inline void atomic_set(atomic_t *v, int i);
static __always_inline void atomic_add(int i, atomic_t *v);
static __always_inline void atomic_sub(int i, atomic_t *v);
static __always_inline void atomic_inc(atomic_t *v);
static __always_inline void atomic_dec(atomic_t *v);
static __always_inline int atomic_add_return(int i, atomic_t *v);
static __always_inline int atomic_sub_return(int i, atomic_t *v);
static __always_inline int atomic_inc_return(atomic_t *v);
static __always_inline int atomic_dec_return(atomic_t *v);

STATIC_ASSERT(__atomic_always_lock_free(sizeof(int), 0));
STATIC_ASSERT(__atomic_always_lock_free(sizeof(long long), 0));

static __always_inline int atomic_read(const atomic_t *v)
{
    return __atomic_load_n(&v->value, DEFAULT_ORDER);
}

static __always_inline void atomic_set(atomic_t *v, int i)
{
    __atomic_store_n(&v->value, i, DEFAULT_ORDER);
}

static __always_inline int atomic_add_return(int i, atomic_t *v)
{
    return __atomic_add_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline int atomic_sub_return(int i, atomic_t *v)
{
    return __atomic_sub_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline int atomic_inc_return(atomic_t *v)
{
    return atomic_add_return(1, v);
}

static __always_inline int atomic_dec_return(atomic_t *v)
{
    return atomic_sub_return(1, v);
}

static __always_inline void atomic_add(int i, atomic_t *v)
{
    atomic_add_return(i, v);
}

static __always_inline void atomic_sub(int i, atomic_t *v)
{
    atomic_sub_return(i, v);
}

static __always_inline void atomic_inc(atomic_t *v) { atomic_add(1, v); }
static __always_inline void atomic_dec(atomic_t *v) { atomic_sub(1, v); }

#endif /* _QUNIX_ATOMIC_H_ */