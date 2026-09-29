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

#include <kernel/compiler.h>
#include <kernel/stdint.h>

typedef void (*irq_handler_t)(void *);

extern int irq_register(uint32_t irq, irq_handler_t handler, void *data);
extern void irq_dispatch(uint32_t irq);

CONTRACT extern void irq_eoi(uint32_t irq); /* End of Interrupt */

#endif /* _QUNIX_IRQ_H_ */