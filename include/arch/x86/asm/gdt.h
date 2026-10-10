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

#define GDT_SIZE_MAX UINT16_MAX
#define GDT_LIMIT_MAX 0xFFFFF

#define GDT_CODE (1u << 3)               /* code segment */
#define GDT_DATA 0                       /* data segment */
#define GDT_LDT 0x02                     /* local descriptor table */
#define GDT_TSS 0x09                     /* task state segment */
#define GDT_DPL(dpl) (((dpl) & 3u) << 5) /* descriptor privilege level */

extern void gdtr_read(unsigned short *size, unsigned long *base);
extern void gdtr_write(unsigned short size, unsigned long base);

/**
 * Set the entry of the GDT at the given index. Used for data and code segments.
 *
 * @index: index of the entry to set.
 * @base: base address of the entry.
 * @limit: limit of the entry.
 * @flags: flags of the entry (GDT_CODE, GDT_DATA, GDT_DPL)
 */
extern void gdt_set_entry(unsigned int index, unsigned long base,
                          unsigned long limit, unsigned int flags);

/**
 * Set the entry of the GDT at the given index. Used for system segments (task
 * state segment, local descriptor table).
 *
 * @index: index of the entry to set.
 * @base: base address of the entry.
 * @limit: limit of the entry.
 * @flags: flags of the entry (GDT_LDT, GDT_TSS, GDT_DPL)
 */
extern void gdt_set_sys_entry(unsigned int index, unsigned long base,
                              unsigned long limit, unsigned int flags);

#endif /* _X86_ASM_GDT_H_ */