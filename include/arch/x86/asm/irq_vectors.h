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

#ifndef _X86_ASM_IRQ_VECTORS_H_
#define _X86_ASM_IRQ_VECTORS_H_

/* Asynchronous exceptions from external hardware. */

#define X86_VECTOR_NR 16
#define X86_VECTOR_NR_MIN 0x20
#define X86_VECTOR_NR_MAX (X86_VECTOR_NR_MIN + X86_VECTOR_NR - 1)

#endif /* _X86_ASM_IRQ_VECTORS_H_ */