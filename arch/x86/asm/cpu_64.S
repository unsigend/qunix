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
    # rdi store the cpu pointer, no need store rdi
    movq %rax, (%rdi)
    movq %rbx, 8(%rdi)
    movq %rcx, 16(%rdi)
    movq %rdx, 24(%rdi)
    movq %rsi, 32(%rdi)
    movq %rdi, 40(%rdi)
    
    leaq 8(%rsp), %rcx
    movq %rcx, 48(%rdi)
    movq %rbp, 56(%rdi)

    movq %r8, 64(%rdi)
    movq %r9, 72(%rdi)
    movq %r10, 80(%rdi)
    movq %r11, 88(%rdi)
    movq %r12, 96(%rdi)
    movq %r13, 104(%rdi)
    movq %r14, 112(%rdi)
    movq %r15, 120(%rdi)

    movq (%rsp), %rcx
    movq %rcx, 128(%rdi)
    pushfq
    popq 136(%rdi)

    movw %cs, 144(%rdi)
    movw %ds, 146(%rdi)
    movw %ss, 148(%rdi)
    movw %es, 150(%rdi)
    movw %fs, 152(%rdi)
    movw %gs, 154(%rdi)

    movq %cr0, %rcx
    movq %rcx, 156(%rdi)

    movq %cr2, %rcx
    movq %rcx, 164(%rdi)

    movq %cr3, %rcx
    movq %rcx, 172(%rdi)

    movq %cr4, %rcx
    movq %rcx, 180(%rdi)

    movq %cr8, %rcx
    movq %rcx, 188(%rdi)

    ret
    
    