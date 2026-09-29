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

#ifndef _ASM_X86_I8259_H_
#define _ASM_X86_I8259_H_

/* This file defines functions and macros for IBM PC 8259 PIC (Programmable
 * Interrupt Controller) */

#define X86_IRQ_TIMER 0x00   /* timer */
#define X86_IRQ_KBD 0x01     /* keyboard */
#define X86_IRQ_CASCADE 0x02 /* cascade (used for slave PIC)*/
#define X86_IRQ_COM2 0x03    /* COM2 */
#define X86_IRQ_COM1 0x04    /* COM1 */
#define X86_IRQ_LPT2 0x05    /* LPT2 */
#define X86_IRQ_FLOPPY 0x06  /* floppy disk */
#define X86_IRQ_LPT1 0x07    /* LPT1 / unreliable error */
#define X86_IRQ_CMOS 0x08    /* CMOS real-time clock */
#define X86_IRQ_AVAL1 0x09   /* available IRQ 1 */
#define X86_IRQ_AVAL2 0x0A   /* available IRQ 2 */
#define X86_IRQ_AVAL3 0x0B   /* available IRQ 3 */
#define X86_IRQ_PS2M 0x0C    /* PS2 mouse */
#define X86_IRQ_FPU 0x0D     /* FPU / Coprocessor */
#define X86_IRQ_ATA 0x0E     /* Primary ATA hard disk */
#define X86_IRQ_ATA2 0x0F    /* Secondary ATA hard disk */

#ifndef __ASSEMBLER__

#include <kernel/stdint.h>

struct i8259_isr /* Interrupt Service Registers */
{
    uint8_t master;
    uint8_t slave;
};

typedef struct i8259_isr i8259_irr; /* Interrupt Request Register (IRR) */
typedef struct i8259_isr i8259_isr; /* In-Service Register (ISR) */

/* Initialize the 8259 PIC controllers, master and slave used for remapping
 * the IRQs in CPU. */
extern void i8259_init(uint8_t master, uint8_t slave);

extern void i8259_eoi(uint8_t irq);

extern void i8259_mask_all(void);
extern void i8259_unmask_all(void);
extern void i8259_set_mask(uint8_t irq);
extern void i8259_clear_mask(uint8_t irq);

extern i8259_irr i8259_get_irr(void);
extern i8259_isr i8259_get_isr(void);

#endif /* __ASSEMBLER__ */

#endif /* _ASM_X86_I8259_H_ */