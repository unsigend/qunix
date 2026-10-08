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

#include <asm/irq.h> /* IRQ_NR */

#include <kernel/interrupt.h>
#include <kernel/list.h>

/**
 * Hardware interrupt chip descriptor.
 *
 * @name: chip name
 * @mask: mask interrupt line
 * @unmask: unmask interrupt line
 * @eoi: end of interrupt
 */
struct irq_chip {
    const char *name;
    void (*mask)(unsigned int irq);
    void (*unmask)(unsigned int irq);
    void (*eoi)(unsigned int irq);
};

/**
 * Interrupt action descriptor for a given interrupt handler.
 *
 * @handler: interrupt handler function
 * @flags: interrupt flags
 * @name: action name
 * @dev: cookie to identify the action
 * @action: action node in the list
 * @irq: interrupt line number
 */
struct irq_action {
    irq_handler_t handler;
    unsigned long flags;
    const char *name;
    void *dev;
    struct list_head action;
    unsigned int irq;
};

/**
 * Interrupt line descriptor represents a single interrupt line.
 *
 * @irq: interrupt line number
 * @chip: hardware chip descriptor
 * @actions: actions list
 * @count: triggered count for this interrupt line
 * @nhandlers: number of handlers registered for this interrupt line
 */
struct irq_desc {
    unsigned int irq;
    struct irq_chip *chip;
    struct list_head actions;
    unsigned long count;
    unsigned int nhandlers;
};

extern void irq_enter(void);
extern void irq_exit(void);

extern int irq_set_chip(unsigned int irq, struct irq_chip *chip);
extern struct irq_chip *irq_get_chip(unsigned int irq);
extern struct irq_desc *irq_get_desc(unsigned int irq);

#endif /* _QUNIX_IRQ_H_ */