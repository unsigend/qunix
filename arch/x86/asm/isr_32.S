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

#include <asm/traps.h>
#include <asm/i8259.h>

#define FAKE_ERRNO 0
#define PADDING 0

.section .text

# Software interrupts (exceptions) from CPU

.global isr0
.type isr0, @function
isr0:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_DIVIDE
    jmp isr_handler

.global isr1
.type isr1, @function
isr1:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_DEBUG
    jmp isr_handler

.global isr2
.type isr2, @function
isr2:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_NMI
    jmp isr_handler

.global isr3
.type isr3, @function
isr3:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_BRKPT
    jmp isr_handler

.global isr6
.type isr6, @function
isr6:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_ILLOP
    jmp isr_handler

.global isr8
.type isr8, @function
isr8:
    pushl $X86_TRAP_DBLFLT
    jmp isr_handler

.global isr12
.type isr12, @function
isr12:
    pushl $X86_TRAP_STACK
    jmp isr_handler

.global isr13
.type isr13, @function
isr13:
    pushl $X86_TRAP_GPFLT
    jmp isr_handler

.global isr14
.type isr14, @function
isr14:
    pushl $X86_TRAP_PGFLT
    jmp isr_handler

.global isr16
.type isr16, @function
isr16: 
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_FPERR
    jmp isr_handler

.global isr17
.type isr17, @function
isr17:
    pushl $X86_TRAP_ALIGN
    jmp isr_handler

.global isr18
.type isr18, @function
isr18:
    pushl $FAKE_ERRNO
    pushl $X86_TRAP_MCHK
    jmp isr_handler

# External hardware interrupts from 0x20

.global isr32
.type isr32, @function
isr32:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_TIMER)
    jmp isr_handler

.global isr33
.type isr33, @function
isr33:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_KBD)
    jmp isr_handler
    
.global isr34
.type isr34, @function
isr34:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_CASCADE)
    jmp isr_handler

.global isr35
.type isr35, @function
isr35:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_COM2)
    jmp isr_handler

.global isr36
.type isr36, @function
isr36:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_COM1)
    jmp isr_handler

.global isr37
.type isr37, @function
isr37:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_LPT2)
    jmp isr_handler

.global isr38
.type isr38, @function
isr38:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_FLOPPY)
    jmp isr_handler

.global isr39
.type isr39, @function
isr39:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_LPT1)
    jmp isr_handler
    
.global isr40
.type isr40, @function
isr40:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_CMOS)
    jmp isr_handler

.global isr41
.type isr41, @function
isr41:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_AVAL1)
    jmp isr_handler

.global isr42
.type isr42, @function
isr42:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_AVAL2)
    jmp isr_handler

.global isr43
.type isr43, @function
isr43:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_AVAL3)
    jmp isr_handler

.global isr44
.type isr44, @function
isr44:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_PS2M)
    jmp isr_handler

.global isr45
.type isr45, @function
isr45:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_FPU)
    jmp isr_handler

.global isr46
.type isr46, @function
isr46:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_ATA)
    jmp isr_handler

.global isr47
.type isr47, @function
isr47:
    pushl $FAKE_ERRNO
    pushl $(X86_IRQ_BASE + X86_IRQ_ATA2)
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

    call task_preempt

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


