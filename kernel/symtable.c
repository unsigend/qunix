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

#include <kernel/symtable.h>

const struct ksym *ksym_lookup(unsigned long addr)
{
    size_t lo = 0, hi = nksyms;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        const struct ksym *sym = &ksymtable[mid];

        if (addr == sym->addr)
            return sym;
        if (addr < sym->addr)
            hi = mid;
        else {
            if (mid == nksyms - 1)
                return sym;
            if (addr < ksymtable[mid + 1].addr)
                return sym;
            else
                lo = mid + 1;
        }
    }

    return NULL;
}