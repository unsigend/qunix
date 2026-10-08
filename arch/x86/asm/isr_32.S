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


#define FAKE_ERRNO 0
#define PADDING 0

.section .text



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


