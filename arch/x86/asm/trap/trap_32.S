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

#include <asm/trap.h>
#include <asm/idt.h>

#define PADDING 0x00
#define ERRCODE 0x00

.macro TRAP_STUB num
    .align TRAP_STUB_ALIGN
    .if !(\num == 8 || (\num >= 10 && \num <= 14) || \num == 17 || \num == 21)
        pushl $ERRCODE
    .endif
    pushl $\num
    jmp trap_entry
.endm

.section .text
.global __trap_stubs
.align TRAP_STUB_ALIGN
__trap_stubs:
    .set num, 0
    .rept IDT_NR
        TRAP_STUB num
        .set num, num + 1
    .endr

.section .text
.global trap_entry
.type trap_entry, @function

trap_entry:
    pushw $PADDING
    pushw %ds
    pushw $PADDING
    pushw %es
    pushw $PADDING
    pushw %fs
    pushw $PADDING
    pushw %gs

    pushal

    pushl %esp # struct trap_frame *frame
    call trap_handle
    addl $4, %esp

    popal

    popw %gs
    addl $2, %esp
    popw %fs
    addl $2, %esp
    popw %es
    addl $2, %esp
    popw %ds
    addl $2, %esp

    # clear error code and trap number
    addl $8, %esp 

    iret

