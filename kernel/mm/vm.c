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
#include <kernel/mm/vm.h>

int vm_map_pages(pagetable_t pagetable, virt_addr_t va, phys_addr_t pa,
                 size_t n, uint32_t perm)
{
    int err = 0;

    for (size_t i = 0; i < n; i++)
        if ((err = vm_map_page(pagetable, va + i * PAGE_SIZE,
                               pa + i * PAGE_SIZE, perm)) < 0)
            return err;

    return 0;
}

int vm_unmap_pages(pagetable_t pagetable, virt_addr_t va, size_t n)
{
    int err = 0;
    for (size_t i = 0; i < n; i++)
        if ((err = vm_unmap_page(pagetable, va + i * PAGE_SIZE)) < 0)
            return err;

    return 0;
}