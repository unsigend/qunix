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

#include <asm/cpuid.h>

#include <kernel/cpuinfo.h>
#include <kernel/panic.h>
#include <kernel/printk.h>
#include <kernel/string.h>

struct cpuinfo {
    char vendor[13];
    char brand[49];
    unsigned family;
    unsigned model;
    unsigned stepping;
    unsigned feat_edx;
    unsigned feat_ecx;
};

static struct cpuinfo cpuinfo;

void cpuinfo_init(void)
{
    unsigned eax, ebx, ecx, edx;

    if (!cpu_has_cpuid())
        panic("CPU does not support CPUID");

    cpuid(CPUID_GETVENDORSTRING, 0, &eax, &ebx, &ecx, &edx);
    memcpy(cpuinfo.vendor, &ebx, 4);
    memcpy(cpuinfo.vendor + 4, &edx, 4);
    memcpy(cpuinfo.vendor + 8, &ecx, 4);
    cpuinfo.vendor[12] = '\0';

    cpuid(CPUID_GETFEATURES, 0, &eax, &ebx, &ecx, &edx);
    cpuinfo.feat_edx = edx;
    cpuinfo.feat_ecx = ecx;

    cpuinfo.stepping = CPUID_EAX_STEPPING(eax);
    cpuinfo.model = CPUID_EAX_MODEL(eax);
    cpuinfo.family = CPUID_EAX_FAMILY(eax);

    if (cpuinfo.family == 0xf)
        cpuinfo.family += CPUID_EAX_EXT_FAMILY(eax);
    if (CPUID_EAX_FAMILY(eax) == 6 || CPUID_EAX_FAMILY(eax) == 0xf)
        cpuinfo.model |= CPUID_EAX_EXT_MODEL(eax) << 4;

    cpuid(CPUID_INTELEXTENDED, 0, &eax, &ebx, &ecx, &edx);
    if (eax >= CPUID_INTELBRANDSTRINGEND) {
        unsigned i;

        for (i = 0; i < 3; i++) {
            cpuid(CPUID_INTELBRANDSTRING + i, 0, &eax, &ebx, &ecx, &edx);
            memcpy(cpuinfo.brand + i * 16, &eax, 4);
            memcpy(cpuinfo.brand + i * 16 + 4, &ebx, 4);
            memcpy(cpuinfo.brand + i * 16 + 8, &ecx, 4);
            memcpy(cpuinfo.brand + i * 16 + 12, &edx, 4);
        }
        cpuinfo.brand[48] = '\0';
    }

    LOGM_INFO("CPU",
              "CPU: Vendor: %s, Brand: %s, Family: %u, Model: %u, Stepping: %u",
              cpuinfo.vendor, cpuinfo.brand, cpuinfo.family, cpuinfo.model,
              cpuinfo.stepping);
}

const char *cpuinfo_vendor(void) { return cpuinfo.vendor; }
const char *cpuinfo_brand(void) { return cpuinfo.brand; }
unsigned cpuinfo_family(void) { return cpuinfo.family; }
unsigned cpuinfo_model(void) { return cpuinfo.model; }
unsigned cpuinfo_stepping(void) { return cpuinfo.stepping; }

int cpuinfo_has(enum cpuinfo_feature feature)
{
    switch (feature) {
    case CPUINFO_FPU: return cpuinfo.feat_edx & CPUID_FEAT_EDX_FPU;
    case CPUINFO_FXSR: return cpuinfo.feat_edx & CPUID_FEAT_EDX_FXSR;
    case CPUINFO_MMX: return cpuinfo.feat_edx & CPUID_FEAT_EDX_MMX;
    case CPUINFO_SSE: return cpuinfo.feat_edx & CPUID_FEAT_EDX_SSE;
    case CPUINFO_SSE2: return cpuinfo.feat_edx & CPUID_FEAT_EDX_SSE2;
    default: return 0;
    }
}