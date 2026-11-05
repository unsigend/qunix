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

#ifndef _X86_ASM_PAGE_32_H_
#define _X86_ASM_PAGE_32_H_

#include <kernel/stdint.h>
#include <kernel/types.h>

#define PG_P (1u << 0)     /* present */
#define PG_RW (1u << 1)    /* read/write */
#define PG_USER (1u << 2)  /* user */
#define PG_PWT (1u << 3)   /* write through */
#define PG_PCD (1u << 4)   /* cache disabled */
#define PG_ACC (1u << 5)   /* accessed */
#define PG_DIRTY (1u << 6) /* dirty */
#define PG_PS (1u << 7)    /* page size */
#define PG_G (1u << 8)     /* global */

#define pg_set_flag(e, flag) ((e) |= (flag))
#define pg_clear_flag(e, flag) ((e) &= ~(flag))
#define pg_test_flag(e, flag) ((e) & (flag))

#define PD_SHIFT 22
#define PT_SHIFT 12
#define OFF_SHIFT 0

#define PD_ENTRIES 1024
#define PT_ENTRIES 1024
#define PG_ADDR_MASK 0xFFFFF000u

#ifndef __ASSEMBLER__

typedef uint32_t pde_t;
typedef uint32_t pte_t;

#define pde_index(va) (((va) >> PD_SHIFT) & (PD_ENTRIES - 1))
#define pte_index(va) (((va) >> PT_SHIFT) & (PT_ENTRIES - 1))
#define pg_offset(va) ((va) & ~PG_ADDR_MASK)
#define pg_flags(e) ((e) & ~(PG_ADDR_MASK))

#define pde_make(pa, flags) (((pde_t)(pa) & PG_ADDR_MASK) | (flags))
#define pte_make(pa, flags) (((pte_t)(pa) & PG_ADDR_MASK) | (flags))

#define pde_pa(pde) ((phys_addr_t)(pde) & PG_ADDR_MASK)
#define pte_pa(pte) ((phys_addr_t)(pte) & PG_ADDR_MASK)

#endif /* __ASSEMBLER__ */

#endif /* _X86_ASM_PAGE_32_H_ */
