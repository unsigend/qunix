# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.


# Context Switch between two tasks
#
# void context_switch(struct task_context **saved,
#                     struct task_context *restored)
#
# Save the current registers on the stack, creating
# a struct task_context on the stack, and save its
# address in *saved. Switch stacks to restored and 
# pop previously saved registers from the stack.

.global context_switch
.type context_switch, @function

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
