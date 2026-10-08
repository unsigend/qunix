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

#ifndef _QUNIX_CPU_H_
#define _QUNIX_CPU_H_

#include <kernel/compiler.h>

/**
 * Hardware generic CPU descriptor.
 *
 * @spinlock_nested: number of spinlock_lock() calls not yet matched by an
 *                   spinlock_unlock() call
 * @irq_ctx_nested:  number of nested interrupt contexts
 * @irq_flags:       flags for irq_save() and irq_restore() calls
 */
struct cpu {
    unsigned int spinlock_nested;

    unsigned int irq_ctx_nested;
    unsigned long irq_flags;
};

CONTRACT extern void cpu_init(void);
CONTRACT extern void cpu_halt(void) __noreturn;

CONTRACT extern struct cpu *cpu_current(void);

#endif /* _QUNIX_CPU_H_ */