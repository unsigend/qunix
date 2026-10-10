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

#ifndef _X86_ASM_GDT_H_
#define _X86_ASM_GDT_H_

#include <kernel/stdint.h>

#define GDT_SIZE_MAX UINT16_MAX

#define GDT_A (1u << 0)   /* accessed */
#define GDT_RW (1u << 1)  /* code: readable; data: writable */
#define GDT_DC (1u << 2)  /* code: conforming; data: expand-down */
#define GDT_EXE (1u << 3) /* executable */
#define GDT_S (1u << 4)   /* non-system segment */
#define GDT_DPL(dpl) (((dpl) & 3u) << 5) /* descriptor privilege level */
#define GDT_P (1u << 7)                  /* present */

#define GDT_CODE (GDT_S | GDT_EXE | GDT_RW) /* code segment */
#define GDT_DATA (GDT_S | GDT_RW)           /* data segment */

#define GDT_LONG (1u << 5)  /* 64-bit mode D/B must clear */
#define GDT_DB (1u << 6)    /* 32-bit segment */
#define GDT_G_4KB (1u << 7) /* 4KB granularity in limit */

#define GDT_BITS32 (GDT_DB | GDT_G_4KB)   /* 32 bit segment */
#define GDT_BITS64 (GDT_LONG | GDT_G_4KB) /* 64 bit segment */

/**
 * Set the entry of the GDT at the given index.
 *
 * @index: index of the entry to set.
 * @base: base address of the entry.
 * @limit: limit of the entry.
 * @access: access flags of the entry (P, DPL, S, E, RC, RW, A).
 * @flags: flag of the entry (G, D/B, L)
 */
extern void gdt_set_entry(unsigned int index, unsigned long base,
                          unsigned long limit, unsigned int access,
                          unsigned int flags);

extern void gdtr_read(unsigned short *size, unsigned long *base);
extern void gdtr_write(unsigned short size, unsigned long base);

#endif /* _X86_ASM_GDT_H_ */