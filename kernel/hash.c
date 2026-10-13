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

#include <kernel/hash.h>
#include <kernel/stddef.h>

static uint32_t fnv1a(const void *data, size_t size)
{
    uint32_t hash = 2166136261u;
    const uint8_t *bytes = (const uint8_t *)data;

    while (size--) {
        hash ^= *bytes++;
        hash = hash * 0x01000193u;
    }

    return hash;
}

static uint32_t djb2(unsigned char *str)
{
    uint32_t hash = 5381;
    unsigned char c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

uint32_t hash_u32(uint32_t key) { return fnv1a(&key, sizeof(uint32_t)); }
uint32_t hash_u64(uint64_t key) { return fnv1a(&key, sizeof(uint64_t)); }
uint32_t hash_str(const char *str) { return djb2((unsigned char *)str); }