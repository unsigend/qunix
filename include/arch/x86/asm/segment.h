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

#ifndef _X86_ASM_SEGMENT_H_
#define _X86_ASM_SEGMENT_H_

#include <asm/cpu.h>

#define SEGSEL_KCODE_IDX 1
#define SEGSEL_KDATA_IDX 2
#define SEGSEL_UCODE_IDX 3
#define SEGSEL_UDATA_IDX 4
#define SEGSEL_TSS_IDX 5

#define SEGSEL_MAKE_GDT(idx, rpl) ((idx) << 3 | (rpl))
#define SEGSEL_MAKE_LDT(idx, rpl) ((idx) << 3 | (rpl) | 0x04)

#define SEGSEL_NULL SEGSEL_MAKE_GDT(0, 0)
#define SEGSEL_KCODE SEGSEL_MAKE_GDT(SEGSEL_KCODE_IDX, CPU_RING0)
#define SEGSEL_KDATA SEGSEL_MAKE_GDT(SEGSEL_KDATA_IDX, CPU_RING0)
#define SEGSEL_UCODE SEGSEL_MAKE_GDT(SEGSEL_UCODE_IDX, CPU_RING3)
#define SEGSEL_UDATA SEGSEL_MAKE_GDT(SEGSEL_UDATA_IDX, CPU_RING3)
#define SEGSEL_TSS SEGSEL_MAKE_GDT(SEGSEL_TSS_IDX, CPU_RING0)

#endif /* _X86_ASM_SEGMENT_H_ */