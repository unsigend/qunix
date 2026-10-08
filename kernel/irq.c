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

#include <asm/irq.h>

#include <kernel/cpu.h>
#include <kernel/errno.h>
#include <kernel/irq.h>
#include <kernel/task.h>

static struct irq_desc irq_desc[IRQ_NR];

void irq_enter(void)
{
    struct cpu *cpu;
    cpu = cpu_current();
    cpu->irq_nesting++;
}

void irq_exit(void)
{
    struct cpu *cpu;
    cpu = cpu_current();
    cpu->irq_nesting--;
    if (!cpu->irq_nesting)
        task_preempt();
}

int irq_set_chip(unsigned int irq, struct irq_chip *chip)
{
    struct irq_desc *desc;

    if (irq >= IRQ_NR || !chip)
        return -EINVAL;
    if (irq_desc[irq].chip)
        return -EBUSY;

    desc = &irq_desc[irq];
    desc->chip = chip;
    desc->count = 0;
    desc->irq = irq;
    list_init(&desc->actions);

    desc->chip->mask(irq); /* mask until the handler is registered */

    return 0;
}

struct irq_chip *irq_get_chip(unsigned int irq)
{
    if (irq >= IRQ_NR)
        return NULL;
    return irq_desc[irq].chip;
}

struct irq_desc *irq_get_desc(unsigned int irq)
{
    if (irq >= IRQ_NR)
        return NULL;
    return &irq_desc[irq];
}