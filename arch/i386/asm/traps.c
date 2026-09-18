/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/traps.h>
#include <qunix/traps.h>

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