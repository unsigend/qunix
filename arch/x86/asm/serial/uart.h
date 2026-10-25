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

#ifndef _SERIAL_UART_H_
#define _SERIAL_UART_H_

#define UART_BAUD_RATE 115200U

#define UART_COM1 0x3F8
#define UART_COM2 0x2F8
#define UART_COM3 0x3E8
#define UART_COM4 0x2E8
#define UART_COM5 0x5F8
#define UART_COM6 0x4F8
#define UART_COM7 0x5E8
#define UART_COM8 0x4E8

#define UART_RBR 0 /* Receiver Buffer Register (DLAB=0) */
#define UART_THR 0 /* Transmitter Holding Register (DLAB=0) */
#define UART_DLL 0 /* Divisor Latch Low (DLAB=1) */
#define UART_DLM 1 /* Divisor Latch High (DLAB=1) */
#define UART_IER 1 /* Interrupt Enable Register (DLAB=0) */
#define UART_IIR 2 /* Interrupt ID Register */
#define UART_FCR 2 /* FIFO Control Register */
#define UART_LCR 3 /* Line Control Register */
#define UART_MCR 4 /* Modem Control Register */
#define UART_LSR 5 /* Line Status Register */
#define UART_MSR 6 /* Modem Status Register */
#define UART_SCR 7 /* Scratch Register */

#define LCR_DATA 0   /* Data bits */
#define LCR_STOP 2   /* Stop bits */
#define LCR_PARITY 3 /* Parity */
#define LCR_BREAK 6  /* Break control */
#define LCR_DLAB 7   /* Divisor Latch Access Bit */

#define LCR_DATA_5 (0 << LCR_DATA)
#define LCR_DATA_6 (1 << LCR_DATA)
#define LCR_DATA_7 (2 << LCR_DATA)
#define LCR_DATA_8 (3 << LCR_DATA)

#define LCR_STOP_1 (0 << LCR_STOP)
#define LCR_STOP_2 (1 << LCR_STOP) /* Stop bits 1.5/2 */

#define LCR_PARITY_NONE (0 << LCR_PARITY)
#define LCR_PARITY_ODD (1 << LCR_PARITY)
#define LCR_PARITY_EVEN (3 << LCR_PARITY)
#define LCR_PARITY_MARK (5 << LCR_PARITY)
#define LCR_PARITY_SPACE (7 << LCR_PARITY)

#define LSR_DR 0   /* Data Ready */
#define LSR_OE 1   /* Overrun Error */
#define LSR_PE 2   /* Parity Error */
#define LSR_FE 3   /* Framing Error */
#define LSR_BI 4   /* Break Interrupt */
#define LSR_THRE 5 /* Transmitter Holding Register Empty */
#define LSR_TEMT 6 /* Transmitter Empty */
#define LSR_IPE 7  /* Impending Error */

#define LSR_DR_MASK (1 << LSR_DR)
#define LSR_OE_MASK (1 << LSR_OE)
#define LSR_PE_MASK (1 << LSR_PE)
#define LSR_FE_MASK (1 << LSR_FE)
#define LSR_BI_MASK (1 << LSR_BI)
#define LSR_THRE_MASK (1 << LSR_THRE)
#define LSR_TEMT_MASK (1 << LSR_TEMT)
#define LSR_IPE_MASK (1 << LSR_IPE)

#endif /* _SERIAL_UART_H_ */