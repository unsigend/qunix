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

#include <asm/memlayout.h>

#include <kernel/config.h>
#include <kernel/mm/memlayout.h>
#include <kernel/panic.h>

extern char _kernel_image_phys_start[];
extern char _kernel_image_phys_end[];
extern char _kernel_image_virt_start[];
extern char _kernel_image_virt_end[];
extern char _kernel_image_size[];

phys_addr_t kernel_image_phys_start = (phys_addr_t)_kernel_image_phys_start;
phys_addr_t kernel_image_phys_end = (phys_addr_t)_kernel_image_phys_end;
virt_addr_t kernel_image_virt_start = (virt_addr_t)_kernel_image_virt_start;
virt_addr_t kernel_image_virt_end = (virt_addr_t)_kernel_image_virt_end;
size_t kernel_image_size = (size_t)_kernel_image_size;

#if CONFIG_DEBUG

phys_addr_t va_to_pa(virt_addr_t va)
{
    if (va < KERNEL_VIRT_START)
        panic("va_to_pa: va 0x%.8x is below KERNEL_VIRT_START 0x%.8x",
              (unsigned int)va, (unsigned int)KERNEL_VIRT_START);

    return va - KERNEL_VIRT_START;
}
virt_addr_t pa_to_va(phys_addr_t pa)
{
    if (pa >= KERNEL_PHYS_END)
        panic("pa_to_va: pa 0x%.8x is above KERNEL_PHYS_END 0x%.8x",
              (unsigned int)pa, (unsigned int)KERNEL_PHYS_END);

    return pa + KERNEL_VIRT_START;
}

#else

phys_addr_t va_to_pa(virt_addr_t va) { return va - KERNEL_VIRT_START; }
virt_addr_t pa_to_va(phys_addr_t pa) { return pa + KERNEL_VIRT_START; }

#endif /* CONFIG_DEBUG */
