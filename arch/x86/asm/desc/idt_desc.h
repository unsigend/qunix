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

#ifndef _DESC_IDT_DESC_H_
#define _DESC_IDT_DESC_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

#ifdef __x86_64__
struct idt_entry {
    uint16_t offset_low;    /* offset [0:15] */
    uint16_t selector;      /* segment selector */
    uint16_t flags;         /* flags */
    uint16_t offset_middle; /* offset [16:31] */
    uint32_t offset_high;   /* offset [32:63] */
    uint32_t reserved;      /* reserved */
} __packed;
#else
struct idt_entry {
    uint16_t offset_low;  /* offset [0:15] */
    uint16_t selector;    /* segment selector */
    uint16_t flags;       /* flags */
    uint16_t offset_high; /* offset [16:31] */
} __packed;
#endif

#endif /* _DESC_IDT_DESC_H_ */