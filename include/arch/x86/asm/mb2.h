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

#ifndef _ASM_X86_MB2_H_
#define _ASM_X86_MB2_H_

/* This file define some multiboot2 related extensions functions, reference:
 * https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html */

#include <kernel/stdint.h>

typedef uintptr_t
    multiboot_info_t; /* multiboot2 information pointer from %ebx */

extern void mb2_validate(uint32_t magic, multiboot_info_t mbi);

extern void mb2_set_mbi(multiboot_info_t mbi);
extern multiboot_info_t mb2_get_mbi(void);

extern struct multiboot_tag *mb2_next_tag(const struct multiboot_tag *tag);
extern struct multiboot_tag *mb2_find_tag(multiboot_info_t mbi, uint32_t type);
extern struct multiboot_tag *mb2_first_tag(multiboot_info_t mbi);

#endif /* _ASM_X86_MB2_H_ */