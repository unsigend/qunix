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

#ifndef _DESC_GDT_DESC_H_
#define _DESC_GDT_DESC_H_

#include <kernel/compiler.h>
#include <kernel/stdint.h>

#define GDT_NR 12

struct gdt_entry {
    uint16_t limit_low;  /* limit [0:15] */
    uint16_t base_low;   /* base [0:15] */
    uint8_t base_middle; /* base [16:23] */
    uint8_t access;      /* access flags */
    uint8_t flags;       /* flags & limit [16:19] */
    uint8_t base_high;   /* base [24:31] */
} __packed;

#ifdef __x86_64__
struct gdt_sys_entry {
    uint16_t limit_low;   /* limit [0:15] */
    uint16_t base_low;    /* base [0:15] */
    uint8_t base_middle1; /* base [16:23] */
    uint8_t access;       /* access flags */
    uint8_t flags;        /* flags & limit [16:19] */
    uint8_t base_middle2; /* base [24:31] */
    uint32_t base_high;   /* base [32:63] */
    uint32_t reserved;    /* reserved */
} __packed;
#else
#define gdt_sys_entry gdt_entry
#endif

#endif /* _DESC_GDT_DESC_H_ */