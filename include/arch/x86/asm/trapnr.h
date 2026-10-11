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

#ifndef _X86_ASM_TRAPNR_H_
#define _X86_ASM_TRAPNR_H_

/* Synchronous exceptions from CPU and NMI. */

#define X86_TRAP_NR 32
#define X86_TRAP_NR_MIN 0
#define X86_TRAP_NR_MAX (X86_TRAP_NR - 1)

#define X86_TRAP_DE 0x00  /* Divide Error */
#define X86_TRAP_DB 0x01  /* Debug */
#define X86_TRAP_NMI 0x02 /* Non-maskable External Interrupt */
#define X86_TRAP_BP 0x03  /* Breakpoint */
#define X86_TRAP_OF 0x04  /* Overflow */
#define X86_TRAP_BR 0x05  /* Bounds Check */
#define X86_TRAP_UD 0x06  /* Invalid Opcode */
#define X86_TRAP_NM 0x07  /* Device Not Available */
#define X86_TRAP_DF 0x08  /* Double Fault */
#define X86_TRAP_CSO 0x09 /* Coprocessor Segment Overrun */
#define X86_TRAP_TS 0x0A  /* Invalid TSS */
#define X86_TRAP_NP 0x0B  /* Segment Not Present */
#define X86_TRAP_SS 0x0C  /* Stack-Segment Fault */
#define X86_TRAP_GP 0x0D  /* General Protection Fault */
#define X86_TRAP_PF 0x0E  /* Page Fault */
/* 0x0f reserved by Intel */
#define X86_TRAP_MF 0x10 /* X87 Floating-Point Exception */
#define X86_TRAP_AC 0x11 /* Alignment Check */
#define X86_TRAP_MC 0x12 /* Machine Check */
#define X86_TRAP_XM 0x13 /* SIMD Floating-Point Exception */
#define X86_TRAP_VE 0x14 /* Virtualization Exception */
#define X86_TRAP_CP 0x15 /* Control Protection Exception */

#endif /* _X86_ASM_TRAPNR_H_ */