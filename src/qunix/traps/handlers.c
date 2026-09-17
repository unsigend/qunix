/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include "handlers.h"
#include <qunix/kernel.h>

void trap_handler_divide(struct trap_frame *frame)
{
    (void)frame;
    panic("divide by zero");
}

void trap_handler_debug(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_nmi(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_brkpt(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_illegal(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_align(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_pgflt(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_gpflt(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_fperr(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_mchk(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_dblflt(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}

void trap_handler_stack(struct trap_frame *frame)
{
    (void)frame;
    panic("Not implemented");
}
