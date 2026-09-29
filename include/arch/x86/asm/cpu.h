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

#ifndef _ASM_X86_CPU_H_
#define _ASM_X86_CPU_H_

#include <kernel/compiler.h>

static __always_inline void cr0_write(unsigned long value)
{
    asm volatile("mov %0, %%cr0" : : "r"(value) : "memory");
}

static __always_inline unsigned long cr0_read(void)
{
    unsigned long value;
    asm volatile("mov %%cr0, %0" : "=r"(value));
    return value;
}

/* CR1: Reserved, the CPU will throw a #UD exception when trying to access it.*/

static __always_inline void cr2_write(unsigned long value)
{
    asm volatile("mov %0, %%cr2" : : "r"(value) : "memory");
}

static __always_inline unsigned long cr2_read(void)
{
    unsigned long value;
    asm volatile("mov %%cr2, %0" : "=r"(value));
    return value;
}

static __always_inline void cr3_write(unsigned long value)
{
    asm volatile("mov %0, %%cr3" : : "r"(value) : "memory");
}

static __always_inline unsigned long cr3_read(void)
{
    unsigned long value;
    asm volatile("mov %%cr3, %0" : "=r"(value));
    return value;
}

static __always_inline void cr4_write(unsigned long value)
{
    asm volatile("mov %0, %%cr4" : : "r"(value) : "memory");
}

static __always_inline unsigned long cr4_read(void)
{
    unsigned long value;
    asm volatile("mov %%cr4, %0" : "=r"(value));
    return value;
}

static __always_inline void cli(void) { asm volatile("cli"); }
static __always_inline void sti(void) { asm volatile("sti"); }

#endif /* _ASM_X86_CPU_H_ */
