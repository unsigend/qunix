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

#include "handlers.h"

#include <kernel/panic.h>

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