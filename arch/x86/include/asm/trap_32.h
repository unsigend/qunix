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

#ifndef _X86_ASM_TRAP_32_H_
#define _X86_ASM_TRAP_32_H_

#define TRAP_STUB_ALIGN 16
#define TRAP_STUB_SIZE TRAP_STUB_ALIGN

#ifndef __ASSEMBLER__

#include <kernel/compiler.h>
#include <kernel/stdint.h>

struct trap_frame {
    /* registers pushed by pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp; /* unused */
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;

    uint32_t trapnum;

    /* pushed by CPU */
    uint32_t err;
    uint32_t eip;
    uint16_t cs;
    uint16_t padding5;
    uint32_t eflags;

    /* cross rings */
    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
} __packed;

#endif /* __ASSEMBLER__ */

#endif /* _X86_ASM_TRAP_32_H_ */