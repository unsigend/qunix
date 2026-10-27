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

#ifndef _X86_ASM_CPU_32_H_
#define _X86_ASM_CPU_32_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

struct arch_cpu {
    uint32_t eax; /* accumulator */
    uint32_t ebx; /* base */
    uint32_t ecx; /* counter */
    uint32_t edx; /* data */

    uint32_t esi; /* source */
    uint32_t edi; /* destination */
    uint32_t esp; /* stack pointer */
    uint32_t ebp; /* stack base pointer */

    uint32_t eip;    /* instruction pointer */
    uint32_t eflags; /* flags */

    uint16_t cs; /* code segment */
    uint16_t ds; /* data segment */
    uint16_t ss; /* stack segment */
    uint16_t es; /* extra segment */
    uint16_t fs; /* extra segment */
    uint16_t gs; /* extra segment */

    uint32_t cr0; /* general control register */
    /* cr1 reserved */
    uint32_t cr2; /* page fault address */
    uint32_t cr3; /* page directory pointer */
    uint32_t cr4; /* general control register */
    /* cr5, cr6, cr7 reserved */
} __packed;

#endif /* _X86_ASM_CPU_32_H_ */