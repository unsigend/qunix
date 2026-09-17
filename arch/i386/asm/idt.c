/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <asm/idt.h>

void idtr_write(const struct idtr *idtr)
{
    asm volatile("lidt %0" : : "m"(*idtr));
}

void idtr_read(struct idtr *idtr) { asm volatile("sidt %0" : "=m"(*idtr)); }

void idt_entry_set(struct idt_entry *entry, uint32_t offset, uint16_t selector,
                   uint8_t flags)
{
    entry->offset_low = (offset & 0xFFFF);
    entry->selector = selector;
    entry->reserved = 0; /* always 0 */
    entry->flags = flags;
    entry->offset_high = (offset >> 16) & 0xFFFF;
}

void idt_init(void) { /* TODO: Implement this */ }