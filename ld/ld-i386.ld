/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

BASE_OFFSET = 0x100000; /* 1MB offset */
KERNEL_PHYS_BASE = BASE_OFFSET; /* Kernel physical base address 1MB */
KERNEL_VIRT_BASE = 0xC0000000; /* Kernel virtual base address 3GB */

/* qunix i386 architecture memory layout using a higher half kernel layout, 
 * the kernel is mapped to 3GB and above. The lower 0-3GB reserved for user space.
 * 
 * physical memory layout:
 *   [0x00000000 - 0x00100000]           0-1MB: BIOS data, video memory, IO ports reserved
 *   [0x00100000 - KERNEL_PHYS_END]      1MB - kernel end: kernel code, data, bss include boot section
 *
 * virtual memory layout:
 *   [0x00000000 - 0xC0000000]          0-3GB: User space code, data, bss
 *   [0xC0000000 - 0xC0100000]          3GB-3GB+1MB: Mapped BIOS data, video memory, IO ports
 *   [0xC0100000 - 0xFFFFFFFF]          3GB+1MB-4GB: Kernel space code, data, bss
*/

ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = KERNEL_PHYS_BASE + KERNEL_VIRT_BASE; /* 3GB+1MB */

    /*  VMA: kernel links address namely for ELF usage 
        LMA: Load memory address namely for GRUB load at physical address
        AT() sets LMA address
    */
    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRT_BASE)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRT_BASE)
    {
        *(.rodata)
    }

    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRT_BASE)
    {
        *(.data)
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRT_BASE) 
    {
        *(COMMON)
        *(.bss)
    }

    /* export symbols for kernel usage */
    _kernel_phys_start = LOADADDR(.text);
    _kernel_phys_end = LOADADDR(.bss) + SIZEOF(.bss);

    _kernel_virt_start = KERNEL_VIRT_BASE;
    _kernel_virt_end = .;
}