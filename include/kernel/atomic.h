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

/* Architecture-independent atomic operations, based on GNU GCC builtin atomic
 * operations. The atomic operations only guarantee atomicity, where ordering is
 * guaranteed by the barrier. */

#define ATOMIC_INIT(val) {(val)}
#define ATOMIC64_INIT(val) {(val)}
#define DEFAULT_ORDER __ATOMIC_RELAXED

static __always_inline int atomic_read(const atomic_t *v);
static __always_inline void atomic_set(atomic_t *v, int i);
static __always_inline void atomic_add(atomic_t *v, int i);
static __always_inline void atomic_sub(atomic_t *v, int i);
static __always_inline void atomic_inc(atomic_t *v);
static __always_inline void atomic_dec(atomic_t *v);
static __always_inline int atomic_add_return(atomic_t *v, int i);
static __always_inline int atomic_sub_return(atomic_t *v, int i);
static __always_inline int atomic_inc_return(atomic_t *v);
static __always_inline int atomic_dec_return(atomic_t *v);

/* Atomic exchange operations, it store the content of the i and return the old
 * value. */
static __always_inline int atomic_xchg(atomic_t *v, int i);

/* Atomic compare and exchange operations, it compares the content of the atomic
 * with the expect value, if they are equal, it stores the new value and returns
 * true. Otherwise, it returns false. */
static __always_inline bool atomic_cmpxchg(atomic_t *v, int expect, int new);

/* 64-bit atomic operations, do not use these functions if WORD_BITS is 32,
 * which has a poor support for 64-bit atomic operations. */
#if WORD_BITS == 64
static __always_inline long long atomic64_read(const atomic64_t *v);
static __always_inline void atomic64_set(atomic64_t *v, long long i);
static __always_inline void atomic64_add(atomic64_t *v, long long i);
static __always_inline void atomic64_sub(atomic64_t *v, long long i);
static __always_inline void atomic64_inc(atomic64_t *v);
static __always_inline void atomic64_dec(atomic64_t *v);
static __always_inline long long atomic64_add_return(atomic64_t *v,
                                                     long long i);
static __always_inline long long atomic64_sub_return(atomic64_t *v,
                                                     long long i);
static __always_inline long long atomic64_inc_return(atomic64_t *v);
static __always_inline long long atomic64_dec_return(atomic64_t *v);
static __always_inline long long atomic64_xchg(atomic64_t *v, long long i);
static __always_inline bool atomic64_cmpxchg(atomic64_t *v, long long expect,
                                             long long new);
#endif /* WORD_BITS == 64 */

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

static __always_inline int atomic_add_return(atomic_t *v, int i)
{
    return __atomic_add_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline int atomic_sub_return(atomic_t *v, int i)
{
    return __atomic_sub_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline int atomic_inc_return(atomic_t *v)
{
    return atomic_add_return(v, 1);
}

static __always_inline int atomic_dec_return(atomic_t *v)
{
    return atomic_sub_return(v, 1);
}

static __always_inline void atomic_add(atomic_t *v, int i)
{
    atomic_add_return(v, i);
}

static __always_inline void atomic_sub(atomic_t *v, int i)
{
    atomic_sub_return(v, i);
}

static __always_inline void atomic_inc(atomic_t *v) { atomic_add(v, 1); }
static __always_inline void atomic_dec(atomic_t *v) { atomic_sub(v, 1); }

static __always_inline int atomic_xchg(atomic_t *v, int i)
{
    return __atomic_exchange_n(&v->value, i, DEFAULT_ORDER);
}

static __always_inline bool atomic_cmpxchg(atomic_t *v, int expect, int new)
{
    return __atomic_compare_exchange_n(&v->value, &expect, new, 0,
                                       DEFAULT_ORDER, DEFAULT_ORDER);
}

#if WORD_BITS == 64
static __always_inline long long atomic64_read(const atomic64_t *v)
{
    return __atomic_load_n(&v->value, DEFAULT_ORDER);
}

static __always_inline void atomic64_set(atomic64_t *v, long long i)
{
    __atomic_store_n(&v->value, i, DEFAULT_ORDER);
}

static __always_inline long long atomic64_add_return(atomic64_t *v, long long i)
{
    return __atomic_add_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline long long atomic64_sub_return(atomic64_t *v, long long i)
{
    return __atomic_sub_fetch(&v->value, i, DEFAULT_ORDER);
}

static __always_inline long long atomic64_inc_return(atomic64_t *v)
{
    return atomic64_add_return(v, 1);
}

static __always_inline long long atomic64_dec_return(atomic64_t *v)
{
    return atomic64_sub_return(v, 1);
}

static __always_inline void atomic64_add(atomic64_t *v, long long i)
{
    atomic64_add_return(v, i);
}

static __always_inline void atomic64_sub(atomic64_t *v, long long i)
{
    atomic64_sub_return(v, i);
}

static __always_inline void atomic64_inc(atomic64_t *v) { atomic64_add(v, 1); }
static __always_inline void atomic64_dec(atomic64_t *v) { atomic64_sub(v, 1); }

static __always_inline long long atomic64_xchg(atomic64_t *v, long long i)
{
    return __atomic_exchange_n(&v->value, i, DEFAULT_ORDER);
}

static __always_inline bool atomic64_cmpxchg(atomic64_t *v, long long expect,
                                             long long new)
{
    return __atomic_compare_exchange_n(&v->value, &expect, new, 0,
                                       DEFAULT_ORDER, DEFAULT_ORDER);
}
#endif /* WORD_BITS == 64 */

#endif /* _QUNIX_ATOMIC_H_ */