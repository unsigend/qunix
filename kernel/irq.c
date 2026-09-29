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

#include <kernel/errno.h>
#include <kernel/irq.h>

#define MAX_IRQS 64

struct irq_context {
    irq_handler_t handler;
    void *data;
};

static struct irq_context irq_contexts[MAX_IRQS];

int irq_register(uint32_t irq, irq_handler_t handler, void *data)
{
    if (irq >= MAX_IRQS)
        return -EINVAL;

    irq_contexts[irq].handler = handler;
    irq_contexts[irq].data = data;

    return 0;
}

void irq_dispatch(uint32_t irq)
{
    if (irq >= MAX_IRQS)
        return;

    if (irq_contexts[irq].handler)
        irq_contexts[irq].handler(irq_contexts[irq].data);

    irq_eoi(irq);
}