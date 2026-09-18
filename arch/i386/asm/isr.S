/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#define ASM_FILE 1
#include <asm/traps.h>

#define FAKE_ERRNO 0
#define PADDING 0

.section .text

.global isr0
.global isr1
.global isr2
.global isr3
.global isr6
.global isr8
.global isr12
.global isr13
.global isr14
.global isr16
.global isr17
.global isr18

isr0:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_DIVIDE
    jmp isr_handler

isr1:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_DEBUG
    jmp isr_handler

isr2:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_NMI
    jmp isr_handler

isr3:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_BRKPT
    jmp isr_handler

isr6:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_ILLOP
    jmp isr_handler

isr8:
    pushl $X86_TRAP_DBLFLT
    jmp isr_handler

isr12:
    pushl $X86_TRAP_STACK
    jmp isr_handler

isr13:
    pushl $X86_TRAP_GPFLT
    jmp isr_handler

isr14:
    pushl $X86_TRAP_PGFLT
    jmp isr_handler

isr16: 
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_FPERR
    jmp isr_handler

isr17:
    pushl $X86_TRAP_ALIGN
    jmp isr_handler

isr18:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_MCHK
    jmp isr_handler

isr_handler:
    pushw $PADDING
    pushw %ds
    pushw $PADDING
    pushw %es
    pushw $PADDING
    pushw %fs
    pushw $PADDING
    pushw %gs

    pusha
    cld
    /* call the kernel layer generic trap handler */
    pushl %esp
    call trap_handler
    addl $4, %esp

    popa
    popw %gs
    addl $2, %esp
    popw %fs
    addl $2, %esp
    popw %es
    addl $2, %esp
    popw %ds
    addl $2, %esp

    /* discard the pushed error code and trap number */
    addl $8, %esp
    iret
