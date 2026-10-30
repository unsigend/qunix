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
#include <kernel/types.h>

#define GDT_SIZE_MAX UINT16_MAX
#define GDT_LIMIT_MAX 0xFFFFF

#define GDT_P (1u << 7)                  /* present */
#define GDT_DPL(dpl) (((dpl) & 3u) << 5) /* descriptor privilege level */
#define GDT_S_CD (1u << 4)               /* code or data segment */
#define GDT_S_SYS 0                      /* system segment */
#define GDT_LONG (1u << 9)               /* 64-bit code segment */
#define GDT_DB_16 0                      /* 16-bit segment */
#define GDT_DB_32 (1u << 10)             /* 32-bit segment */
#define GDT_G_4K (1u << 11)              /* 4KB granularity */
#define GDT_G_1B 0                       /* byte granularity */

/* Code/Data segment flags */
#define GDT_EXEC (1u << 3) /* executable */
#define GDT_DC (1u << 2)   /* direction/conforming */
#define GDT_RW (1u << 1)   /* readable/writable */
#define GDT_AC (1u << 0)   /* accessed */

/* System segment flags */
#define GDT_LDT 0x02 /* local descriptor table */
#define GDT_TSS 0x09 /* task state segment */

extern void gdtr_read(unsigned short *size, unsigned long *base);
extern void gdtr_write(unsigned short size, unsigned long base);

extern void gdt_init_cpu(cpuid_t cpuid);
extern void gdt_set_entry(cpuid_t cpuid, unsigned int index, unsigned long base,
                          unsigned long limit, unsigned int flags);
extern void gdt_set_sys_entry(cpuid_t cpuid, unsigned int index,
                              unsigned long base, unsigned long limit,
                              unsigned int flags);

#endif /* _X86_ASM_GDT_H_ */