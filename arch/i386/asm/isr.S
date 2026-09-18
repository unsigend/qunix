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
isr0:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_DIVIDE
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
