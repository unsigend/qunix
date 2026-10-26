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

#define UART_COM1 0x3F8 /* COM1 */
#define UART_COM2 0x2F8 /* COM2 */
#define UART_COM3 0x3E8 /* COM3 */
#define UART_COM4 0x2E8 /* COM4 */
#define UART_COM5 0x5F8 /* COM5 */
#define UART_COM6 0x4F8 /* COM6 */
#define UART_COM7 0x5E8 /* COM7 */
#define UART_COM8 0x4E8 /* COM8 */

#define UART_RBR 0 /* Receiver buffer (DLAB=0, read) */
#define UART_THR 0 /* Transmit buffer (DLAB=0, write) */
#define UART_DLL 0 /* Divisor latch low (DLAB=1) */
#define UART_DLM 1 /* Divisor latch high (DLAB=1) */
#define UART_IER 1 /* Interrupt enable (DLAB=0) */
#define UART_IIR 2 /* Interrupt identification (read) */
#define UART_FCR 2 /* FIFO control (write) */
#define UART_LCR 3 /* Line control */
#define UART_MCR 4 /* Modem control */
#define UART_LSR 5 /* Line status */
#define UART_MSR 6 /* Modem status */
#define UART_SCR 7 /* Scratch */

/* Line Control Register */

#define LCR_DATA 0   /* Character length */
#define LCR_STOP 2   /* Stop bits */
#define LCR_PARITY 3 /* Parity */
#define LCR_BREAK 6  /* Break enable */
#define LCR_DLAB 7   /* Divisor latch access */

#define LCR_DATA_5 (0 << LCR_DATA) /* 5 bits */
#define LCR_DATA_6 (1 << LCR_DATA) /* 6 bits */
#define LCR_DATA_7 (2 << LCR_DATA) /* 7 bits */
#define LCR_DATA_8 (3 << LCR_DATA) /* 8 bits */

#define LCR_STOP_1 (0 << LCR_STOP) /* 1 stop bit */
#define LCR_STOP_2 (1 << LCR_STOP) /* 1.5 or 2 stop bits */

#define LCR_PARITY_NONE (0 << LCR_PARITY)  /* None */
#define LCR_PARITY_ODD (1 << LCR_PARITY)   /* Odd */
#define LCR_PARITY_EVEN (3 << LCR_PARITY)  /* Even */
#define LCR_PARITY_MARK (5 << LCR_PARITY)  /* Mark */
#define LCR_PARITY_SPACE (7 << LCR_PARITY) /* Space */

#define LCR_BREAK_MASK (1 << LCR_BREAK)
#define LCR_DLAB_MASK (1 << LCR_DLAB)

/* Interrupt Enable Register */

#define IER_RDA 0  /* Received Data Available */
#define IER_THRE 1 /* Transmitter Holding Register Empty */
#define IER_RLS 2  /* Receiver Line Status */
#define IER_MS 3   /* Modem Status */

#define IER_RDA_MASK (1 << IER_RDA)
#define IER_THRE_MASK (1 << IER_THRE)
#define IER_RLS_MASK (1 << IER_RLS)
#define IER_MS_MASK (1 << IER_MS)

/* FIFO Control Register */

#define FCR_ENABLE 0   /* Enable FIFOs */
#define FCR_CLEAR_RX 1 /* Clear receive FIFO */
#define FCR_CLEAR_TX 2 /* Clear transmit FIFO */
#define FCR_DMA 3      /* DMA mode select */
#define FCR_TRIGGER 6  /* Interrupt trigger level */

#define FCR_ENABLE_MASK (1 << FCR_ENABLE)
#define FCR_CLEAR_RX_MASK (1 << FCR_CLEAR_RX)
#define FCR_CLEAR_TX_MASK (1 << FCR_CLEAR_TX)
#define FCR_DMA_MASK (1 << FCR_DMA)

#define FCR_TRIGGER_1 (0 << FCR_TRIGGER)  /* 1 byte */
#define FCR_TRIGGER_4 (1 << FCR_TRIGGER)  /* 4 bytes */
#define FCR_TRIGGER_8 (2 << FCR_TRIGGER)  /* 8 bytes */
#define FCR_TRIGGER_14 (3 << FCR_TRIGGER) /* 14 bytes */

/* Interrupt Identification Register */

#define IIR_PENDING 0 /* 0 = interrupt pending */
#define IIR_STATE 1   /* Interrupt state */
#define IIR_TIMEOUT 3 /* Timeout pending (16550) */
#define IIR_FIFO 6    /* FIFO buffer state */

#define IIR_PENDING_MASK (1 << IIR_PENDING)
#define IIR_TIMEOUT_MASK (1 << IIR_TIMEOUT)

#define IIR_STATE_MODEM (0 << IIR_STATE) /* Modem Status (prio 4) */
#define IIR_STATE_THRE (1 << IIR_STATE)  /* THR Empty (prio 3) */
#define IIR_STATE_RDA (2 << IIR_STATE)   /* Received Data Available (prio 2) */
#define IIR_STATE_RLS (3 << IIR_STATE)   /* Receiver Line Status (prio 1) */

#define IIR_FIFO_NONE (0 << IIR_FIFO)     /* No FIFO */
#define IIR_FIFO_UNUSABLE (1 << IIR_FIFO) /* FIFO enabled but unusable */
#define IIR_FIFO_ENABLED (2 << IIR_FIFO)  /* FIFO enabled */

/* Modem Control Register */

#define MCR_DTR 0  /* Data Terminal Ready */
#define MCR_RTS 1  /* Request to Send */
#define MCR_OUT1 2 /* OUT1 (unused on PC) */
#define MCR_OUT2 3 /* OUT2 (gate UART IRQ on PC) */
#define MCR_LOOP 4 /* Loopback */

#define MCR_DTR_MASK (1 << MCR_DTR)
#define MCR_RTS_MASK (1 << MCR_RTS)
#define MCR_OUT1_MASK (1 << MCR_OUT1)
#define MCR_OUT2_MASK (1 << MCR_OUT2)
#define MCR_LOOP_MASK (1 << MCR_LOOP)

/* Line Status Register */

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

/* Modem Status Register */

#define MSR_DCTS 0 /* Delta CTS */
#define MSR_DDSR 1 /* Delta DSR */
#define MSR_TERI 2 /* Trailing Edge RI */
#define MSR_DDCD 3 /* Delta DCD */
#define MSR_CTS 4  /* Clear to Send */
#define MSR_DSR 5  /* Data Set Ready */
#define MSR_RI 6   /* Ring Indicator */
#define MSR_DCD 7  /* Data Carrier Detect */

#define MSR_DCTS_MASK (1 << MSR_DCTS)
#define MSR_DDSR_MASK (1 << MSR_DDSR)
#define MSR_TERI_MASK (1 << MSR_TERI)
#define MSR_DDCD_MASK (1 << MSR_DDCD)
#define MSR_CTS_MASK (1 << MSR_CTS)
#define MSR_DSR_MASK (1 << MSR_DSR)
#define MSR_RI_MASK (1 << MSR_RI)
#define MSR_DCD_MASK (1 << MSR_DCD)

#endif /* _SERIAL_UART_H_ */
