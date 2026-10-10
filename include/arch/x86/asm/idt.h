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

#ifndef _X86_ASM_IDT_H_
#define _X86_ASM_IDT_H_

#define IDT_NR 256

#ifdef __x86_64__
#define IDT_IST(ist) ((ist) & 0x7) /* interrupt stack table offset */
#endif

#define IDT_INT (0xe << 8)  /* interrupt gate */
#define IDT_TRAP (0xf << 8) /* trap gate */
#ifdef __i386__
#define IDT_TASK (0x5 << 8) /* task gate abandoned in x86-64 */
#endif

#define IDT_DPL(dpl) (((dpl) & 3u) << 13) /* descriptor privilege level */

extern void idtr_read(unsigned short *size, unsigned long *base);
extern void idtr_write(unsigned short size, unsigned long base);

/**
 * Set the entry of the IDT at the given index.
 *
 * @index: index of the entry to set.
 * @selector: segment selector.
 * @addr: address of the Interrupt Service Routine.
 * @flags: flags of the entry.
 */
extern void idt_set_entry(unsigned int index, unsigned short selector,
                          unsigned long addr, unsigned int flags);

#endif /* _X86_ASM_IDT_H_ */