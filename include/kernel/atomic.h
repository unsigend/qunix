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

static __always_inline int atomic_read(atomic_t *v);
static __always_inline void atomic_set(atomic_t *v, int i);
static __always_inline void atomic_add(atomic_t *v, int i);
static __always_inline void atomic_sub(atomic_t *v, int i);
static __always_inline void atomic_inc(atomic_t *v);
static __always_inline void atomic_dec(atomic_t *v);

/* Atomic arithmetic operations, return the new value */
static __always_inline int atomic_add_return(atomic_t *v, int i);
static __always_inline int atomic_sub_return(atomic_t *v, int i);
static __always_inline int atomic_inc_return(atomic_t *v);
static __always_inline int atomic_dec_return(atomic_t *v);

/* Atomic exchange operation, return the old value */
static __always_inline int atomic_xchg(atomic_t *v, int i);

/* Atomic compare and exchange operation, if the expect equals the value in v,
 * write the new value to v and return the old value. If not equal no-ops
 * performed, return the old value in v. */
static __always_inline int atomic_cmpxchg(atomic_t *v, int expect, int new);

#if WORD_SIZE == 8
static __always_inline long long atomic64_read(atomic64_t *v);
static __always_inline void atomic64_set(atomic64_t *v, long long i);
static __always_inline void atomic64_add(atomic64_t *v, long long i);
static __always_inline void atomic64_sub(atomic64_t *v, long long i);
static __always_inline void atomic64_inc(atomic64_t *v);
static __always_inline void atomic64_dec(atomic64_t *v);

/* 64-bit atomic arithmetic operations, return the new value */
static __always_inline long long atomic64_add_return(atomic64_t *v,
                                                     long long i);
static __always_inline long long atomic64_sub_return(atomic64_t *v,
                                                     long long i);
static __always_inline long long atomic64_inc_return(atomic64_t *v);
static __always_inline long long atomic64_dec_return(atomic64_t *v);

/* 64-bit atomic exchange operation, return the old value */
static __always_inline long long atomic64_xchg(atomic64_t *v, long long i);

/* 64-bit atomic compare and exchange operation, if the expect equals the value
 * in v, write the new value to v and return the old value. If not equal no-ops
 * performed, return the old value in v. */
static __always_inline long long
atomic64_cmpxchg(atomic64_t *v, long long expect, long long new);

#endif /* WORD_SIZE == 8 */

static __always_inline int atomic_read(atomic_t *v)
{
    return __atomic_load_n(&v->value, __ATOMIC_RELAXED);
}
static __always_inline void atomic_set(atomic_t *v, int i)
{
    __atomic_store_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline int atomic_add_return(atomic_t *v, int i)
{
    return __atomic_add_fetch(&v->value, i, __ATOMIC_RELAXED);
}
static __always_inline int atomic_sub_return(atomic_t *v, int i)
{
    return __atomic_sub_fetch(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline void atomic_add(atomic_t *v, int i)
{
    atomic_add_return(v, i);
}
static __always_inline void atomic_sub(atomic_t *v, int i)
{
    atomic_sub_return(v, i);
}

static __always_inline int atomic_inc_return(atomic_t *v)
{
    return atomic_add_return(v, 1);
}
static __always_inline int atomic_dec_return(atomic_t *v)
{
    return atomic_sub_return(v, 1);
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
static __always_inline long long atomic64_read(atomic64_t *v)
{
    return __atomic_load_n(&v->value, __ATOMIC_RELAXED);
}
static __always_inline void atomic64_set(atomic64_t *v, long long i)
{
    __atomic_store_n(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline long long atomic64_add_return(atomic64_t *v, long long i)
{
    return __atomic_add_fetch(&v->value, i, __ATOMIC_RELAXED);
}
static __always_inline long long atomic64_sub_return(atomic64_t *v, long long i)
{
    return __atomic_sub_fetch(&v->value, i, __ATOMIC_RELAXED);
}

static __always_inline void atomic64_add(atomic64_t *v, long long i)
{
    atomic64_add_return(v, i);
}
static __always_inline void atomic64_sub(atomic64_t *v, long long i)
{
    atomic64_sub_return(v, i);
}

static __always_inline long long atomic64_inc_return(atomic64_t *v)
{
    return atomic64_add_return(v, 1);
}
static __always_inline long long atomic64_dec_return(atomic64_t *v)
{
    return atomic64_sub_return(v, 1);
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