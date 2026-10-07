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

#ifndef _QUNIX_BITOPS_H_
#define _QUNIX_BITOPS_H_

#include <kernel/compiler.h>
#include <kernel/feature.h>
#include <kernel/stdbool.h>

/* Bit operations without atomicity guarantee, for the return version of the
 * operation, it returns the previous value of the bit. */
static __always_inline void bit_set(void *addr, int nr);
static __always_inline void bit_clear(void *addr, int nr);
static __always_inline void bit_flip(void *addr, int nr);
static __always_inline bool bit_set_return(void *addr, int nr);
static __always_inline bool bit_clear_return(void *addr, int nr);
static __always_inline bool bit_flip_return(void *addr, int nr);
static __always_inline bool bit_read(void *addr, int nr);

/* Atomic bit operations guarantee atomicity, if you already hold a lock, use
 * non-atomic bit operations instead. */
static __always_inline void atomic_bit_set(void *addr, int nr);
static __always_inline void atomic_bit_clear(void *addr, int nr);
static __always_inline void atomic_bit_flip(void *addr, int nr);
static __always_inline bool atomic_bit_set_return(void *addr, int nr);
static __always_inline bool atomic_bit_clear_return(void *addr, int nr);
static __always_inline bool atomic_bit_flip_return(void *addr, int nr);
static __always_inline bool atomic_bit_read(void *addr, int nr);

static __always_inline void bit_set(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    *word |= mask;
}

static __always_inline void bit_clear(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    *word &= ~mask;
}

static __always_inline void bit_flip(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    *word ^= mask;
}

static __always_inline bool bit_set_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    bool ret = *word & mask;
    *word |= mask;
    return ret;
}

static __always_inline bool bit_clear_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    bool ret = *word & mask;
    *word &= ~mask;
    return ret;
}

static __always_inline bool bit_flip_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    bool ret = *word & mask;
    *word ^= mask;
    return ret;
}

static __always_inline bool bit_read(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    return (*word & mask) != 0;
}

static __always_inline void atomic_bit_set(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    __atomic_fetch_or(word, mask, DEFAULT_ORDER);
}

static __always_inline void atomic_bit_clear(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    __atomic_fetch_and(word, ~mask, DEFAULT_ORDER);
}

static __always_inline void atomic_bit_flip(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    __atomic_fetch_xor(word, mask, DEFAULT_ORDER);
}

static __always_inline bool atomic_bit_set_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    return __atomic_fetch_or(word, mask, DEFAULT_ORDER) & mask;
}

static __always_inline bool atomic_bit_clear_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    return __atomic_fetch_and(word, ~mask, DEFAULT_ORDER) & mask;
}

static __always_inline bool atomic_bit_flip_return(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    return __atomic_fetch_xor(word, mask, DEFAULT_ORDER) & mask;
}

static __always_inline bool atomic_bit_read(void *addr, int nr)
{
    unsigned long *word = (unsigned long *)addr + nr / WORD_BITS;
    unsigned long mask = 1UL << (nr % WORD_BITS);
    return (__atomic_load_n(word, DEFAULT_ORDER) & mask) != 0;
}

#endif /* _QUNIX_BITOPS_H_ */