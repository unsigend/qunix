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

#ifndef _QUNIX_INTERRUPT_H_
#define _QUNIX_INTERRUPT_H_

#include <kernel/compiler.h>
#include <kernel/stdbool.h>

#define IRQF_SAMPLE_RADOM (1 << 2) /* Sample random number from interrupt */
#define IRQF_SHARED                                                            \
    (1 << 3) /* Shared interrupt line among multiple                           \
                handlers */

enum irqreturn {
    IRQ_NONE = 0,    /* Interrupt not handled */
    IRQ_HANDLED = 1, /* Interrupt handled */
};

typedef enum irqreturn irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(unsigned int irq, void *dev);

/* Disable or enable cpu local interrupts. These functions are not nested
 * safe, an interrupt may occur between the enable and disable calls. For
 * nested interrupts calls, use irq_save() and irq_restore() instead. */
CONTRACT extern void irq_enable(void);
CONTRACT extern void irq_disable(void);

/* Enable or disable a specific interrupt line. Nested safe, only the outermost
 * irq_enable_line() will enable the interrupt line back. */
extern void irq_enable_line(unsigned int irq);
extern void irq_disable_line(unsigned int irq);

/* Check if cpu local interrupts are disabled. Return true if disabled, false
 * otherwise. */
CONTRACT extern bool irq_is_disabled(void);

/* Save and restore the current cpu local interrupts state. Disable and enable
 * interrupts only for the outermost irq_save() and irq_restore() calls. Nested
 * calls are safe. */
CONTRACT extern unsigned long irq_save(void);
CONTRACT extern void irq_restore(unsigned long flags);

/* Return true if the current execution is in an interrupt context. False
 * otherwise. */
extern bool in_interrupt(void);

/**
 * Register an interrupt handler for a given IRQ line.
 *
 * @irq: IRQ line number
 * @handler: Interrupt handler function
 * @flags: Interrupt flags
 * @name: Interrupt name
 * @dev: Device pointer
 *
 * @return: 0 on success, -ERRNO on failure
 */
extern int irq_register(unsigned int irq, irq_handler_t handler,
                        unsigned long flags, const char *name, void *dev);

extern void irq_unregister(unsigned int irq, void *dev);

#endif /* _QUNIX_INTERRUPT_H_ */