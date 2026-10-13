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

void interrupt_enable(void) { asm volatile("sti" : : : "memory"); }
void interrupt_disable(void) { asm volatile("cli" : : : "memory"); }

bool interrupt_is_disabled(void)
{
    return (flags_read() & EFLAGS_IF_MASK) == 0;
}

unsigned long interrupt_save(void)
{
    unsigned long flags;

    flags = flags_read() & EFLAGS_IF_MASK;
    if (flags)
        interrupt_disable();
    return flags;
}

void interrupt_restore(unsigned long flags)
{
    if (flags)
        interrupt_enable();
}