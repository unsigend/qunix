# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.

#define ASM_FILE 1
#include <boot/multiboot2.h>

#define STACK_SIZE 0x1000 /* stack size 4 KB */
#define MB2_HEADER_LENGTH \
(multiboot_header_end - multiboot_header) /* header length */
#define MB2_HEADER_CHECKSUM \
-(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + MB2_HEADER_LENGTH)


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

    /* end tag */
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_tag_end:

multiboot_header_end:

.section .bss
.align 16
stack_bottom:
.skip STACK_SIZE
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    /* set stack pointer */
    movl $(stack_top), %esp

    pushl $0
    popf
    xorl %ebp, %ebp

    /* push multiboot information structure */
    pushl %ebx
    /* push magic number */
    pushl %eax

    /* bootmain(uint32_t magic, multiboot_info_t *mbi) */
    call bootmain
loop:
    hlt
    jmp loop
