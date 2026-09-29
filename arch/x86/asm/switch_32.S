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


.global context_switch
.type context_switch, @function

# Context Switch between two tasks
#
# void context_switch(struct task_context **saved,
#                     struct task_context *restored)
#
# Save the current registers on the stack, creating a struct
# task_context on the stack, and save its address in *saved. 
# Switch stacks to restored and pop previously saved registers 
# from the stack.

context_switch:
    movl 4(%esp), %eax # saved
    movl 8(%esp), %edx # restored

    # Save current callee-saved registers
    pushl %ebp
    pushl %ebx
    pushl %esi
    pushl %edi

    # Switch tasks
    movl %esp, (%eax)
    movl %edx, %esp

    # Restore previous callee-saved registers
    popl %edi
    popl %esi
    popl %ebx
    popl %ebp

    ret
