/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_X86_BOOT_MB2_H
#define ARCH_X86_BOOT_MB2_H

/* This file define some multiboot2 related extensions functions, reference:
 * https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html */

#include <stdint.h>

typedef uintptr_t
    multiboot_info_t; /* multiboot2 information pointer from %ebx */

extern void mb2_validate(uint32_t magic, multiboot_info_t mbi);

extern void mb2_set_mbi(multiboot_info_t mbi);
extern multiboot_info_t mb2_get_mbi(void);

extern struct multiboot_tag *mb2_next_tag(const struct multiboot_tag *tag);
extern struct multiboot_tag *mb2_find_tag(multiboot_info_t mbi, uint32_t type);
extern struct multiboot_tag *mb2_first_tag(multiboot_info_t mbi);

#endif