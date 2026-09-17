/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#ifndef ARCH_I386_ASM_TRAPS_H
#define ARCH_I386_ASM_TRAPS_H

#define X86_TRAP_DIVIDE 0x00 /* divide error */
#define X86_TRAP_DEBUG 0x01  /* debug exception */
#define X86_TRAP_NMI 0x02    /* non-maskable interrupt */
#define X86_TRAP_BRKPT 0x03  /* breakpoint */
#define X86_TRAP_OFLOW 0x04  /* overflow */
#define X86_TRAP_BOUND 0x05  /* bounds check */
#define X86_TRAP_ILLOP 0x06  /* invalid opcode */
#define X86_TRAP_DEVICE 0x07 /* device not available */
#define X86_TRAP_DBLFLT 0x08 /* double fault */
#define X86_TRAP_COPROC                                                        \
    0x09 /* coprocessor segment overrun, reserved not used since 486 */
#define X86_TRAP_TSS 0x0A      /* invalid task switch segment */
#define X86_TRAP_SEGNP 0x0B    /* segment not present */
#define X86_TRAP_STACK 0x0C    /* stack exception */
#define X86_TRAP_GPFLT 0x0D    /* general protection fault */
#define X86_TRAP_PGFLT 0x0E    /* page fault */
#define X86_TRAP_RESERVED 0x0F /* reserved */
#define X86_TRAP_FPERR 0x10    /* x87 FPU floating point error */
#define X86_TRAP_ALIGN 0x11    /* alignment check */
#define X86_TRAP_MCHK 0x12     /* machine check */
#define X86_TRAP_SIMDERR 0x13  /* SIMD floating point error */
#define X86_TRAP_VIRT 0x14     /* virtualization exception */
#define X86_TRAP_SECURITY 0x1E /* security exception */

/* 0x15 - 0x1D, 0x1F are reserved; 0x20 - 0xFF are for external interrupts */

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