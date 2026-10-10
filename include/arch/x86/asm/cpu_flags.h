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

#ifndef _X86_ASM_CPU_FLAGS_H_
#define _X86_ASM_CPU_FLAGS_H_

#define EFLAGS_CF 0    /* Carry Flag */
#define EFLAGS_PF 2    /* Parity Flag */
#define EFLAGS_AF 4    /* Auxiliary Carry Flag */
#define EFLAGS_ZF 6    /* Zero Flag */
#define EFLAGS_SF 7    /* Sign Flag */
#define EFLAGS_TF 8    /* Trap Flag */
#define EFLAGS_IF 9    /* Interrupt Enable Flag */
#define EFLAGS_DF 10   /* Direction Flag */
#define EFLAGS_OF 11   /* Overflow Flag */
#define EFLAGS_IOPL 12 /* I/O Privilege Level */
#define EFLAGS_NT 14   /* Nested Task */
#define EFLAGS_RF 16   /* Resume Flag */
#define EFLAGS_VM 17   /* Virtual-8086 Mode */
#define EFLAGS_AC 18   /* Alignment Check / Access Control */
#define EFLAGS_VIF 19  /* Virtual Interrupt Flag */
#define EFLAGS_VIP 20  /* Virtual Interrupt Pending */
#define EFLAGS_ID 21   /* ID Flag */

#define CR0_PE 0  /* Protected Mode Enable */
#define CR0_MP 1  /* Monitor Co-Processor */
#define CR0_EM 2  /* Emulation */
#define CR0_TS 3  /* Task Switched */
#define CR0_ET 4  /* Extension Type */
#define CR0_NE 5  /* Numeric Error */
#define CR0_WP 16 /* Write Protect */
#define CR0_AM 18 /* Alignment Mask */
#define CR0_NW 29 /* Not-Write Through */
#define CR0_CD 30 /* Cache Disable */
#define CR0_PG 31 /* Paging */

#define CR3_PCID 0  /* Process-Context Identifier */
#define CR3_PWT 3   /* Page-level Write-Through */
#define CR3_PCD 4   /* Page-level Cache Disable */
#define CR3_PDBR 12 /* Page Directory Base Register */

#define CR4_VME 0    /* Virtual-8086 Mode Extensions */
#define CR4_PVI 1    /* Protected Mode Virtual Interrupts */
#define CR4_TSD 2    /* Time Stamp enabled only in ring 0 */
#define CR4_DE 3     /* Debugging Extensions */
#define CR4_PSE 4    /* Page Size Extension */
#define CR4_PAE 5    /* Physical Address Extension */
#define CR4_MCE 6    /* Machine Check Exception */
#define CR4_PGE 7    /* Page Global Enable */
#define CR4_PCE 8    /* Performance Monitoring Counter Enable */
#define CR4_OSFXSR 9 /* OS support for fxsave and fxrstor instructions */
#define CR4_OSXMMEXCPT                                                         \
    10 /* OS Support for unmasked simd floating point exceptions */
#define CR4_UMIP                                                               \
    11 /* User-Mode Instruction Prevention (SGDT, SIDT, SLDT, SMSW, and STR    \
          are disabled in user mode) */
#define CR4_LA57 12 /* 5 level paging */
#define CR4_VMXE 13 /* Virtual Machine Extensions Enable */
#define CR4_SMXE 14 /* Safer Mode Extensions Enable */
#define CR4_FSGSBASE                                                           \
    16 /* Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE  \
        */
#define CR4_PCIDE 17   /* PCID Enable */
#define CR4_OSXSAVE 18 /* XSAVE And Processor Extended States Enable */
#define CR4_SMEP 20    /* Supervisor Mode Executions Protection Enable */
#define CR4_SMAP 21    /* Supervisor Mode Access Protection Enable */
#define CR4_PKE 22     /* Enable protection keys for user-mode pages */
#define CR4_CET 23     /* Enable Control-flow Enforcement Technology */
#define CR4_PKS 24     /* Enable protection keys for supervisor-mode pages */

#ifdef __x86_64__
#define CR8_PRI 0 /* Priority */
#endif

#define EFLAGS_CF_MASK (1UL << EFLAGS_CF)
#define EFLAGS_PF_MASK (1UL << EFLAGS_PF)
#define EFLAGS_AF_MASK (1UL << EFLAGS_AF)
#define EFLAGS_ZF_MASK (1UL << EFLAGS_ZF)
#define EFLAGS_SF_MASK (1UL << EFLAGS_SF)
#define EFLAGS_TF_MASK (1UL << EFLAGS_TF)
#define EFLAGS_IF_MASK (1UL << EFLAGS_IF)
#define EFLAGS_DF_MASK (1UL << EFLAGS_DF)
#define EFLAGS_OF_MASK (1UL << EFLAGS_OF)
#define EFLAGS_IOPL_MASK (3UL << EFLAGS_IOPL)
#define EFLAGS_NT_MASK (1UL << EFLAGS_NT)
#define EFLAGS_RF_MASK (1UL << EFLAGS_RF)
#define EFLAGS_VM_MASK (1UL << EFLAGS_VM)
#define EFLAGS_AC_MASK (1UL << EFLAGS_AC)
#define EFLAGS_VIF_MASK (1UL << EFLAGS_VIF)
#define EFLAGS_VIP_MASK (1UL << EFLAGS_VIP)
#define EFLAGS_ID_MASK (1UL << EFLAGS_ID)

#define CR0_PE_MASK (1UL << CR0_PE)
#define CR0_MP_MASK (1UL << CR0_MP)
#define CR0_EM_MASK (1UL << CR0_EM)
#define CR0_TS_MASK (1UL << CR0_TS)
#define CR0_ET_MASK (1UL << CR0_ET)
#define CR0_NE_MASK (1UL << CR0_NE)
#define CR0_WP_MASK (1UL << CR0_WP)
#define CR0_AM_MASK (1UL << CR0_AM)
#define CR0_NW_MASK (1UL << CR0_NW)
#define CR0_CD_MASK (1UL << CR0_CD)
#define CR0_PG_MASK (1UL << CR0_PG)

#define CR3_PCID_MASK (((1UL << 12) - 1) << CR3_PCID)
#define CR3_PWT_MASK (1UL << CR3_PWT)
#define CR3_PCD_MASK (1UL << CR3_PCD)
#define CR3_PDBR_MASK (~((1UL << CR3_PDBR) - 1))

#define CR4_VME_MASK (1UL << CR4_VME)
#define CR4_PVI_MASK (1UL << CR4_PVI)
#define CR4_TSD_MASK (1UL << CR4_TSD)
#define CR4_DE_MASK (1UL << CR4_DE)
#define CR4_PSE_MASK (1UL << CR4_PSE)
#define CR4_PAE_MASK (1UL << CR4_PAE)
#define CR4_MCE_MASK (1UL << CR4_MCE)
#define CR4_PGE_MASK (1UL << CR4_PGE)
#define CR4_PCE_MASK (1UL << CR4_PCE)
#define CR4_OSFXSR_MASK (1UL << CR4_OSFXSR)
#define CR4_OSXMMEXCPT_MASK (1UL << CR4_OSXMMEXCPT)
#define CR4_UMIP_MASK (1UL << CR4_UMIP)
#define CR4_LA57_MASK (1UL << CR4_LA57)
#define CR4_VMXE_MASK (1UL << CR4_VMXE)
#define CR4_SMXE_MASK (1UL << CR4_SMXE)
#define CR4_FSGSBASE_MASK (1UL << CR4_FSGSBASE)
#define CR4_PCIDE_MASK (1UL << CR4_PCIDE)
#define CR4_OSXSAVE_MASK (1UL << CR4_OSXSAVE)
#define CR4_SMEP_MASK (1UL << CR4_SMEP)
#define CR4_SMAP_MASK (1UL << CR4_SMAP)
#define CR4_PKE_MASK (1UL << CR4_PKE)
#define CR4_CET_MASK (1UL << CR4_CET)
#define CR4_PKS_MASK (1UL << CR4_PKS)

#ifdef __x86_64__
#define CR8_PRI_MASK (0xFUL << CR8_PRI)
#endif

#endif /* _X86_ASM_CPU_FLAGS_H_ */