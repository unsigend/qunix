/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

/* Kernel is linked and loaded at 1MB, physical address == link address. */

ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

KERNEL_LOAD_ADDR = 0x100000;

SECTIONS
{
    . = KERNEL_LOAD_ADDR;

    .text ALIGN(4K) :
    {
        KEEP(*(.multiboot)) /* boot header must be within the first 32KB */
        *(.text*)
    }

    .rodata ALIGN(4K) :
    {
        *(.rodata*)
    }

    .data ALIGN(4K) :
    {
        *(.data*)
    }

    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss*)
    }

    _kernel_start = KERNEL_LOAD_ADDR;
    _kernel_end = .;
}
