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

#include <asm/fpu.h>

#include <kernel/printk.h>
#include <kernel/string.h>

static char fpu_template[FPU_STATE_SIZE] __aligned(FPU_STATE_ALIGN);

void fpu_init(void)
{
    asm volatile("fninit" ::: "memory");
    asm volatile("fxsave %0" : "=m"(fpu_template)::"memory");

    LOGM_INFO("FPU", "FPU initialized successfully");
}

void fpu_state_init(struct fpu_state *state)
{
    memcpy(state, &fpu_template, FPU_STATE_SIZE);
}

void fpu_state_save(struct fpu_state *state)
{
    asm volatile("fxsave %0" : "=m"(state->image)::"memory");
}

void fpu_state_restore(struct fpu_state *state)
{
    asm volatile("fxrstor %0" : : "m"(state->image));
}