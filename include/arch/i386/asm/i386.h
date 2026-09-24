/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_I386_H
#define ARCH_I386_ASM_I386_H

#include <asm.h>
#include <stdint.h>

/* i386 architecture specific assembly functions */

static inline void cr0_write(uint32_t value)
{
    asm volatile("movl %0, %%cr0" : : "r"(value) : "memory");
}

static inline uint32_t cr0_read(void)
{
    uint32_t value;
    asm volatile("movl %%cr0, %0" : "=r"(value));
    return value;
}

/* CR1: Reserved, the CPU will throw a #UD exception when trying to access it.*/

static inline void cr2_write(uint32_t value)
{
    asm volatile("movl %0, %%cr2" : : "r"(value) : "memory");
}

static inline uint32_t cr2_read(void)
{
    uint32_t value;
    asm volatile("movl %%cr2, %0" : "=r"(value));
    return value;
}

static inline void cr3_write(uint32_t value)
{
    asm volatile("movl %0, %%cr3" : : "r"(value) : "memory");
}

static inline uint32_t cr3_read(void)
{
    uint32_t value;
    asm volatile("movl %%cr3, %0" : "=r"(value));
    return value;
}

static inline void cr4_write(uint32_t value)
{
    asm volatile("movl %0, %%cr4" : : "r"(value) : "memory");
}

static inline uint32_t cr4_read(void)
{
    uint32_t value;
    asm volatile("movl %%cr4, %0" : "=r"(value));
    return value;
}

#endif