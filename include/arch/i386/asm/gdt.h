/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_GDT_H
#define ARCH_I386_ASM_GDT_H

/* Global Descriptor Table for i386 architecture, for all macros the naming
 * convention is: SEG for segment descriptor and SEGS for segment
 * selector.
 * Reference: https://wiki.osdev.org/Global_Descriptor_Table.
 */

#define SEG_MAX_ENTRIES 5 /* Maximum number of entries in the GDT */

#define SEG_NULL_IDX 0      /* Null segment */
#define SEG_KER_CODE_IDX 1  /* Kernel code segment */
#define SEG_KER_DATA_IDX 2  /* Kernel data segment */
#define SEG_USER_CODE_IDX 3 /* User code segment */
#define SEG_USER_DATA_IDX 4 /* User data segment */

#define SEG_DPL_RING0 0 /* Ring 0 */
#define SEG_DPL_RING1 1 /* Ring 1 */
#define SEG_DPL_RING2 2 /* Ring 2 */
#define SEG_DPL_RING3 3 /* Ring 3 */

#define SEG_S_SYSTEM 0   /* System */
#define SEG_S_CODEDATA 1 /* Code/Data */

#define SEG_DATA_RD 0x00        /* Read-Only */
#define SEG_DATA_RDA 0x01       /* Read-Only, accessed */
#define SEG_DATA_RDWR 0x02      /* Read/Write */
#define SEG_DATA_RDWRA 0x03     /* Read/Write, accessed */
#define SEG_DATA_RDEXPD 0x04    /* Read-Only, expand-down */
#define SEG_DATA_RDEXPDA 0x05   /* Read-Only, expand-down, accessed */
#define SEG_DATA_RDWREXPD 0x06  /* Read/Write, expand-down */
#define SEG_DATA_RDWREXPDA 0x07 /* Read/Write, expand-down, accessed */
#define SEG_CODE_EX 0x08        /* Execute-Only */
#define SEG_CODE_EXA 0x09       /* Execute-Only, accessed */
#define SEG_CODE_EXRD 0x0A      /* Execute/Read */
#define SEG_CODE_EXRDA 0x0B     /* Execute/Read, accessed */
#define SEG_CODE_EXC 0x0C       /* Execute-Only, conforming */
#define SEG_CODE_EXCA 0x0D      /* Execute-Only, conforming, accessed */
#define SEG_CODE_EXRDC 0x0E     /* Execute/Read, conforming */
#define SEG_CODE_EXRDCA 0x0F    /* Execute/Read, conforming, accessed */

#define SEG_G_1B 0  /* Granularity: 1 byte */
#define SEG_G_4KB 1 /* Granularity: 4 KB */

#define SEG_DB_16 0 /* 16-bit segment */
#define SEG_DB_32 1 /* 32-bit segment */

#define SEG_LONG_MODE 1 /* Long mode mutual exclusion with DB flag */

#define SEGS_TI_GDT 0 /* Global Descriptor Table */
#define SEGS_TI_LDT 1 /* Local Descriptor Table */

#define SEGS_RPL_RING0 0 /* Ring 0 */
#define SEGS_RPL_RING1 1 /* Ring 1 */
#define SEGS_RPL_RING2 2 /* Ring 2 */
#define SEGS_RPL_RING3 3 /* Ring 3 */

#ifndef ASM_FILE

#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;  /* Limit[0:15] */
    uint16_t base_low;   /* Base[0:15] */
    uint8_t base_mid;    /* Base[16:23] */
    uint8_t access;      /* P, DPL, S, Type */
    uint8_t granularity; /* G, D/B, L, AVL, Limit[16:19] */
    uint8_t base_high;   /* Base[24:31] */
} __attribute__((packed));

struct gdtr {
    uint16_t size; /* The size of the GDT table in bytes subtracted by 1 */
    uint32_t base; /* Base address of the GDT */
} __attribute__((packed));

typedef uint16_t seg_selector_t;

extern void gdt_init(void);
extern void gdt_entry_set(struct gdt_entry *entry, uint32_t base,
                          uint32_t limit, uint8_t flags, uint8_t access);
extern void gdtr_write(const struct gdtr *gdtr);
extern void gdtr_read(struct gdtr *gdtr);

/* Macros for member access for GDT entries */
#define SEG_GET_P(entry) (((entry)->access & 0x80) >> 7)
#define SEG_GET_DPL(entry) (((entry)->access & 0x60) >> 5)
#define SEG_GET_S(entry) (((entry)->access & 0x10) >> 4)
#define SEG_GET_E(entry) (((entry)->access & 0x08) >> 3)
#define SEG_GET_DC(entry) (((entry)->access & 0x04) >> 2)
#define SEG_GET_RW(entry) (((entry)->access & 0x02) >> 1)
#define SEG_GET_ACCESS(entry) (((entry)->access & 0x01))
#define SEG_GET_TYPE(entry) (((entry)->access & 0x0F))

#define SEG_GET_G(entry) (((entry)->granularity & 0x80) >> 7)
#define SEG_GET_DB(entry) (((entry)->granularity & 0x40) >> 6)
#define SEG_GET_L(entry) (((entry)->granularity & 0x20) >> 5)
#define SEG_GET_AVL(entry) (((entry)->granularity & 0x10) >> 4)

#define SEG_GET_BASE(entry)                                                    \
    (((uint32_t)(entry)->base_high << 24) |                                    \
     ((uint32_t)(entry)->base_mid << 16) | (entry)->base_low)
#define SEG_GET_LIMIT(entry)                                                   \
    ((((entry)->granularity & 0x0F) << 16) | (entry)->limit_low)

/* Macros for member access for segment selectors */
#define SEGS_GET_IDX(selector) ((selector) >> 3)
#define SEGS_GET_TI(selector) (((selector) & 0x04) >> 2)
#define SEGS_GET_RPL(selector) ((selector) & 0x03)

#define SEGS_MAKE(idx, ti, rpl) (((idx) << 3) | ((ti) << 2) | (rpl))

#endif

#endif