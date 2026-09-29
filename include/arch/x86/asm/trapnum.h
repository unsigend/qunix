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

#ifndef _ASM_X86_TRAPNUM_H_
#define _ASM_X86_TRAPNUM_H_

#define X86_TRAP_DIVIDE 0x00 /* Divide error */
#define X86_TRAP_DEBUG 0x01  /* Debug exception */
#define X86_TRAP_NMI 0x02    /* Non-maskable interrupt */
#define X86_TRAP_BRKPT 0x03  /* Breakpoint */
#define X86_TRAP_OFLOW 0x04  /* Overflow */
#define X86_TRAP_BOUND 0x05  /* Bounds check */
#define X86_TRAP_ILLOP 0x06  /* Invalid opcode */
#define X86_TRAP_DEVICE 0x07 /* Device not available */
#define X86_TRAP_DBLFLT 0x08 /* Double fault */
#define X86_TRAP_COPROC                                                        \
    0x09 /* Coprocessor segment overrun, reserved not used since 486 */
#define X86_TRAP_TSS 0x0A      /* Invalid task switch segment */
#define X86_TRAP_SEGNP 0x0B    /* Segment not present */
#define X86_TRAP_STACK 0x0C    /* Stack exception */
#define X86_TRAP_GPFLT 0x0D    /* General protection fault */
#define X86_TRAP_PGFLT 0x0E    /* Page fault */
#define X86_TRAP_RESERVED 0x0F /* Reserved */
#define X86_TRAP_FPERR 0x10    /* x87 FPU floating point error */
#define X86_TRAP_ALIGN 0x11    /* Alignment check */
#define X86_TRAP_MCHK 0x12     /* Machine check */
#define X86_TRAP_SIMDERR 0x13  /* SIMD floating point error */
#define X86_TRAP_VIRT 0x14     /* Virtualization exception */
#define X86_TRAP_SECURITY 0x1E /* Security exception */

/* 0x15 - 0x1D, 0x1F are reserved */

/* 0x20 - 0xFF are for external interrupts */
#define X86_IRQ_BASE 0x20
#define X86_IRQ_MAXNUM 16

#endif /* _ASM_X86_TRAPNUM_H_ */