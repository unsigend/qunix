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

#include <asm/cpu.h>
#include <asm/traps.h>

#include <kernel/traps.h>
#include <kernel/types.h>

/* Map a x86 hardware trap number to a generic kernel trap number */
static int map_trapnum(int trapnum)
{
    switch (trapnum) {
    case X86_TRAP_DIVIDE: return TRAP_DIVIDE;
    case X86_TRAP_DEBUG: return TRAP_DEBUG;
    case X86_TRAP_NMI: return TRAP_NMI;
    case X86_TRAP_BRKPT: return TRAP_BRKPT;
    case X86_TRAP_ILLOP: return TRAP_ILLEGAL;
    case X86_TRAP_DBLFLT: return TRAP_DBLFLT;
    case X86_TRAP_STACK: return TRAP_STACK;
    case X86_TRAP_GPFLT: return TRAP_GPFLT;
    case X86_TRAP_PGFLT: return TRAP_PGFLT;
    case X86_TRAP_FPERR: return TRAP_FPERR;
    case X86_TRAP_ALIGN: return TRAP_ALIGN;
    case X86_TRAP_MCHK: return TRAP_MCHK;
    default: return -1;
    }
}

int trap_getnum(const struct trap_frame *frame)
{
    return map_trapnum(frame->trapnum);
}

virt_addr_t trap_fault_addr(void) { return (virt_addr_t)cr2_read(); }