/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "asm/cpu.h"
#include "trap.h"

#include <asm/idt.h>
#include <asm/page.h>
#include <asm/trap.h>
#include <asm/vector.h>

#include <kernel/irq.h>
#include <kernel/mm/vm.h>
#include <kernel/panic.h>

extern char __trap_stubs[];

unsigned long trap_stubs(unsigned int nr)
{
    if (nr >= IDT_NR)
        panic("trap_stubs: invalid trap number");

    return (unsigned long)__trap_stubs + nr * TRAP_STUB_SIZE;
}

void trap_handle(struct trap_frame *frame)
{
    unsigned int nr;

    nr = frame->trapnum;

    if (nr >= X86_VECTOR_IRQ_NR_MIN &&
        nr <= X86_VECTOR_IRQ_NR_MAX) /* external hardware interrupt */
    {
        irqreturn_t irqret;

        irqret = irq_handle(nr - X86_VECTOR_IRQ_NR_MIN);
        UNUSED(irqret);
    }

    else if (nr == X86_VECTOR_SYSCALL) /* TODO: System Call */
    {
        int ret;
        UNUSED(ret);
    }

    else /* synchronous exceptions from CPU and NMI. */
    {
        switch (nr) {
        case X86_TRAP_DE: trap_die(frame->eip, nr, frame->err, "divide error");
        case X86_TRAP_DB: trap_die(frame->eip, nr, frame->err, "debug");
        case X86_TRAP_NMI: trap_die(frame->eip, nr, frame->err, "nmi");
        case X86_TRAP_BP: trap_die(frame->eip, nr, frame->err, "breakpoint");
        case X86_TRAP_OF: trap_die(frame->eip, nr, frame->err, "overflow");
        case X86_TRAP_BR: trap_die(frame->eip, nr, frame->err, "bounds check");
        case X86_TRAP_UD:
            trap_die(frame->eip, nr, frame->err, "invalid opcode");
        case X86_TRAP_NM:
            trap_die(frame->eip, nr, frame->err, "device not available");
        case X86_TRAP_DF: trap_die(frame->eip, nr, frame->err, "double fault");
        case X86_TRAP_CSO:
            trap_die(frame->eip, nr, frame->err, "coprocessor segment overrun");
        case X86_TRAP_TS: trap_die(frame->eip, nr, frame->err, "invalid TSS");
        case X86_TRAP_NP:
            trap_die(frame->eip, nr, frame->err, "segment not present");
        case X86_TRAP_SS:
            trap_die(frame->eip, nr, frame->err, "stack-segment fault");
        case X86_TRAP_GP:
            trap_die(frame->eip, nr, frame->err, "general protection fault");
        case X86_TRAP_PF: /* page fault */
        {
            unsigned int flags = 0;
            unsigned int err;
            virt_addr_t addr;

            err = frame->err;
            if (err & PF_P)
                flags |= VM_FAULT_PRESENT;
            if (err & PF_WR)
                flags |= VM_FAULT_WRITE;
            if (err & PF_USER)
                flags |= VM_FAULT_USER;
            if (err & PF_ID)
                flags |= VM_FAULT_EXEC;

            addr = cr2_read();
            vm_handle_fault(addr, flags);
        }
        case X86_TRAP_MF:
            trap_die(frame->eip, nr, frame->err,
                     "x87 floating-point exception");
        case X86_TRAP_AC:
            trap_die(frame->eip, nr, frame->err, "alignment check");
        case X86_TRAP_MC: trap_die(frame->eip, nr, frame->err, "machine check");
        case X86_TRAP_XM:
            trap_die(frame->eip, nr, frame->err,
                     "SIMD floating-point exception");
        case X86_TRAP_VE:
            trap_die(frame->eip, nr, frame->err, "virtualization exception");
        case X86_TRAP_CP:
            trap_die(frame->eip, nr, frame->err,
                     "control protection exception");
        default: panic("unhandled trap: %u", nr);
        }
    }
}