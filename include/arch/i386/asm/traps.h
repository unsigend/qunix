/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_TRAPS_H
#define ARCH_I386_ASM_TRAPS_H

#include <asm/trapnum.h>

#ifndef ASM_FILE

#include <stdint.h>

struct trap_frame {
    /* registers are pushed by pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t oesp; /* old ESP useless & ignored */
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint16_t gs;
    uint16_t padding1;
    uint16_t fs;
    uint16_t padding2;
    uint16_t es;
    uint16_t padding3;
    uint16_t ds;
    uint16_t padding4;
    uint32_t trapnum;

    /* below are pushed by the x86 hardware */
    uint32_t err;
    uint32_t eip;
    uint16_t cs;
    uint16_t padding5;
    uint32_t eflags;

    /* crossing rings such as from user to kernel */
    uint32_t esp;
    uint16_t ss;
    uint16_t padding6;
} __attribute__((packed));

#endif

#endif