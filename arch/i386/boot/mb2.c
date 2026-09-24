/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/mb2.h>
#include <boot/multiboot2.h>
#include <qunix/kernel.h>
#include <stddef.h>
#include <stdint.h>

static multiboot_info_t mb2_info = 0;

struct multiboot_base {
    uint32_t total_size; /* total size of the boot information including this
                            field and terminating tag in bytes */
    uint32_t reserved;   /* always set to 0 and must be ignored by OS */
};

void mb2_set_mbi(multiboot_info_t mbi) { mb2_info = mbi; }
multiboot_info_t mb2_get_mbi(void) { return mb2_info; }

void mb2_validate(uint32_t magic, multiboot_info_t mbi)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        panic("invalid multiboot2 magic number: 0x%x\n", magic);
    if (!IS_ALIGNED(mbi, MULTIBOOT_INFO_ALIGN))
        panic("invalid alignment of multiboot2 information: 0x%zx\n", mbi);
}

struct multiboot_tag *mb2_next_tag(const struct multiboot_tag *tag)
{
    return (struct multiboot_tag *)ALIGN((uintptr_t)tag + tag->size,
                                         MULTIBOOT_TAG_ALIGN);
}

struct multiboot_tag *mb2_first_tag(multiboot_info_t mbi)
{
    return (struct multiboot_tag *)((uintptr_t)mbi +
                                    sizeof(struct multiboot_base));
}

struct multiboot_tag *mb2_find_tag(multiboot_info_t mbi, uint32_t type)
{
    struct multiboot_tag *tag = NULL;
    struct multiboot_base *base = (struct multiboot_base *)mbi;
    uintptr_t raw = (uintptr_t)base + sizeof(struct multiboot_base);
    uintptr_t end = (uintptr_t)base + base->total_size;

    while (raw < end) {
        struct multiboot_tag *cur = (struct multiboot_tag *)raw;
        if (cur->type == type) {
            tag = cur;
            break;
        }
        raw = ALIGN(raw + cur->size, MULTIBOOT_TAG_ALIGN);
    }
    return tag;
}