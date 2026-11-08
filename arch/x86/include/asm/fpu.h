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

#ifndef _X86_ASM_FPU_H_
#define _X86_ASM_FPU_H_

#include <kernel/compiler.h>

#define FPU_STATE_SIZE 512
#define FPU_STATE_ALIGN 16

struct fpu_state {
    unsigned char image[FPU_STATE_SIZE];
} __aligned(FPU_STATE_ALIGN);

extern void fpu_init(void);

extern void fpu_state_init(struct fpu_state *state);
extern void fpu_state_save(struct fpu_state *state);
extern void fpu_state_restore(struct fpu_state *state);

#endif /* _X86_ASM_FPU_H_ */