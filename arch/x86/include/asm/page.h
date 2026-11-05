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

#ifndef _X86_ASM_PAGE_H_
#define _X86_ASM_PAGE_H_

#include <asm/cpu.h>

#include <kernel/compiler.h>
#include <kernel/types.h>

#define PF_P (1u << 0)    /* present */
#define PF_WR (1u << 1)   /* write */
#define PF_USER (1u << 2) /* user */
#define PF_RSVD (1u << 3) /* reserved */
#define PF_ID (1u << 4)   /* instruction fetch */
#define PF_PK (1u << 5)   /* protection key */
#define PF_SS (1u << 6)   /* shadow stack */
#define PF_SGX (1u << 15) /* SGX */

#endif /* _X86_ASM_PAGE_H_ */
