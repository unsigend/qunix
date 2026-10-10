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


# Store the current CPU state into the given struct arch_cpu.
#   void read_cpu(struct arch_cpu *cpu)

.global read_cpu
.type read_cpu, @function

read_cpu:
    pushl %eax
    # eax store the cpu pointer
    movl 8(%esp), %eax

    movl %ebx, 4(%eax)
    movl %ecx, 8(%eax)
    movl %edx, 12(%eax)
    movl %esi, 16(%eax)
    movl %edi, 20(%eax)
    # store esp before call read_cpu
    leal 8(%esp), %ecx
    movl %ecx, 24(%eax)
    movl %ebp, 28(%eax)

    movl 4(%esp), %ecx
    movl %ecx, 32(%eax)
    pushf
    popl 36(%eax)

    movw %cs, 40(%eax)
    movw %ds, 42(%eax)
    movw %ss, 44(%eax)
    movw %es, 46(%eax)
    movw %fs, 48(%eax)
    movw %gs, 50(%eax)

    movl %cr0, %ecx
    movl %ecx, 52(%eax)

    movl %cr2, %ecx
    movl %ecx, 56(%eax)

    movl %cr3, %ecx
    movl %ecx, 60(%eax)

    movl %cr4, %ecx
    movl %ecx, 64(%eax)

    # ecx store the original saved eax
    popl %ecx
    movl %ecx, (%eax)
    ret
