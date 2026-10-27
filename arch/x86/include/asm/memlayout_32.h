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

#ifndef _X86_ASM_MEMLAYOUT_32_H_
#define _X86_ASM_MEMLAYOUT_32_H_

#include <kernel/sizes.h>

#define KERNEL_PHYS_START 0x0
#define KERNEL_PHYS_END GB_N(1)

#define KERNEL_VIRT_START GB_N(3)
#define KERNEL_VIRT_END GB_N(4)

#define USER_VIRT_START 0x0
#define USER_VIRT_END GB_N(3)

/* kernel image physical addresses */
#define KERNEL_IMAGE_PHYS_START kernel_image_phys_start
#define KERNEL_IMAGE_PHYS_END kernel_image_phys_end

/* kernel image virtual addresses */
#define KERNEL_IMAGE_VIRT_START kernel_image_virt_start
#define KERNEL_IMAGE_VIRT_END kernel_image_virt_end

/* kernel image size */
#define KERNEL_IMAGE_SIZE kernel_image_size

#ifndef __ASSEMBLER__
#include <kernel/types.h>

extern phys_addr_t kernel_image_phys_start;
extern phys_addr_t kernel_image_phys_end;
extern virt_addr_t kernel_image_virt_start;
extern virt_addr_t kernel_image_virt_end;
extern size_t kernel_image_size;

#endif /* __ASSEMBLER__ */

#endif /* _X86_ASM_MEMLAYOUT_32_H_ */