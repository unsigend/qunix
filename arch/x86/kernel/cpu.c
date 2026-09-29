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
#include <asm/gdt.h>
#include <asm/idt.h>

#include <kernel/cpu.h>
#include <kernel/printk.h>

void cpu_halt(void)
{
    for (;;)
        asm volatile("hlt");
}

void cpu_enable_interrupts(void) { asm volatile("sti"); }
void cpu_disable_interrupts(void) { asm volatile("cli"); }

unsigned long cpu_save_interrupts(void)
{
    unsigned long flags = read_flags() & FLAGS_IF_MASK;
    cpu_disable_interrupts();
    return flags;
}

void cpu_restore_interrupts(unsigned long flags)
{
    if (flags & FLAGS_IF_MASK)
        cpu_enable_interrupts();
}

void cpu_init(void)
{
    gdt_init();
    idt_init();

    LOGM(LOG_LEVEL_INFO, "CPU", "CPU initialized successfully");
}