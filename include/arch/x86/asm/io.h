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

#ifndef _X86_ASM_IO_H_
#define _X86_ASM_IO_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

static __always_inline void outb(uint16_t port, uint8_t value);
static __always_inline void outw(uint16_t port, uint16_t value);
static __always_inline void outl(uint16_t port, uint32_t value);
static __always_inline uint8_t inb(uint16_t port);
static __always_inline uint16_t inw(uint16_t port);
static __always_inline uint32_t inl(uint16_t port);

/* Wait a very small amount of time, used as a small delay for old hardware */
static __always_inline void io_wait(void);

static __always_inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static __always_inline void outw(uint16_t port, uint16_t value)
{
    asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

static __always_inline void outl(uint16_t port, uint32_t value)
{
    asm volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

static __always_inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static __always_inline uint16_t inw(uint16_t port)
{
    uint16_t value;
    asm volatile("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static __always_inline uint32_t inl(uint16_t port)
{
    uint32_t value;
    asm volatile("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static __always_inline void io_wait(void) { outb(0x80, 0); }

#endif /* _X86_ASM_IO_H_ */