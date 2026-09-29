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

#include <asm/page.h>

uint32_t pde_make_flag(int present, int write, int user)
{
    return (present ? PG_P_MASK : 0) | (write ? PG_RW_MASK : 0) |
           (user ? PG_USER_MASK : 0);
}

uint32_t pte_make_flag(int present, int write, int user, int pat, int global)
{
    return (present ? PG_P_MASK : 0) | (write ? PG_RW_MASK : 0) |
           (user ? PG_USER_MASK : 0) | (pat ? PTE_PAT_MASK : 0) |
           (global ? PTE_G_MASK : 0);
}

pde_t pde_make(phys_addr_t pa, uint32_t flags)
{
    return (pa & PG_ADDR_MASK) | flags;
}

pte_t pte_make(phys_addr_t pa, uint32_t flags)
{
    return (pa & PG_ADDR_MASK) | flags;
}
