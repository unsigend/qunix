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

#include <kernel/mm/page.h>

int page_map_range(pagetable_t root, phys_addr_t pa, virt_addr_t va, size_t n,
                   unsigned int flags)
{
    int err;
    size_t nmap;

    for (nmap = 0; nmap < n; nmap++) {
        if ((err = page_map(root, pa + nmap * PAGE_SIZE, va + nmap * PAGE_SIZE,
                            flags))) {
            page_unmap_range(root, va, nmap);
            return err;
        }
    }
    return 0;
}

void page_unmap_range(pagetable_t root, virt_addr_t va, size_t n)
{
    size_t nunmap;
    for (nunmap = 0; nunmap < n; nunmap++)
        page_unmap(root, va + nunmap * PAGE_SIZE);
}