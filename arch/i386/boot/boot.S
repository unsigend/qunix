# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.

#define ASM_FILE 1
#include <boot/multiboot2.h>
#include <qunix/mm/memlayout.h>

#define MB2_HEADER_LENGTH \
(multiboot_header_end - multiboot_header) /* header length */
#define MB2_HEADER_CHECKSUM \
-(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + MB2_HEADER_LENGTH)

#define PGDIR_SIZE 4096
#define PGDIR_ALIGN 4096
#define STACK_SIZE 0x1000 /* stack size 4 KB */

.section .multiboot
.align 8
multiboot_header:
    /* magic number */
    .long MULTIBOOT2_HEADER_MAGIC
    /* ISA: i386 */
    .long MULTIBOOT_ARCHITECTURE_I386
    /* header length */
    .long MB2_HEADER_LENGTH
    /* checksum */
    .long MB2_HEADER_CHECKSUM

multiboot_header_tag:

    /* memory map request */
    .short MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST
    .short 0
    .long 12
    .long MULTIBOOT_TAG_TYPE_MMAP
    .long 0 /* padding */

    /* end tag */
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_tag_end:

multiboot_header_end:

.section .bss

/* temporary stack for C */
.align 16
stack_bottom:
    .skip STACK_SIZE
stack_top:

.align PGDIR_ALIGN
pgdir:
    .skip PGDIR_SIZE

.section .text
.global _start
.type _start, @function

_start:
    /* preserve multiboot registers across paging setup */
    movl %eax, %edi         /* magic number */
    movl %ebx, %esi         /* mbi */

    /* All the ELF address is linked based on the KERNEL_VIRT_BASE, 
      so we can't directly use the symbol address, need to use a trick
      to calculate the physical address of the symbols based on the LMA 
      address. For relative calls/jumps the rel32 offset is the same for
      both LMA and VMA. */
    movl $(stack_top - KERNEL_VIRT_BASE), %esp

    /* enable PSE 4MB page in CR4 */
    movl %cr4, %eax
    orl $0x10, %eax
    movl %eax, %cr4

    movl $(pgdir - KERNEL_VIRT_BASE), %ecx

    /* 0x83 = 0b10000011 = Present | Read/Write | Page Size=1 
       we need to set the Present bit to 1, and the Read/Write bit to 1,
       and the Page Size bit to 1, since we are using 4MB large page.

       768 = 3GB / 4MB, 3072 = 768 * 4
    */

    /* PDE[0] : identity map  PHYS 0-4MB -> VIRT 0 - 4MB */
    movl $0x00000083, (%ecx)

    /* PDE[768 - 1023] : higher half map for all kernel 
       virtual space PHYS 0-1GB -> VIRT 3GB-4GB */
    xorl %eax, %eax         /* physical base 0 */
    movl $(768 * 4), %edx
.Lmap_page:
    movl %eax, (%ecx, %edx)
    orl $0x83, (%ecx, %edx)
    addl $0x400000, %eax  /* 4MB per page */
    addl $4, %edx
    cmpl $(1024 * 4), %edx
    jl .Lmap_page

    movl %ecx, %cr3 /* load CR3 */

    /* enable paging */
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    pushl $0
    popf
    xorl %ebp, %ebp

    movl $.Lhigher_half, %eax
    jmp *%eax /* indirect jump to virtual address, update %eip */

.Lhigher_half:
    movl $(stack_top), %esp
    xorl %ebp, %ebp

    pushl $0
    popf


    /* int bootmain(magic, mbi); */
    pushl %esi
    pushl %edi
    call bootmain

.Lloop:
    hlt
    jmp .Lloop
