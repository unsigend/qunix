/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include "handlers.h"
#include <qunix/kernel.h>
#include <qunix/traps.h>

void trap_handler(struct trap_frame *frame)
{
    int trapnum = trap_getnum(frame);
    switch (trapnum) {
    case TRAP_DIVIDE: trap_handler_divide(frame); break;
    case TRAP_DEBUG: trap_handler_debug(frame); break;
    case TRAP_NMI: trap_handler_nmi(frame); break;
    case TRAP_BRKPT: trap_handler_brkpt(frame); break;
    case TRAP_ILLEGAL: trap_handler_illegal(frame); break;
    case TRAP_ALIGN: trap_handler_align(frame); break;
    case TRAP_PGFLT: trap_handler_pgflt(frame); break;
    case TRAP_GPFLT: trap_handler_gpflt(frame); break;
    case TRAP_FPERR: trap_handler_fperr(frame); break;
    case TRAP_MCHK: trap_handler_mchk(frame); break;
    case TRAP_DBLFLT: trap_handler_dblflt(frame); break;
    case TRAP_STACK: trap_handler_stack(frame); break;
    default: panic("unhandled trap %d", trapnum);
    }
}