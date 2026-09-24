/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_TRAPS_H
#define QUNIX_TRAPS_H

#include <qunix/macros.h>
#include <qunix/mm/types.h>

/* Generic kernel trap numbers, architecture-independent. Each architecture must
 * provide a mapper from its hardware vectors to these logical trap numbers */

#define TRAP_DIVIDE 0  /* integer divide by zero */
#define TRAP_DEBUG 1   /* debug / single-step */
#define TRAP_NMI 2     /* non-maskable interrupt */
#define TRAP_BRKPT 3   /* breakpoint */
#define TRAP_ILLEGAL 4 /* illegal / undefined instruction */
#define TRAP_ALIGN 5   /* alignment fault */
#define TRAP_PGFLT 6   /* page fault */
#define TRAP_GPFLT 7   /* general protection / access violation */
#define TRAP_FPERR 8   /* floating-point error */
#define TRAP_MCHK 9    /* machine check / hardware error */
#define TRAP_DBLFLT 10 /* double / nested fault */
#define TRAP_STACK 11  /* stack overflow / stack fault */

#define TRAP_MAX 12 /* total number of generic traps */

/* Architecture specific trap frame, all architectures must provide this */
CONTRACT struct trap_frame;

/* Trap handler for all traps, architecture-independent. */
extern void trap_handler(struct trap_frame *frame);

/* Get the trap number from the trap frame */
CONTRACT int trap_getnum(const struct trap_frame *frame);

/* Faulting virtual address for page faults and other faults */
CONTRACT virt_addr_t trap_fault_addr(void);

#endif