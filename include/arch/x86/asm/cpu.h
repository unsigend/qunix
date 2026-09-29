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

#ifndef _ASM_X86_CPU_H_
#define _ASM_X86_CPU_H_

#include <kernel/compiler.h>

static __always_inline void write_cr0(unsigned long value);
static __always_inline unsigned long read_cr0(void);

/* CR1: Reserved control register, the CPU will throw a #UD exception when
 * trying to access it. */

static __always_inline void write_cr2(unsigned long value);
static __always_inline unsigned long read_cr2(void);

static __always_inline void write_cr3(unsigned long value);
static __always_inline unsigned long read_cr3(void);

static __always_inline void write_cr4(unsigned long value);
static __always_inline unsigned long read_cr4(void);

static __always_inline unsigned long read_flags(void);

#define FLAGS_CF 0    /* Carry Flag */
#define FLAGS_PF 2    /* Parity Flag */
#define FLAGS_AF 4    /* Auxiliary Carry Flag */
#define FLAGS_ZF 6    /* Zero Flag */
#define FLAGS_SF 7    /* Sign Flag */
#define FLAGS_TF 8    /* Trap Flag */
#define FLAGS_IF 9    /* Interrupt Flag */
#define FLAGS_DF 10   /* Direction Flag */
#define FLAGS_OF 11   /* Overflow Flag */
#define FLAGS_IOPL 12 /* I/O Privilege Level */
#define FLAGS_NT 14   /* Nested Task Flag */
#define FLAGS_RF 16   /* Resume Flag */
#define FLAGS_VM 17   /* Virtual 8086 Mode Flag */
#define FLAGS_AC 18   /* Alignment Check */
#define FLAGS_VIF 19  /* Virtual Interrupt Flag */
#define FLAGS_VIP 20  /* Virtual Interrupt Pending */
#define FLAGS_ID 21   /* ID Flag */

#define CR0_PE 0  /* Protected Mode Enable */
#define CR0_MP 1  /* Monitor Coprocessor */
#define CR0_EM 2  /* X87 FPU Emulation */
#define CR0_TS 3  /* Task Switched */
#define CR0_ET 4  /* Extension Type */
#define CR0_NE 5  /* Numeric Error */
#define CR0_WP 16 /* Write Protect */
#define CR0_AM 18 /* Alignment Mask */
#define CR0_NW 29 /* Not Writethrough */
#define CR0_CD 30 /* Cache Disable */
#define CR0_PG 31 /* Paging */

#define CR4_VME 0    /* Virtual Mode Extensions */
#define CR4_PVI 1    /* Protected-mode Virtual Interrupts */
#define CR4_TSD 2    /* Time Stamp Disable */
#define CR4_DE 3     /* Debugging Extensions */
#define CR4_PSE 4    /* Page Size Extension */
#define CR4_PAE 5    /* Physical Address Extension */
#define CR4_MCE 6    /* Machine Check Exception */
#define CR4_PGE 7    /* Page Global Enable */
#define CR4_PCE 8    /* Performance-Monitoring Counter Enable */
#define CR4_OSFXSR 9 /* OS support for FXSAVE and FXRSTOR instructions */
#define CR4_OSXMMEXCPT                                                         \
    10              /* OS support for Unmasked SIMD Floating-Point Exceptions */
#define CR4_UMIP 11 /* User-Mode Instruction Prevention */
#define CR4_LA57                                                               \
    12 /* 57-bit linear address (if set the processor uses 5-level paging      \
          otherwise it uses 4-level paging) */
#define CR4_VMXE 13 /* Virtual Machine Extensions Enable */
#define CR4_SMXE 14 /* Safer Mode Extensions Enable */
#define CR4_FSGSBASE                                                           \
    16 /* Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and           \
          WRGSBASE */
#define CR4_PCIDE 17   /* Enable PCID */
#define CR4_OSXSAVE 18 /* XSAVE and Processor Extended States Enable */
#define CR4_SMEP 20    /* Supervisor Mode Execution Protection Enable */
#define CR4_SMAP 21    /* Supervisor Mode Access Prevention Enable */
#define CR4_PKE 22     /* Protection Key Enable */
#define CR4_CET 23     /* Control-flow Enforcement Technology */
#define CR4_PKS 24     /* Protection Keys for Supervisor-Mode Pages */

#define FLAGS_CF_MASK (1 << FLAGS_CF)
#define FLAGS_PF_MASK (1 << FLAGS_PF)
#define FLAGS_AF_MASK (1 << FLAGS_AF)
#define FLAGS_ZF_MASK (1 << FLAGS_ZF)
#define FLAGS_SF_MASK (1 << FLAGS_SF)
#define FLAGS_TF_MASK (1 << FLAGS_TF)
#define FLAGS_IF_MASK (1 << FLAGS_IF)
#define FLAGS_DF_MASK (1 << FLAGS_DF)
#define FLAGS_OF_MASK (1 << FLAGS_OF)
#define FLAGS_IOPL_MASK (3 << FLAGS_IOPL)
#define FLAGS_NT_MASK (1 << FLAGS_NT)
#define FLAGS_RF_MASK (1 << FLAGS_RF)
#define FLAGS_VM_MASK (1 << FLAGS_VM)
#define FLAGS_AC_MASK (1 << FLAGS_AC)
#define FLAGS_VIF_MASK (1 << FLAGS_VIF)
#define FLAGS_VIP_MASK (1 << FLAGS_VIP)
#define FLAGS_ID_MASK (1 << FLAGS_ID)

#define CR0_PE_MASK (1 << CR0_PE)
#define CR0_MP_MASK (1 << CR0_MP)
#define CR0_EM_MASK (1 << CR0_EM)
#define CR0_TS_MASK (1 << CR0_TS)
#define CR0_ET_MASK (1 << CR0_ET)
#define CR0_NE_MASK (1 << CR0_NE)
#define CR0_WP_MASK (1 << CR0_WP)
#define CR0_AM_MASK (1 << CR0_AM)
#define CR0_NW_MASK (1 << CR0_NW)
#define CR0_CD_MASK (1 << CR0_CD)
#define CR0_PG_MASK (1 << CR0_PG)

#define CR4_VME_MASK (1 << CR4_VME)
#define CR4_PVI_MASK (1 << CR4_PVI)
#define CR4_TSD_MASK (1 << CR4_TSD)
#define CR4_DE_MASK (1 << CR4_DE)
#define CR4_PSE_MASK (1 << CR4_PSE)
#define CR4_PAE_MASK (1 << CR4_PAE)
#define CR4_MCE_MASK (1 << CR4_MCE)
#define CR4_PGE_MASK (1 << CR4_PGE)
#define CR4_PCE_MASK (1 << CR4_PCE)
#define CR4_OSFXSR_MASK (1 << CR4_OSFXSR)
#define CR4_OSXMMEXCPT_MASK (1 << CR4_OSXMMEXCPT)
#define CR4_UMIP_MASK (1 << CR4_UMIP)
#define CR4_LA57_MASK (1 << CR4_LA57)
#define CR4_VMXE_MASK (1 << CR4_VMXE)
#define CR4_SMXE_MASK (1 << CR4_SMXE)
#define CR4_FSGSBASE_MASK (1 << CR4_FSGSBASE)
#define CR4_PCIDE_MASK (1 << CR4_PCIDE)
#define CR4_OSXSAVE_MASK (1 << CR4_OSXSAVE)
#define CR4_SMEP_MASK (1 << CR4_SMEP)
#define CR4_SMAP_MASK (1 << CR4_SMAP)
#define CR4_PKE_MASK (1 << CR4_PKE)
#define CR4_CET_MASK (1 << CR4_CET)
#define CR4_PKS_MASK (1 << CR4_PKS)

static __always_inline void write_cr0(unsigned long value)
{
    asm volatile("mov %0, %%cr0" : : "r"(value) : "memory");
}

static __always_inline unsigned long read_cr0(void)
{
    unsigned long value;
    asm volatile("mov %%cr0, %0" : "=r"(value));
    return value;
}

static __always_inline void write_cr2(unsigned long value)
{
    asm volatile("mov %0, %%cr2" : : "r"(value) : "memory");
}

static __always_inline unsigned long read_cr2(void)
{
    unsigned long value;
    asm volatile("mov %%cr2, %0" : "=r"(value));
    return value;
}

static __always_inline void write_cr3(unsigned long value)
{
    asm volatile("mov %0, %%cr3" : : "r"(value) : "memory");
}

static __always_inline unsigned long read_cr3(void)
{
    unsigned long value;
    asm volatile("mov %%cr3, %0" : "=r"(value));
    return value;
}

static __always_inline void write_cr4(unsigned long value)
{
    asm volatile("mov %0, %%cr4" : : "r"(value) : "memory");
}

static __always_inline unsigned long read_cr4(void)
{
    unsigned long value;
    asm volatile("mov %%cr4, %0" : "=r"(value));
    return value;
}

static __always_inline unsigned long read_flags(void)
{
    unsigned long value;
    asm volatile("pushf\n\t"
                 "pop %0"
                 : "=r"(value));
    return value;
}

#endif /* _ASM_X86_CPU_H_ */
