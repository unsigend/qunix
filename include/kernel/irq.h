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

#ifndef _QUNIX_IRQ_H_
#define _QUNIX_IRQ_H_

#include <kernel/interrupt.h>
#include <kernel/list.h>
#include <kernel/spinlock.h>

struct irq_chip {
    const char *name;

    void (*mask)(unsigned int irq);
    void (*unmask)(unsigned int irq);
    void (*eoi)(unsigned int irq);
};

struct irq_desc {
    const struct irq_chip *chip;
    unsigned int irq;
    struct list_head action_list;
    unsigned int depth; /* nested irq disable line depth */
    unsigned int nhandlers;
    unsigned long triggered;
    unsigned long unhandled;
    struct spinlock lock;
};

struct irq_action {
    const char *name;
    irq_handler_t handler;
    unsigned int flags;
    void *dev;
    struct list_node action;
};

extern irqreturn_t irq_handle(unsigned int irq);
extern int irq_set_chip(unsigned int irq, const struct irq_chip *chip);

extern void irq_enter(void);
extern void irq_exit(void);

CONTRACT extern void irq_init(void);

#endif /* _QUNIX_IRQ_H_ */