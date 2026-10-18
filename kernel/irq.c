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

#include <asm/irq.h> /* IRQ_NR */

#include <kernel/cpu.h>
#include <kernel/errno.h>
#include <kernel/interrupt.h>
#include <kernel/irq.h>
#include <kernel/kmalloc.h>
#include <kernel/panic.h>
#include <kernel/preempt.h>
#include <kernel/sched.h>

static struct irq_desc irq_desc[IRQ_NR];

int irq_set_chip(unsigned int irq, const struct irq_chip *chip)
{
    struct irq_desc *desc;

    if (irq >= IRQ_NR || !chip)
        return -EINVAL;

    if (irq_desc[irq].chip)
        return -EBUSY;

    desc = &irq_desc[irq];

    desc->chip = chip;
    desc->irq = irq;
    list_init(&desc->action_list);
    desc->depth = 1; /* keep disabled at start */
    desc->nhandlers = 0;
    desc->triggered = 0;
    desc->unhandled = 0;

    chip->mask(irq);

    return 0;
}

int irq_register(unsigned int irq, irq_handler_t handler, unsigned int flags,
                 const char *name, void *dev)
{
    struct irq_action *action;
    struct irq_desc *desc;

    if (irq >= IRQ_NR || !handler)
        return -EINVAL;

    desc = &irq_desc[irq];

    if (!desc->chip)
        return -ENODEV;
    if (!list_empty(
            &desc->action_list)) /* in current version even IRQF_SHARED is
                                    specified, only one handler is allowed */
        return -EBUSY;

    action = kmalloc(sizeof(struct irq_action));
    if (!action)
        return -ENOMEM;

    action->name = name;
    action->handler = handler;
    action->flags = flags;
    action->dev = dev;
    list_init(&action->action);

    list_add_tail(&desc->action_list, &action->action);
    desc->nhandlers++;

    if (desc->nhandlers == 1)
        irq_enable_line(irq);

    return 0;
}

void irq_unregister(unsigned int irq, void *dev)
{
    struct irq_desc *desc;
    struct irq_action *action;

    if (irq >= IRQ_NR)
        panic("invalid irq number %u", irq);

    desc = &irq_desc[irq];
    if (!desc->chip || desc->nhandlers == 0)
        panic("irq line %u is not registered", irq);

    if (!dev && desc->nhandlers > 1)
        panic("irq line %u is shared need dev", irq);

    list_for_each_entry(&desc->action_list, action, struct irq_action, action)
    {
        if (action->dev == dev) {
            list_del(&desc->action_list, &action->action);
            kfree(action);
            desc->nhandlers--;
            if (!desc->nhandlers)
                irq_disable_line(irq); /* last one */
            break;
        }
    }
}

void irq_enable_line(unsigned int irq)
{
    struct irq_desc *desc;

    if (irq >= IRQ_NR)
        panic("invalid irq number %u", irq);

    desc = &irq_desc[irq];
    if (!desc->chip)
        panic("irq line %u is not registered", irq);
    if (desc->depth == 0)
        panic("unbalance enable irq line %u", irq);

    desc->depth--;
    if (!desc->depth)
        desc->chip->unmask(irq);
}

void irq_disable_line(unsigned int irq)
{
    struct irq_desc *desc;

    if (irq >= IRQ_NR)
        panic("invalid irq number %u", irq);

    desc = &irq_desc[irq];
    if (!desc->chip)
        panic("irq line %u is not registered", irq);

    if (desc->depth == 0)
        desc->chip->mask(irq);

    desc->depth++;
}

bool in_interrupt(void) { return cpu_current()->irq_depth > 0; }
void irq_enter(void) { cpu_current()->irq_depth++; }
void irq_exit(void)
{
    if (cpu_current()->irq_depth == 0)
        panic("unbalance irq exit");
    cpu_current()->irq_depth--;
    if (!in_interrupt() && preemptible() && need_resched())
        schedule();
}

irqreturn_t irq_handle(unsigned int irq)
{
    struct irq_desc *desc;
    const struct irq_chip *chip;
    struct irq_action *action;
    irqreturn_t ret = IRQ_NONE, handler_ret;

    /* the irq line is always correct */
    desc = &irq_desc[irq];
    chip = desc->chip;

    desc->triggered++;

    if (!chip) {
        desc->unhandled++;
        return IRQ_NONE;
    }

    irq_enter();
    list_for_each_entry(&desc->action_list, action, struct irq_action, action)
    {
        handler_ret = action->handler(irq, action->dev);
        ret |= handler_ret;
    }

    if (ret == IRQ_NONE)
        desc->unhandled++;

    chip->eoi(irq);
    irq_exit();

    return ret;
}