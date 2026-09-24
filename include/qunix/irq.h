/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef QUNIX_IRQ_H
#define QUNIX_IRQ_H

#include <qunix/macros.h>
#include <stdint.h>

typedef void (*irq_handler_t)(void *);

extern int irq_register(uint32_t irq, irq_handler_t handler, void *data);
extern void irq_dispatch(uint32_t irq);

CONTRACT void irq_eoi(uint32_t irq); /* End of Interrupt */

#endif