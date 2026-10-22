# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

MKDIR     := mk
SCRIPTDIR := scripts

include $(MKDIR)/config.mk
include $(MKDIR)/toolchain.mk

BUILDDIR  := build
DEPDIR    := $(BUILDDIR)/dep
OBJDIR    := $(BUILDDIR)/obj
BINDIR    := $(BUILDDIR)/bin
LDDIR     := ld
SRCDIR    := kernel driver lib $(ARCH_SRC_DIRS)
INCDIR    := include

# sources
SRCS_C := $(filter-out %_$(ARCH_OTHER_BITS).c, \
          $(shell find $(SRCDIR) -name "*.c" 2>/dev/null))
SRCS_S := $(filter-out %_$(ARCH_OTHER_BITS).S, \
          $(shell find $(SRCDIR) -name "*.S" 2>/dev/null))

# objects
OBJS_C := $(patsubst %.c, $(OBJDIR)/%.c.o, $(SRCS_C))
OBJS_S := $(patsubst %.S, $(OBJDIR)/%.S.o, $(SRCS_S))
OBJS   := $(OBJS_C) $(OBJS_S)

# deps
DEPS := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(OBJS))

-include $(DEPS)

# rules
$(OBJDIR)/%.c.o: %.c
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/$*.c.d)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEPDIR)/$*.c.d -MT $@ -c $< -o $@
	@echo "  CC    $<"

$(OBJDIR)/%.S.o: %.S
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/$*.S.d)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEPDIR)/$*.S.d -MT $@ -c $< -o $@
	@echo "  AS    $<"

KERNEL_ELF := $(BINDIR)/qunix-$(ARCH).elf
KERNEL_ELF_TMP := $(BUILDDIR)/qunix-$(ARCH).tmp.elf
KSYMS_C    := $(BUILDDIR)/ksymtable.c
KSYMS_O    := $(OBJDIR)/ksymtable.c.o
KSYMS_STUB_C := $(BUILDDIR)/ksymtable_stub.c
KSYMS_STUB_O := $(OBJDIR)/ksymtable_stub.c.o
KERNEL_ISO := $(BINDIR)/qunix-$(ARCH).iso
ISODIR     := $(BUILDDIR)/iso
BOCHS_CFG  := config/bochs/$(ARCH)-bochs.cfg

.PHONY: all elf mb-check check-includes line iso run qemu qemu-gdb bochs gen-config clang clean version help
.DEFAULT_GOAL := help

all: $(OBJS)

$(KSYMS_STUB_C):
	@mkdir -p $(BUILDDIR)
	@$(PYTHON) $(SCRIPTDIR)/gen-symtable.py < /dev/null > $@

$(KSYMS_STUB_O): $(KSYMS_STUB_C)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "  CC    $<"

$(KERNEL_ELF_TMP): $(OBJS) $(KSYMS_STUB_O)
	@mkdir -p $(BUILDDIR)
	@$(LD) $(LDFLAGS) -T $(LDDIR)/ld-$(ARCH).ld -o $@ $(OBJS) $(KSYMS_STUB_O) $(LIBGCC)
	@echo "  LD    $@"

$(KSYMS_C): $(KERNEL_ELF_TMP)
	@$(NM) -n $< | $(PYTHON) $(SCRIPTDIR)/gen-symtable.py > $@
	@echo "  NM    $@"

$(KSYMS_O): $(KSYMS_C)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "  CC    $<"

$(KERNEL_ELF): $(OBJS) $(KSYMS_O)
	@mkdir -p $(BINDIR)
	@$(LD) $(LDFLAGS) -T $(LDDIR)/ld-$(ARCH).ld -o $@ $(OBJS) $(KSYMS_O) $(LIBGCC)
	@echo "  LD    $@"

elf: $(KERNEL_ELF)

mb-check: elf
	@$(GRUB_FILE) --is-x86-multiboot2 $(KERNEL_ELF) \
		&& echo "  OK    $(KERNEL_ELF) is multiboot2 compliant" \
		|| echo "  FAIL  $(KERNEL_ELF) is not multiboot2 compliant"

check-includes:
	@sh $(SCRIPTDIR)/check-includes.sh $(SRCDIR) $(INCDIR)

line:
	@$(PYTHON) $(SCRIPTDIR)/linecount.py

iso: elf
	@mkdir -p $(ISODIR)/boot/grub
	@cp $(KERNEL_ELF) $(ISODIR)/boot/$(notdir $(KERNEL_ELF))
	@printf 'set timeout=-1\nset default=0\nmenuentry "%s" {\n\tmultiboot2 /boot/%s sched=%s\n\tboot\n}\n' \
		"$(KERNEL_NAME)" "$(notdir $(KERNEL_ELF))" "$(SCHED_POLICY)" > $(ISODIR)/boot/grub/grub.cfg
	@$(GRUB_MKRESCUE) -o $(KERNEL_ISO) $(ISODIR) 2>/dev/null
	@echo "  ISO   $(KERNEL_ISO)"

run: qemu

qemu: iso
	@$(QEMU) -cdrom $(KERNEL_ISO) -no-reboot -no-shutdown

qemu-gdb: iso
	@$(QEMU) -cdrom $(KERNEL_ISO) -no-reboot -no-shutdown -s -S

bochs: iso
	@$(BOCHS) -f $(BOCHS_CFG) -q

gen-config:
	@$(PYTHON) $(SCRIPTDIR)/gen-config.py

clang:
	@$(MAKE) clean
	@bear -- $(MAKE) all

clean:
	@rm -rf $(BUILDDIR)

version:
	@echo "qunix v$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) arch=$(ARCH)"

help:
	@echo "qunix - A minimal Unix-like Kernel v$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"
	@echo "Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved."
	@echo ""
	@echo "USAGE:"
	@echo "\tmake all                 - build all object files"
	@echo "\tmake elf                 - build the kernel as an ELF file"
	@echo "\tmake iso                 - build the kernel as an ISO file"
	@echo "\tmake run                 - run the kernel in default emulator"
	@echo "\tmake qemu                - run the kernel in QEMU"
	@echo "\tmake qemu-gdb            - run the kernel in QEMU with GDB"
	@echo "\tmake bochs               - run the kernel in Bochs"
	@echo ""
	@echo "\tmake clean               - clean the build directory"
	@echo "\tmake help                - show this help message"
	@echo "\tmake clang               - generate compile_commands.json"
	@echo "\tmake gen-config          - generate the config files"
	@echo "\tmake mb-check            - check multiboot2 compliance of ELF"
	@echo "\tmake check-includes      - check header include conventions"
	@echo "\tmake line                - count source lines by module"
	@echo "\tmake version             - show the version of the kernel"
	@echo ""
