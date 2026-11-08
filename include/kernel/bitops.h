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

#include <kernel/bits.h>
#include <kernel/stdbool.h>

static inline bool bit_load(void *addr, int nr);
static inline void bit_set(void *addr, int nr);
static inline void bit_clear(void *addr, int nr);
static inline void bit_flip(void *addr, int nr);
static inline bool bit_fetch_set(void *addr, int nr);
static inline bool bit_fetch_clear(void *addr, int nr);
static inline bool bit_fetch_flip(void *addr, int nr);

static inline bool atomic_bit_load(void *addr, int nr);
static inline void atomic_bit_set(void *addr, int nr);
static inline void atomic_bit_clear(void *addr, int nr);
static inline void atomic_bit_flip(void *addr, int nr);
static inline bool atomic_bit_fetch_set(void *addr, int nr);
static inline bool atomic_bit_fetch_clear(void *addr, int nr);
static inline bool atomic_bit_fetch_flip(void *addr, int nr);

#define _WORD_BASE(addr, nr) ((unsigned long *)(addr) + (nr) / WORD_BITS)
#define _WORD_MASK(nr) (1UL << ((nr) % WORD_BITS))

static inline bool bit_load(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    return !!(*word & mask);
}

static inline void bit_set(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    *word |= mask;
}

static inline void bit_clear(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    *word &= ~mask;
}

static inline void bit_flip(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    *word ^= mask;
}

static inline bool bit_fetch_set(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    bool old = !!(*word & mask);
    *word |= mask;
    return old;
}

static inline bool bit_fetch_clear(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    bool old = !!(*word & mask);
    *word &= ~mask;
    return old;
}

static inline bool bit_fetch_flip(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    bool old = !!(*word & mask);
    *word ^= mask;
    return old;
}

static inline bool atomic_bit_load(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    return !!(__atomic_load_n(word, __ATOMIC_RELAXED) & mask);
}

static inline void atomic_bit_set(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    __atomic_fetch_or(word, mask, __ATOMIC_RELAXED);
}

static inline void atomic_bit_clear(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    __atomic_fetch_and(word, ~mask, __ATOMIC_RELAXED);
}

static inline void atomic_bit_flip(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    __atomic_fetch_xor(word, mask, __ATOMIC_RELAXED);
}

static inline bool atomic_bit_fetch_set(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    return !!(__atomic_fetch_or(word, mask, __ATOMIC_RELAXED) & mask);
}

static inline bool atomic_bit_fetch_clear(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    return !!(__atomic_fetch_and(word, ~mask, __ATOMIC_RELAXED) & mask);
}

static inline bool atomic_bit_fetch_flip(void *addr, int nr)
{
    unsigned long *word = _WORD_BASE(addr, nr);
    unsigned long mask = _WORD_MASK(nr);
    return !!(__atomic_fetch_xor(word, mask, __ATOMIC_RELAXED) & mask);
}

#endif /* _QUNIX_BITOPS_H_ */