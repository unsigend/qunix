# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.

#define ASM_FILE 1
#include <asm/traps.h>
#include <asm/i8259.h>

#define FAKE_ERRNO 0
#define PADDING 0

.section .text

# software interrupts (exceptions) from CPU
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
    # call the kernel layer generic trap handler
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

    # discard the pushed error code and trap number
    addl $8, %esp
    iret

# external hardware interrupts
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15


irq0:
    pusha
    cld
    pushl $X86_IRQ_TIMER
    call irq_dispatch
    addl $4, %esp
    call task_preempt
    popa
    iret

irq1:
    pusha
    cld
    pushl $X86_IRQ_KBD
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq2:
    pusha
    cld
    pushl $X86_IRQ_CASCADE
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq3:
    pusha
    cld
    pushl $X86_IRQ_COM2
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq4:
    pusha
    cld
    pushl $X86_IRQ_COM1
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq5:
    pusha
    cld
    pushl $X86_IRQ_LPT2
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq6:
    pusha
    cld
    pushl $X86_IRQ_FLOPPY
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq7:
    pusha
    cld
    pushl $X86_IRQ_LPT1
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq8:
    pusha
    cld
    pushl $X86_IRQ_CMOS
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq9:
    pusha
    cld
    pushl $X86_IRQ_AVAL1
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq10:
    pusha
    cld
    pushl $X86_IRQ_AVAL2
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq11:
    pusha
    cld
    pushl $X86_IRQ_AVAL3
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq12:
    pusha
    cld
    pushl $X86_IRQ_PS2M
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq13:
    pusha
    cld
    pushl $X86_IRQ_FPU
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq14:
    pusha
    cld
    pushl $X86_IRQ_ATA
    call irq_dispatch
    addl $4, %esp
    popa
    iret

irq15:
    pusha
    cld
    pushl $X86_IRQ_ATA2
    call irq_dispatch
    addl $4, %esp
    popa
    iret