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

void trap_handler_divide(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("divide by zero");
}

void trap_handler_debug(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for debug");
}

void trap_handler_nmi(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for NMI");
}

void trap_handler_brkpt(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for breakpoint");
}

void trap_handler_illegal(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for illegal instruction");
}

void trap_handler_align(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for alignment check");
}

void trap_handler_pgflt(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("page fault at 0x%zx", trap_get_fault_addr());
}

void trap_handler_gpflt(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for general protection fault");
}

void trap_handler_fperr(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for floating point exception");
}

void trap_handler_mchk(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for machine check");
}

void trap_handler_dblflt(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for double fault");
}

void trap_handler_stack(struct trap_frame *frame)
{
    UNUSED(frame);
    panic("not implemented trap handler for stack segment fault");
}
