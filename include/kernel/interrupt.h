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

enum irqreturn {
    IRQ_NONE = 0,
    IRQ_HANDLED = 1,
};

#define IRQF_SHARED (1u << 0) /* shared handler in one line */

typedef enum irqreturn irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(unsigned int irq, void *dev);

CONTRACT extern void interrupt_enable(void);
CONTRACT extern void interrupt_disable(void);
CONTRACT extern bool interrupt_is_disabled(void);

CONTRACT extern unsigned long interrupt_save(void);
CONTRACT extern void interrupt_restore(unsigned long flags);

extern int irq_register(unsigned int irq, irq_handler_t handler,
                        unsigned int flags, const char *name, void *dev);
extern void irq_unregister(unsigned int irq, void *dev);

extern void irq_enable_line(unsigned int irq);
extern void irq_disable_line(unsigned int irq);

extern bool in_interrupt(void);

#endif /* _QUNIX_INTERRUPT_H_ */