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

#ifndef _X86_ASM_MB2_H_
#define _X86_ASM_MB2_H_

#include <kernel/macros.h>
#include <kernel/mm/memlayout.h>
#include <kernel/panic.h>
#include <kernel/stdint.h>

#include <asm/multiboot2.h>

struct mb2_header {
    uint32_t total_size;
    uint32_t reserved; /* Reserved always 0*/
};

static inline void mb2_validate(uint32_t magic, uint32_t mbi)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        panic("Invalid multiboot2 header magic");
    if (!IS_ALIGNED(mbi, MULTIBOOT_INFO_ALIGN))
        panic("Invalid alignment of multiboot2 info");
}

static inline struct mb2_header *mb2_get_header(uint32_t mbi)
{
    return (struct mb2_header *)pa_to_va((phys_addr_t)mbi);
}

static inline struct multiboot_tag *mb2_get_tag(uint32_t mbi)
{
    return (struct multiboot_tag *)pa_to_va(
        (phys_addr_t)(mbi + sizeof(struct mb2_header)));
}

static inline struct multiboot_tag *mb2_next_tag(struct multiboot_tag *tag)
{
    return (struct multiboot_tag *)ROUND_UP((uint32_t)tag + tag->size,
                                            MULTIBOOT_TAG_ALIGN);
}

#endif /* _X86_ASM_MB2_H_ */