# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

#define ASM_FILE 1

#include <kernel/mm/page.h>
#include <asm/multiboot2.h>
#include <asm/cpu_flags.h>
#include <asm/memlayout.h>

# i386 boot image, started by GRUB under Multiboot2.
#
# Linked symbols are virtual addresses. GRUB loads the image at the
# physical address, which the symbols are all based on KERNEL_VIRT_START.
# See ld/ld-i386.ld.

#define PADDING 0
#define LENGTH(start, end) ((end) - (start))
#define HEADER_LENGTH LENGTH(multiboot_header, multiboot_header_end)
#define CHECKSUM -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + HEADER_LENGTH)

#define STACK_SIZE PAGE_SIZE
#define STACK_ALIGN 16

#define HUGE_PAGE_MASK 0x83
#define HUGE_PAGE_MAKE(pa) (((pa) & ~0x3fffff) | HUGE_PAGE_MASK)

.section .bss
.global __boot_stack_top
.align STACK_ALIGN
__boot_stack_top:
    .skip STACK_SIZE
__boot_stack_bottom:

# temporary data section, the kernel future will reclaim this section.
.section .reclaimable, "aw", @nobits
.align PAGE_SIZE
boot_pgtable:
    .skip PAGE_SIZE
boot_pgtable_end:

.section .multiboot
.align MULTIBOOT_HEADER_ALIGN

multiboot_header:
    # magic number
    .long MULTIBOOT2_HEADER_MAGIC
    # architecture
    .long MULTIBOOT_ARCHITECTURE_I386
    # header length 
    .long HEADER_LENGTH
    # checksum
    .long CHECKSUM

    # information request tag
multiboot_header_info_request:
    .short MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
    .short MULTIBOOT_HEADER_TAG_OPTIONAL
    .long LENGTH(multiboot_header_info_request, multiboot_header_info_request_end)
    .long MULTIBOOT_TAG_TYPE_MMAP
    .long MULTIBOOT_TAG_TYPE_FRAMEBUFFER
    .long MULTIBOOT_TAG_TYPE_CMDLINE
    .long PADDING
multiboot_header_info_request_end:

    # entry address tag
    .short MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    .short 0
    .long 12
    .long _start - KERNEL_VIRT_START
    .long PADDING

    # end tag
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_end:

.section .text
.global _start
.type _start, @function

_start:
    # set up page table
    leal boot_pgtable - KERNEL_VIRT_START, %edi

    # identity map: 0-4MB pa to 0-4MB va
    movl $HUGE_PAGE_MAKE(0), %ecx
    movl %ecx, (%edi)

    # higher half: 0-1GB pa to 3GB-4GB va, 256 4MB pages
    movl $HUGE_PAGE_MAKE(0), %ecx
    leal 3072(%edi), %esi
    movl $256, %edx
map_higher_half:
    movl %ecx, (%esi)
    addl $MB_N(4), %ecx
    addl $4, %esi
    decl %edx
    jnz map_higher_half

    # load page table
    leal boot_pgtable - KERNEL_VIRT_START, %edx
    movl %edx, %cr3

    # enable PSE
    movl %cr4, %edi
    orl $(CR4_PSE_MASK), %edi
    movl %edi, %cr4

    # enable paging
    movl %cr0, %edi
    orl $(CR0_PG_MASK), %edi
    movl %edi, %cr0

    leal flush, %edi
    jmp *%edi

flush:
    # set stack pointer
    movl $__boot_stack_bottom, %esp

    # call bootmain(uint32_t magic, uint32_t mbi)
    pushl %ebx
    pushl %eax
    cld
    call bootmain

    # never reach here
    cli
halt:
    hlt
    jmp halt