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

#ifndef _ASM_X86_I8253_H_
#define _ASM_X86_I8253_H_

/* Intel 8253 Programmable Interval Timer (PIT) */

#include <kernel/stdint.h>

/* Initialize the 8253 programmable interval timer with the given frequency in
 * Hz, return 0 on success, -errno on failure */
extern int i8253_init(uint32_t hz);

#endif /* _ASM_X86_I8253_H_ */