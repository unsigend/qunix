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

#include <kernel/cpu.h>
#include <kernel/errno.h>
#include <kernel/interrupt.h>
#include <kernel/irq.h>
#include <kernel/kmalloc.h>

bool in_interrupt(void)
{
    struct cpu *cpu;
    cpu = cpu_current();
    return cpu->irq_nesting > 0;
}

int irq_register(unsigned int irq, irq_handler_t handler, unsigned long flags,
                 const char *name, void *dev)
{
    struct irq_desc *desc;
    struct irq_action *action;

    if (irq >= IRQ_NR || !handler)
        return -EINVAL;
    desc = irq_get_desc(irq);
    if (!desc || !desc->chip)
        return -ENODEV;
    if (!list_empty(&desc->actions))
        return -EBUSY; /* No shared IRQ lines support yet */

    action = kmalloc(sizeof(struct irq_action));
    if (!action)
        return -ENOMEM;

    action->handler = handler;
    action->flags = flags;
    action->name = name;
    action->dev = dev;
    list_init(&action->action);
    action->irq = irq;

    /* add to the end of the list */
    list_add_tail(&desc->actions, &action->action);
    desc->nhandlers++;
    if (desc->nhandlers == 1)
        desc->chip->unmask(irq); /* unmask if first handler */

    return 0;
}

void irq_unregister(unsigned int irq, void *dev)
{
    struct irq_desc *desc;
    struct irq_action *action;
    struct list_head *pos;

    if (irq >= IRQ_NR)
        return;
    desc = irq_get_desc(irq);
    if (!desc || !desc->chip)
        return;
    if (list_empty(&desc->actions))
        return;

    list_for_each(&desc->actions, pos)
    {
        action = list_entry(pos, struct irq_action, action);
        if (action->dev == dev) {
            list_del(&action->action);
            desc->nhandlers--;
            if (desc->nhandlers == 0)
                desc->chip->mask(irq); /* mask if no handlers */
            kfree(action);
            break;
        }
    }
}