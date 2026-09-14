# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.


# tools
CC      := $(TOOLCHAIN)gcc
AS      := $(TOOLCHAIN)as
LD      := $(TOOLCHAIN)ld
AR      := $(TOOLCHAIN)ar
NM      := $(TOOLCHAIN)nm
OBJCOPY := $(TOOLCHAIN)objcopy
OBJDUMP := $(TOOLCHAIN)objdump
STRIP   := $(TOOLCHAIN)strip
PYTHON  := python3

QEMU  := qemu-system-$(ARCH)
BOCHS := bochs

GRUB_FILE := $(TOOLCHAIN)grub-file
GRUB_MKRESCUE := $(TOOLCHAIN)grub-mkrescue

# arch family
ifeq ($(ARCH),$(filter $(ARCH),i386 x86_64))
    ARCH_FAMILY := x86
else ifeq ($(ARCH),$(filter $(ARCH),riscv32 riscv64))
    ARCH_FAMILY := riscv
else
    $(error unsupported ARCH '$(ARCH)' — valid: i386, x86_64, riscv32, riscv64)
endif

# arch flags
ifeq ($(ARCH),i386)
    CFLAGS_ARCH  := -m32 -march=i686
    ASFLAGS_ARCH := --32
    LDFLAGS_ARCH := -m elf_i386
else ifeq ($(ARCH),x86_64)
    CFLAGS_ARCH  := -m64 -march=x86-64 -mcmodel=kernel \
                    -mno-red-zone -mno-mmx -mno-sse -mno-sse2
    ASFLAGS_ARCH := --64
    LDFLAGS_ARCH := -m elf_x86_64
else ifeq ($(ARCH),riscv32)
    CFLAGS_ARCH  := -march=rv32ima -mabi=ilp32
    ASFLAGS_ARCH :=
    LDFLAGS_ARCH := -m elf32lriscv
else ifeq ($(ARCH),riscv64)
    CFLAGS_ARCH  := -march=rv64gc -mabi=lp64
    ASFLAGS_ARCH :=
    LDFLAGS_ARCH := -m elf64lriscv
endif

# libgcc provides arch runtime helpers
LIBGCC := $(shell $(CC) $(CFLAGS_ARCH) -print-libgcc-file-name)

# freestanding
CFLAGS_FREESTANDING := -ffreestanding
CFLAGS_FREESTANDING += -fno-builtin
CFLAGS_FREESTANDING += -fno-common
CFLAGS_FREESTANDING += -fno-stack-protector
CFLAGS_FREESTANDING += -fno-pie
CFLAGS_FREESTANDING += -fno-pic
CFLAGS_FREESTANDING += -nostdlib
CFLAGS_FREESTANDING += -nostdinc

# warnings
CFLAGS_WARN := -Wall -Wextra -Werror
CFLAGS_WARN += -Wundef
CFLAGS_WARN += -Wshadow
CFLAGS_WARN += -Wstrict-prototypes
CFLAGS_WARN += -Wmissing-prototypes

# debug
ifeq ($(DEBUG),1)
    CFLAGS_OPT := -O0 -g3
else
    CFLAGS_OPT := -O2
endif

# include paths
CFLAGS_INC := -I include
CFLAGS_INC += -I include/lib/libc
CFLAGS_INC += -I include/arch/$(ARCH)
CFLAGS_INC += -I include/arch/$(ARCH_FAMILY)

# flags
CFLAGS  := -std=gnu11 $(CFLAGS_FREESTANDING) $(CFLAGS_ARCH) \
           $(CFLAGS_WARN) $(CFLAGS_OPT) $(CFLAGS_INC)

ASFLAGS := $(ASFLAGS_ARCH)

LDFLAGS := $(LDFLAGS_ARCH) -nostdlib -static
