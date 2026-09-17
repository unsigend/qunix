/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_IDT_H
#define ARCH_I386_ASM_IDT_H

#define GATE_TYPE_TASK 0x05   /* Task gate */
#define GATE_TYPE_INT16 0x06  /* 16-bit interrupt gate */
#define GATE_TYPE_TRAP16 0x07 /* 16-bit trap gate */
#define GATE_TYPE_INT32 0x0E  /* 32-bit interrupt gate */
#define GATE_TYPE_TRAP32 0x0F /* 32-bit trap gate */

#define GATE_DPL_RING0 0 /* Ring 0 */
#define GATE_DPL_RING1 1 /* Ring 1 */
#define GATE_DPL_RING2 2 /* Ring 2 */
#define GATE_DPL_RING3 3 /* Ring 3 */

#ifndef ASM_FILE

#include <stdint.h>

struct idt_entry {
    uint16_t offset_low;  /* offset[0:15] */
    uint16_t selector;    /* selector */
    uint8_t reserved;     /* reserved */
    uint8_t flags;        /* P, DPL, Gate type */
    uint16_t offset_high; /* offset[16:31] */
} __attribute__((packed));

struct idtr {
    uint16_t size; /* the size of the IDT table in bytes subtracted by 1 */
    uint32_t base; /* base address of the IDT */
} __attribute__((packed));

extern void idt_init(void);
extern void idt_entry_set(struct idt_entry *entry, uint32_t offset,
                          uint16_t selector, uint8_t flags);
extern void idtr_write(const struct idtr *idtr);
extern void idtr_read(struct idtr *idtr);

/* Macros for member access for IDT entries */
#define GATE_GET_P(entry) (((entry)->flags & 0x80) >> 7)
#define GATE_GET_DPL(entry) (((entry)->flags & 0x60) >> 5)
#define GATE_GET_TYPE(entry) (((entry)->flags & 0x0F))
#define GATE_GET_OFFSET(entry)                                                 \
    (((uint32_t)(entry)->offset_high << 16) | (entry)->offset_low)

#define GATE_MAKE_FLAGS(p, dpl, type) (((p) << 7) | ((dpl) << 5) | (type))

#endif

#endif