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
#define GDT_LIMIT_MAX 0xFFFFF

#define GDT_CODE (1u << 3) /* code segment */
#define GDT_DATA 0         /* data segment */

#define GDT_LDT 0x02 /* local descriptor table */
#define GDT_TSS 0x09 /* task state segment */

#define GDT_DPL(dpl) (((dpl) & 3u) << 5) /* descriptor privilege level */

extern void gdt_init(void);
extern void gdtr_read(unsigned short *size, unsigned long *base);
extern void gdtr_write(unsigned short size, unsigned long base);

/*
 * gdt_set_entry - write a code or data descriptor at @index.
 *
 * An out-of-range @index or @limit panics.
 */
extern void gdt_set_entry(unsigned int index, unsigned long base,
                          unsigned long limit, unsigned int flags);

/*
 * gdt_set_sys_entry - write a system descriptor at @index.
 *
 * Used for a TSS or an LDT. An out-of-range @index or @limit panics.
 */
extern void gdt_set_sys_entry(unsigned int index, unsigned long base,
                              unsigned long limit, unsigned int flags);

#endif /* _X86_ASM_GDT_H_ */