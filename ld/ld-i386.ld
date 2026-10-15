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

ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

KERNEL_OFFSET = 0x100000; /* 1MB */
KERNEL_PHYS_BASE = 0x0;
KERNEL_VIRT_BASE = 0xC0000000; /* 3GB */

/* load base address for GRUB */
KERNEL_IMG_LOAD_BASE = KERNEL_PHYS_BASE + KERNEL_OFFSET;

/* virtual base address for ELF file */
KERNEL_IMG_VIRT_BASE = KERNEL_VIRT_BASE + KERNEL_OFFSET;

SECTIONS
{
    . = KERNEL_IMG_VIRT_BASE;

    /* VMA: kernel image virtual address in ELF file 
     * LMA: kernel image physical address load by GRUB */

    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRT_BASE)
    {
        KEEP(*(.multiboot)) /* boot header must be within the first 32KB */
        *(.text*)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRT_BASE)
    {
        *(.rodata*)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRT_BASE)
    {
        *(.data*)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRT_BASE)
    {
        *(COMMON)
        *(.bss*)
    }

    _kernel_image_phys_start = KERNEL_IMG_LOAD_BASE;
    _kernel_image_phys_end = LOADADDR(.bss) + SIZEOF(.bss);

    _kernel_image_virt_start = KERNEL_IMG_VIRT_BASE;
    _kernel_image_virt_end = ADDR(.bss) + SIZEOF(.bss);

    _kernel_image_size = _kernel_image_phys_end - _kernel_image_phys_start;
}
