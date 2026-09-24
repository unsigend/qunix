/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <qunix/error.h>
#include <qunix/irq.h>

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