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

#include <kernel/stdint.h>
#include <kernel/string.h>
#include <kernel/utils.h>

#define GET(base, index, sz) ((void *)((char *)(base) + (index) * (sz)))

void swap(void *lhs, void *rhs, size_t sz)
{
    unsigned long word;
    unsigned char byte;
    size_t i = 0;

    if (lhs == rhs)
        return;

    for (; i + sizeof(word) <= sz; i += sizeof(word)) {
        memcpy(&word, lhs + i, sizeof(word));
        memcpy(lhs + i, rhs + i, sizeof(word));
        memcpy(rhs + i, &word, sizeof(word));
    }
    for (; i < sz; i++) {
        memcpy(&byte, lhs + i, 1);
        memcpy(lhs + i, rhs + i, 1);
        memcpy(rhs + i, &byte, 1);
    }
}

int sort(void *base, size_t n, size_t sz, int (*cmp)(void *, void *))
{
    size_t min, mid, max;
    size_t pivot;

    if (!base || !sz || !cmp)
        return -1;
    if (n <= 1)
        return 0;

    /* Median of three pivot selection: MIN[[min, mid], max], MAX[mid, max]*/
    min = 0, mid = n / 2, max = n - 1;
    if (cmp(GET(base, min, sz), GET(base, mid, sz)) > 0)
        swap(GET(base, min, sz), GET(base, mid, sz), sz);
    if (cmp(GET(base, min, sz), GET(base, max, sz)) > 0)
        swap(GET(base, min, sz), GET(base, max, sz), sz);
    if (cmp(GET(base, mid, sz), GET(base, max, sz)) > 0)
        swap(GET(base, mid, sz), GET(base, max, sz), sz);

    if (n <= 3)
        return 0;

    pivot = max - 1;
    swap(GET(base, mid, sz), GET(base, pivot, sz), sz);

    size_t l = 1, r = n - 3;
    while (l < r) {
        while (l < r && cmp(GET(base, l, sz), GET(base, pivot, sz)) < 0)
            l++;
        while (l < r && cmp(GET(base, r, sz), GET(base, pivot, sz)) > 0)
            r--;
        if (l < r) {
            swap(GET(base, l, sz), GET(base, r, sz), sz);
            l++, r--;
        }
    }
    size_t pos = l;
    if (cmp(GET(base, pos, sz), GET(base, pivot, sz)) <= 0)
        pos++;
    swap(GET(base, pos, sz), GET(base, pivot, sz), sz);

    return sort(base, pos, sz, cmp) ||
           sort(GET(base, pos + 1, sz), n - pos - 1, sz, cmp);
}

void *bsearch(const void *key, const void *ptr, size_t n, size_t size,
              int (*cmp)(const void *, const void *))
{
    const char *base = (const char *)ptr;
    size_t lo = 0;
    size_t hi = n;

    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        const char *p = base + mid * size;
        int c = cmp(key, p);

        if (c == 0)
            return (void *)p;
        if (c < 0)
            hi = mid;
        else
            lo = mid + 1;
    }

    return NULL;
}