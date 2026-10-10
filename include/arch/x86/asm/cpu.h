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

#ifndef _X86_ASM_CPU_H_
#define _X86_ASM_CPU_H_

#ifdef __x86_64__
#include <asm/cpu_64.h>
#else
#include <asm/cpu_32.h>
#endif

extern void read_cpu(struct arch_cpu *cpu);

static __always_inline unsigned long read_flags(void);

static __always_inline unsigned long read_cr0(void);
static __always_inline unsigned long read_cr2(void);
static __always_inline unsigned long read_cr3(void);
static __always_inline unsigned long read_cr4(void);
#ifdef __x86_64__
static __always_inline unsigned long read_cr8(void);
#endif

static __always_inline void write_cr0(unsigned long value);
static __always_inline void write_cr2(unsigned long value);
static __always_inline void write_cr3(unsigned long value);
static __always_inline void write_cr4(unsigned long value);
#ifdef __x86_64__
static __always_inline void write_cr8(unsigned long value);
#endif

static __always_inline unsigned long read_flags(void)
{
    unsigned long flags;
    asm volatile("pushf\n\t"
                 "pop %0\n\t"
                 : "=r"(flags)
                 :
                 : "memory");
    return flags;
}

static __always_inline unsigned long read_cr0(void)
{
    unsigned long cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0) : : "memory");
    return cr0;
}

static __always_inline unsigned long read_cr2(void)
{
    unsigned long cr2;
    asm volatile("mov %%cr2, %0" : "=r"(cr2) : : "memory");
    return cr2;
}

static __always_inline unsigned long read_cr3(void)
{
    unsigned long cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3) : : "memory");
    return cr3;
}

static __always_inline unsigned long read_cr4(void)
{
    unsigned long cr4;
    asm volatile("mov %%cr4, %0" : "=r"(cr4) : : "memory");
    return cr4;
}

#ifdef __x86_64__
static __always_inline unsigned long read_cr8(void)
{
    unsigned long cr8;
    asm volatile("mov %%cr8, %0" : "=r"(cr8) : : "memory");
    return cr8;
}
#endif

static __always_inline void write_cr0(unsigned long value)
{
    asm volatile("mov %0, %%cr0" : : "r"(value) : "memory");
}

static __always_inline void write_cr2(unsigned long value)
{
    asm volatile("mov %0, %%cr2" : : "r"(value) : "memory");
}

static __always_inline void write_cr3(unsigned long value)
{
    asm volatile("mov %0, %%cr3" : : "r"(value) : "memory");
}

static __always_inline void write_cr4(unsigned long value)
{
    asm volatile("mov %0, %%cr4" : : "r"(value) : "memory");
}

#ifdef __x86_64__
static __always_inline void write_cr8(unsigned long value)
{
    asm volatile("mov %0, %%cr8" : : "r"(value) : "memory");
}
#endif

#endif /* _X86_ASM_CPU_H_ */