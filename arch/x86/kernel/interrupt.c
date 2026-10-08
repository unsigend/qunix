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

#include <asm/cpu.h>

#include <kernel/interrupt.h>

void irq_enable(void) { asm volatile("sti" ::: "memory"); }
void irq_disable(void) { asm volatile("cli" ::: "memory"); }
bool irq_is_disabled(void) { return (read_flags() & FLAGS_IF_MASK) == 0U; }

unsigned long irq_save(void)
{
    unsigned long flags;
    flags = read_flags() & FLAGS_IF_MASK;
    if (flags)
        irq_disable();
    return flags;
}

void irq_restore(unsigned long flags)
{
    if (flags)
        irq_enable();
}
