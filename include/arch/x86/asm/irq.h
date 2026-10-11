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

#ifndef _X86_ASM_IRQ_H_
#define _X86_ASM_IRQ_H_

/* External hardware interrupts. */

#define IRQ_NR 16

#define IRQ_TIMER 0   /* Programmable Interval Timer */
#define IRQ_KBD 1     /* Keyboard */
#define IRQ_CASCADE 2 /* Cascade */
#define IRQ_COM2 3    /* COM2 */
#define IRQ_COM1 4    /* COM1 */
#define IRQ_LPT2 5    /* LPT2 */
#define IRQ_FLOPPY 6  /* Floppy disk */
#define IRQ_LPT1 7    /* LPT1 / Unreliable spurious interrupts */
#define IRQ_RTC 8     /* CMOS real-time clock */
#define IRQ_AVAL1 9   /* Available 1 */
#define IRQ_AVAL2 10  /* Available 2 */
#define IRQ_AVAL3 11  /* Available 3 */
#define IRQ_PS2M 12   /* PS2 Mouse */
#define IRQ_FPU 13    /* FPU/Coprocessor/Inter-processor */
#define IRQ_ATA1 14   /* ATA Primary */
#define IRQ_ATA2 15   /* ATA Secondary */

#endif /* _X86_ASM_IRQ_H_ */