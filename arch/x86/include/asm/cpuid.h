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

#ifndef _X86_ASM_CPUID_H_
#define _X86_ASM_CPUID_H_

#ifndef __ASSEMBLER__

extern int cpu_has_cpuid(void);
static inline void cpuid(unsigned leaf, unsigned subleaf, unsigned *eax,
                         unsigned *ebx, unsigned *ecx, unsigned *edx);

enum cpuid_requests {
    CPUID_GETVENDORSTRING,
    CPUID_GETFEATURES,
    CPUID_GETTLB,
    CPUID_GETSERIAL,

    CPUID_INTELEXTENDED = 0x80000000,
    CPUID_INTELFEATURES,
    CPUID_INTELBRANDSTRING,
    CPUID_INTELBRANDSTRINGMORE,
    CPUID_INTELBRANDSTRINGEND,
};
#endif /* __ASSEMBLER__ */

#define CPUID_VENDOR_AMD "AuthenticAMD"
#define CPUID_VENDOR_AMD_OLD "AMDisbetter!"
#define CPUID_VENDOR_INTEL "GenuineIntel"
#define CPUID_VENDOR_VIA "VIA VIA VIA "
#define CPUID_VENDOR_TRANSMETA "GenuineTMx86"
#define CPUID_VENDOR_TRANSMETA_OLD "TransmetaCPU"
#define CPUID_VENDOR_CYRIX "CyrixInstead"
#define CPUID_VENDOR_CENTAUR "CentaurHauls"
#define CPUID_VENDOR_NEXGEN "NexGenDriven"
#define CPUID_VENDOR_UMC "UMC UMC UMC "
#define CPUID_VENDOR_SIS "SiS SiS SiS "
#define CPUID_VENDOR_NSC "Geode by NSC"
#define CPUID_VENDOR_RISE "RiseRiseRise"
#define CPUID_VENDOR_VORTEX "Vortex86 SoC"
#define CPUID_VENDOR_AO486 "MiSTer AO486"
#define CPUID_VENDOR_AO486_OLD "GenuineAO486"
#define CPUID_VENDOR_ZHAOXIN "  Shanghai  "
#define CPUID_VENDOR_HYGON "HygonGenuine"
#define CPUID_VENDOR_ELBRUS "E2K MACHINE "
#define CPUID_VENDOR_QEMU "TCGTCGTCGTCG"
#define CPUID_VENDOR_KVM " KVMKVMKVM  "
#define CPUID_VENDOR_VMWARE "VMwareVMware"
#define CPUID_VENDOR_VIRTUALBOX "VBoxVBoxVBox"
#define CPUID_VENDOR_XEN "XenVMMXenVMM"
#define CPUID_VENDOR_HYPERV "Microsoft Hv"
#define CPUID_VENDOR_PARALLELS " prl hyperv "
#define CPUID_VENDOR_PARALLELS_ALT " lrpepyh vr "
#define CPUID_VENDOR_BHYVE "bhyve bhyve "
#define CPUID_VENDOR_QNX " QNXQVMBSQG "

/* Leaf 1 EAX: family / model / stepping */
#define CPUID_EAX_STEPPING(eax) ((eax) & 0xfu)
#define CPUID_EAX_MODEL(eax) (((eax) >> 4) & 0xfu)
#define CPUID_EAX_FAMILY(eax) (((eax) >> 8) & 0xfu)
#define CPUID_EAX_TYPE(eax) (((eax) >> 12) & 0x3u)
#define CPUID_EAX_EXT_MODEL(eax) (((eax) >> 16) & 0xfu)
#define CPUID_EAX_EXT_FAMILY(eax) (((eax) >> 20) & 0xffu)

/* Leaf 1 EBX */
#define CPUID_FEAT_EBX_BRAND_INDEX (0xffu << 0)
#define CPUID_FEAT_EBX_CLFLUSH_LINE_SIZE (0xffu << 8)
#define CPUID_FEAT_EBX_MAX_LOGICAL_PROC (0xffu << 16)
#define CPUID_FEAT_EBX_INITIAL_APIC_ID (0xffu << 24)

/* Leaf 1 ECX */
#define CPUID_FEAT_ECX_SSE3 (1u << 0)
#define CPUID_FEAT_ECX_PCLMUL (1u << 1)
#define CPUID_FEAT_ECX_DTES64 (1u << 2)
#define CPUID_FEAT_ECX_MONITOR (1u << 3)
#define CPUID_FEAT_ECX_DS_CPL (1u << 4)
#define CPUID_FEAT_ECX_VMX (1u << 5)
#define CPUID_FEAT_ECX_SMX (1u << 6)
#define CPUID_FEAT_ECX_EST (1u << 7)
#define CPUID_FEAT_ECX_TM2 (1u << 8)
#define CPUID_FEAT_ECX_SSSE3 (1u << 9)
#define CPUID_FEAT_ECX_CID (1u << 10)
#define CPUID_FEAT_ECX_SDBG (1u << 11)
#define CPUID_FEAT_ECX_FMA (1u << 12)
#define CPUID_FEAT_ECX_CX16 (1u << 13)
#define CPUID_FEAT_ECX_XTPR (1u << 14)
#define CPUID_FEAT_ECX_PDCM (1u << 15)
#define CPUID_FEAT_ECX_PCID (1u << 17)
#define CPUID_FEAT_ECX_DCA (1u << 18)
#define CPUID_FEAT_ECX_SSE4_1 (1u << 19)
#define CPUID_FEAT_ECX_SSE4_2 (1u << 20)
#define CPUID_FEAT_ECX_X2APIC (1u << 21)
#define CPUID_FEAT_ECX_MOVBE (1u << 22)
#define CPUID_FEAT_ECX_POPCNT (1u << 23)
#define CPUID_FEAT_ECX_TSC_DEADLINE (1u << 24)
#define CPUID_FEAT_ECX_AES (1u << 25)
#define CPUID_FEAT_ECX_XSAVE (1u << 26)
#define CPUID_FEAT_ECX_OSXSAVE (1u << 27)
#define CPUID_FEAT_ECX_AVX (1u << 28)
#define CPUID_FEAT_ECX_F16C (1u << 29)
#define CPUID_FEAT_ECX_RDRAND (1u << 30)
#define CPUID_FEAT_ECX_HYPERVISOR (1u << 31)

/* Leaf 1 EDX */
#define CPUID_FEAT_EDX_FPU (1u << 0)
#define CPUID_FEAT_EDX_VME (1u << 1)
#define CPUID_FEAT_EDX_DE (1u << 2)
#define CPUID_FEAT_EDX_PSE (1u << 3)
#define CPUID_FEAT_EDX_TSC (1u << 4)
#define CPUID_FEAT_EDX_MSR (1u << 5)
#define CPUID_FEAT_EDX_PAE (1u << 6)
#define CPUID_FEAT_EDX_MCE (1u << 7)
#define CPUID_FEAT_EDX_CX8 (1u << 8)
#define CPUID_FEAT_EDX_APIC (1u << 9)
#define CPUID_FEAT_EDX_SEP (1u << 11)
#define CPUID_FEAT_EDX_MTRR (1u << 12)
#define CPUID_FEAT_EDX_PGE (1u << 13)
#define CPUID_FEAT_EDX_MCA (1u << 14)
#define CPUID_FEAT_EDX_CMOV (1u << 15)
#define CPUID_FEAT_EDX_PAT (1u << 16)
#define CPUID_FEAT_EDX_PSE36 (1u << 17)
#define CPUID_FEAT_EDX_PSN (1u << 18)
#define CPUID_FEAT_EDX_CLFLUSH (1u << 19)
#define CPUID_FEAT_EDX_DS (1u << 21)
#define CPUID_FEAT_EDX_ACPI (1u << 22)
#define CPUID_FEAT_EDX_MMX (1u << 23)
#define CPUID_FEAT_EDX_FXSR (1u << 24)
#define CPUID_FEAT_EDX_SSE (1u << 25)
#define CPUID_FEAT_EDX_SSE2 (1u << 26)
#define CPUID_FEAT_EDX_SS (1u << 27)
#define CPUID_FEAT_EDX_HTT (1u << 28)
#define CPUID_FEAT_EDX_TM (1u << 29)
#define CPUID_FEAT_EDX_IA64 (1u << 30)
#define CPUID_FEAT_EDX_PBE (1u << 31)

#ifndef __ASSEMBLER__
static inline void cpuid(unsigned leaf, unsigned subleaf, unsigned *eax,
                         unsigned *ebx, unsigned *ecx, unsigned *edx)
{
    asm volatile("cpuid"
                 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                 : "a"(leaf), "c"(subleaf));
}
#endif /* __ASSEMBLER__ */

#endif /* _X86_ASM_CPUID_H_ */