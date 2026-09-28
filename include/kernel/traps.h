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

#ifndef _QUNIX_TRAPS_H_
#define _QUNIX_TRAPS_H_

#include <kernel/compiler.h>
#include <kernel/types.h>

#define TRAP_DIVIDE 0  /* Integer divide by zero */
#define TRAP_DEBUG 1   /* Debug/Single-step */
#define TRAP_NMI 2     /* Non-maskable Interrupt */
#define TRAP_BRKPT 3   /* Breakpoint */
#define TRAP_ILLEGAL 4 /* Illegal/Undefined instruction */
#define TRAP_ALIGN 5   /* Alignment Fault */
#define TRAP_PGFLT 6   /* Page Fault */
#define TRAP_GPFLT 7   /* General protection/Access violation */
#define TRAP_FPERR 8   /* Floating-point Error */
#define TRAP_MCHK 9    /* Machine check/Hardware error */
#define TRAP_DBLFLT 10 /* Double/Nested Fault */
#define TRAP_STACK 11  /* Stack Overflow/Stack Fault */

#define TRAP_MAX 12

CONTRACT struct trap_frame;

extern void trap_handler(struct trap_frame *frame);
CONTRACT extern int trap_getnum(const struct trap_frame *frame);
CONTRACT extern virt_addr_t trap_fault_addr(void);

#endif /* _QUNIX_TRAPS_H_ */