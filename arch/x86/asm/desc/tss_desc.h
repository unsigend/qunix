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

#ifndef _DESC_TSS_DESC_H_
#define _DESC_TSS_DESC_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

#ifdef __x86_64__
struct tss_entry {
} __packed;
#else
struct tss_entry {
    uint16_t link; /* segment selector for the TSS of previous task */
    uint16_t reserved1;
    uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved2;
    uint32_t esp1;
    uint16_t ss1;
    uint16_t reserved3;
    uint32_t esp2;
    uint16_t ss2;
    uint16_t reserved4;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;
    uint16_t padding1;
    uint16_t cs;
    uint16_t padding2;
    uint16_t ss;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    uint16_t fs;
    uint16_t padding5;
    uint16_t gs;
    uint16_t padding6;
    uint16_t ldt;
    uint16_t padding7;
    uint16_t reserved5;
    uint16_t iopb; /* I/O map base address */
} __packed;
#endif /* __x86_64__ */

#endif /* _DESC_TSS_DESC_H_ */