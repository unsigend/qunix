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

#ifndef _QUNIX_CPUINFO_H_
#define _QUNIX_CPUINFO_H_

enum cpuinfo_feature {
    CPUINFO_FPU,
    CPUINFO_FXSR,
    CPUINFO_MMX,
    CPUINFO_SSE,
    CPUINFO_SSE2,
};

extern void cpuinfo_init(void);
extern const char *cpuinfo_vendor(void);
extern const char *cpuinfo_brand(void);
extern unsigned cpuinfo_family(void);
extern unsigned cpuinfo_model(void);
extern unsigned cpuinfo_stepping(void);
extern int cpuinfo_has(enum cpuinfo_feature feature);

#endif /* _QUNIX_CPUINFO_H_ */