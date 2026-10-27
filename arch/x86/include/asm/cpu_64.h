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

#ifndef _X86_ASM_CPU_64_H_
#define _X86_ASM_CPU_64_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

struct arch_cpu {
    uint64_t rax; /* accumulator */
    uint64_t rbx; /* base */
    uint64_t rcx; /* counter */
    uint64_t rdx; /* data */

    uint64_t rsi; /* source */
    uint64_t rdi; /* destination */
    uint64_t rsp; /* stack pointer */
    uint64_t rbp; /* stack base pointer */

    uint64_t r8, r9, r10, r11, r12, r13, r14, r15; /* extended registers */

    uint64_t rip;    /* instruction pointer */
    uint64_t rflags; /* flags */

    uint16_t cs; /* code segment */
    uint16_t ds; /* data segment */
    uint16_t ss; /* stack segment */
    uint16_t es; /* extra segment */
    uint16_t fs; /* extra segment */
    uint16_t gs; /* extra segment */

    uint64_t cr0; /* general control register */
    /* cr1 reserved */
    uint64_t cr2; /* page fault address */
    uint64_t cr3; /* page directory pointer */
    uint64_t cr4; /* general control register */
    /* cr5, cr6, cr7 reserved */
    uint64_t cr8; /* task priority register */
} __packed;

#endif /* _X86_ASM_CPU_64_H_ */