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
#include <kernel/stdbool.h>
#include <kernel/types.h>

#define ATOMIC_INIT(i) {(i)}
#if WORD_SIZE == 8
#define ATOMIC64_INIT(i) {(i)}
#endif /* WORD_SIZE == 8 */

static __always_inline int atomic_load(atomic_t *v);
static __always_inline void atomic_store(atomic_t *v, int i);
static __always_inline void atomic_add(atomic_t *v, int i);
static __always_inline void atomic_sub(atomic_t *v, int i);
static __always_inline void atomic_inc(atomic_t *v);
static __always_inline void atomic_dec(atomic_t *v);
static __always_inline int atomic_fetch_add(atomic_t *v, int i);
static __always_inline int atomic_fetch_sub(atomic_t *v, int i);
static __always_inline int atomic_fetch_inc(atomic_t *v);
static __always_inline int atomic_fetch_dec(atomic_t *v);
static __always_inline int atomic_xchg(atomic_t *v, int i);
static __always_inline int atomic_cmpxchg(atomic_t *v, int expect, int new);

#if WORD_SIZE == 8
static __always_inline long long atomic64_load(atomic64_t *v);
static __always_inline void atomic64_store(atomic64_t *v, long long i);
static __always_inline void atomic64_add(atomic64_t *v, long long i);
static __always_inline void atomic64_sub(atomic64_t *v, long long i);
static __always_inline void atomic64_inc(atomic64_t *v);
static __always_inline void atomic64_dec(atomic64_t *v);
static __always_inline long long atomic64_fetch_add(atomic64_t *v, long long i);
static __always_inline long long atomic64_fetch_sub(atomic64_t *v, long long i);
static __always_inline long long atomic64_fetch_inc(atomic64_t *v);
static __always_inline long long atomic64_fetch_dec(atomic64_t *v);
static __always_inline long long atomic64_xchg(atomic64_t *v, long long i);
static __always_inline long long
atomic64_cmpxchg(atomic64_t *v, long long expect, long long new);

#endif /* WORD_SIZE == 8 */

static __always_inline int atomic_load(atomic_t *v)
{
    return __atomic_load_n(&v->value, __ATOMIC_RELAXED);
}
static __always_inline void atomic_store(atomic_t *v, int i)
{
    __atomic_store_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline int atomic_fetch_add(atomic_t *v, int i)
{
    return __atomic_fetch_add(&v->value, i, __ATOMIC_RELAXED);
}
static __always_inline int atomic_fetch_sub(atomic_t *v, int i)
{
    return __atomic_fetch_sub(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline void atomic_add(atomic_t *v, int i)
{
    atomic_fetch_add(v, i);
}
static __always_inline void atomic_sub(atomic_t *v, int i)
{
    atomic_fetch_sub(v, i);
}

static __always_inline int atomic_fetch_inc(atomic_t *v)
{
    return atomic_fetch_add(v, 1);
}
static __always_inline int atomic_fetch_dec(atomic_t *v)
{
    return atomic_fetch_sub(v, 1);
}

static __always_inline void atomic_inc(atomic_t *v) { atomic_add(v, 1); }
static __always_inline void atomic_dec(atomic_t *v) { atomic_sub(v, 1); }

static __always_inline int atomic_xchg(atomic_t *v, int i)
{
    return __atomic_exchange_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline int atomic_cmpxchg(atomic_t *v, int expect, int new)
{
    atomic_t old = ATOMIC_INIT(expect);
    __atomic_compare_exchange_n(&v->value, (int *)&old.value, new, false,
                                __ATOMIC_RELAXED, __ATOMIC_RELAXED);
    return old.value;
}

#if WORD_SIZE == 8
static __always_inline long long atomic64_load(atomic64_t *v)
{
    return __atomic_load_n(&v->value, __ATOMIC_RELAXED);
}
static __always_inline void atomic64_store(atomic64_t *v, long long i)
{
    __atomic_store_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline long long atomic64_fetch_add(atomic64_t *v, long long i)
{
    return __atomic_fetch_add(&v->value, i, __ATOMIC_RELAXED);
}
static __always_inline long long atomic64_fetch_sub(atomic64_t *v, long long i)
{
    return __atomic_fetch_sub(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline void atomic64_add(atomic64_t *v, long long i)
{
    atomic64_fetch_add(v, i);
}
static __always_inline void atomic64_sub(atomic64_t *v, long long i)
{
    atomic64_fetch_sub(v, i);
}

static __always_inline long long atomic64_fetch_inc(atomic64_t *v)
{
    return atomic64_fetch_add(v, 1);
}
static __always_inline long long atomic64_fetch_dec(atomic64_t *v)
{
    return atomic64_fetch_sub(v, 1);
}

static __always_inline void atomic64_inc(atomic64_t *v) { atomic64_add(v, 1); }
static __always_inline void atomic64_dec(atomic64_t *v) { atomic64_sub(v, 1); }

static __always_inline long long atomic64_xchg(atomic64_t *v, long long i)
{
    return __atomic_exchange_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline long long
atomic64_cmpxchg(atomic64_t *v, long long expect, long long new)
{
    atomic64_t old = ATOMIC64_INIT(expect);
    __atomic_compare_exchange_n(&v->value, (long long *)&old.value, new, false,
                                __ATOMIC_RELAXED, __ATOMIC_RELAXED);
    return old.value;
}

#endif /* WORD_SIZE == 8 */

#endif /* _QUNIX_ATOMIC_H_ */