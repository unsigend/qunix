/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/mm/page.h>
#include <qunix/mm/vm.h>

extern int vm_map_pages(pagetable_t pagetable, virt_addr_t va, phys_addr_t pa,
                        size_t n, uint32_t perm, int alloc)
{
    int err = 0;

    for (size_t i = 0; i < n; i++)
        if ((err = vm_map_page(pagetable, va + i * PAGE_SIZE,
                               pa + i * PAGE_SIZE, perm, alloc)) < 0)
            return err;

    return 0;
}

extern void vm_unmap_pages(pagetable_t pagetable, virt_addr_t va, size_t n)
{
    for (size_t i = 0; i < n; i++)
        vm_unmap_page(pagetable, va + i * PAGE_SIZE);
}