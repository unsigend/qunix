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

#ifndef _X86_ASM_TRAP_H_
#define _X86_ASM_TRAP_H_

#ifdef __x86_64__
#include <asm/trap_64.h>
#else
#include <asm/trap_32.h>
#endif
#include <asm/trapnr.h>

#ifndef __ASSEMBLER__

extern void trap_entry(void);
extern void trap_handle(struct trap_frame *frame);
extern unsigned long trap_stubs(unsigned int nr);

#endif /* __ASSEMBLER__ */

#endif /* _X86_ASM_TRAP_H_ */