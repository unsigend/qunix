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
ifeq ($(RUN_KTEST),1)
BUILDDIR  := build/test
override TEST := 1
CFLAGS    += -DCONFIG_TEST=1 -DCONFIG_TEST_QEMU_EXIT=1
endif

DEPDIR    := $(BUILDDIR)/dep
OBJDIR    := $(BUILDDIR)/obj
BINDIR    := $(BUILDDIR)/bin
LDDIR     := ld
SRCDIR    := kernel driver lib $(ARCH_SRC_DIRS)
ifeq ($(TEST),1)
SRCDIR    += test
CFLAGS    += -DCONFIG_TEST=1
endif
INCDIR    := include
USERDIR   := user
USER_LDS  := $(LDDIR)/ld-user-$(ARCH).ld
USER_ELF  := $(BINDIR)/init-$(ARCH).elf
USER_BIN  := $(BINDIR)/init-$(ARCH).bin

QEMU_DEBUG_PORT ?= 0xf4

# sources
SRCS_C := $(filter-out %_$(ARCH_OTHER_BITS).c, \
          $(shell find $(SRCDIR) -name "*.c" 2>/dev/null))
SRCS_S := $(filter-out %_$(ARCH_OTHER_BITS).S, \
          $(shell find $(SRCDIR) -name "*.S" 2>/dev/null))

USER_SRCS_C := $(filter-out %_$(ARCH_OTHER_BITS).c, \
               $(shell find $(USERDIR) -name "*.c" 2>/dev/null))
USER_SRCS_S := $(filter-out %_$(ARCH_OTHER_BITS).S, \
               $(shell find $(USERDIR) -name "*.S" 2>/dev/null))

# objects
OBJS_C := $(patsubst %.c, $(OBJDIR)/%.c.o, $(SRCS_C))
OBJS_S := $(patsubst %.S, $(OBJDIR)/%.S.o, $(SRCS_S))
OBJS   := $(OBJS_C) $(OBJS_S)

USER_OBJS_C := $(patsubst %.c, $(OBJDIR)/%.c.o, $(USER_SRCS_C))
USER_OBJS_S := $(patsubst %.S, $(OBJDIR)/%.S.o, $(USER_SRCS_S))
USER_OBJS   := $(USER_OBJS_C) $(USER_OBJS_S)

# deps
DEPS := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(OBJS))
USER_DEPS := $(patsubst $(OBJDIR)/%.o, $(DEPDIR)/%.d, $(USER_OBJS))

-include $(DEPS)
-include $(USER_DEPS)

# rules
$(OBJDIR)/user/%.c.o: user/%.c
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/user/$*.c.d)
	@$(CC) $(USER_CFLAGS) -MMD -MP -MF $(DEPDIR)/user/$*.c.d -MT $@ -c $< -o $@
	@echo "  CC    $<"

$(OBJDIR)/user/%.S.o: user/%.S
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/user/$*.S.d)
	@$(CC) $(USER_CFLAGS) -MMD -MP -MF $(DEPDIR)/user/$*.S.d -MT $@ -c $< -o $@
	@echo "  AS    $<"

$(OBJDIR)/%.c.o: %.c
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/$*.c.d)
	@$(CC) $(CFLAGS) -MMD -MP -MF $(DEPDIR)/$*.c.d -MT $@ -c $< -o $@
	@echo "  CC    $<"

$(OBJDIR)/arch/$(ARCH_FAMILY)/kernel/user_image_$(ARCH_BITS).S.o: \
		arch/$(ARCH_FAMILY)/kernel/user_image_$(ARCH_BITS).S $(USER_BIN)
	@mkdir -p $(dir $@) $(dir $(DEPDIR)/arch/$(ARCH_FAMILY)/kernel/user_image_$(ARCH_BITS).S.d)
	@$(CC) $(CFLAGS) -DINIT_BIN=\"$(abspath $(USER_BIN))\" \
		-MMD -MP -MF $(DEPDIR)/arch/$(ARCH_FAMILY)/kernel/user_image_$(ARCH_BITS).S.d \
		-MT $@ -c $< -o $@
	@echo "  AS    $<"

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

.PHONY: all elf user mb-check check-includes line iso run qemu qemu-gdb bochs gen-config clang clean version help test
.DEFAULT_GOAL := help

all: $(USER_BIN) $(OBJS)

$(USER_ELF): $(USER_OBJS) $(USER_LDS)
	@mkdir -p $(BINDIR)
	@$(LD) $(LDFLAGS) -T $(USER_LDS) -o $@ $(USER_OBJS)
	@echo "  LD    $@"

$(USER_BIN): $(USER_ELF)
	@$(OBJCOPY) -O binary $< $@
	@echo "  BIN   $@"

user: $(USER_BIN)

$(KSYMS_STUB_C):
	@mkdir -p $(BUILDDIR)
	@$(PYTHON) $(SCRIPTDIR)/gen-symtable.py < /dev/null > $@

$(KSYMS_STUB_O): $(KSYMS_STUB_C)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "  CC    $<"

$(KERNEL_ELF_TMP): $(USER_BIN) $(OBJS) $(KSYMS_STUB_O)
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

$(KERNEL_ELF): $(USER_BIN) $(OBJS) $(KSYMS_O)
	@mkdir -p $(BINDIR)
	@$(LD) $(LDFLAGS) -T $(LDDIR)/ld-$(ARCH).ld -o $@ $(OBJS) $(KSYMS_O) $(LIBGCC)
	@echo "  LD    $@"

elf: $(KERNEL_ELF)

mb-check: elf
	@$(GRUB_FILE) --is-x86-multiboot2 $(KERNEL_ELF) \
		&& echo "  OK    $(KERNEL_ELF) is multiboot2 compliant" \
		|| echo "  FAIL  $(KERNEL_ELF) is not multiboot2 compliant"

check-includes:
	@sh $(SCRIPTDIR)/check-includes.sh $(SRCDIR) $(INCDIR) $(USERDIR)

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
	@$(QEMU) -cdrom $(KERNEL_ISO) -serial stdio -no-reboot -no-shutdown

qemu-gdb: iso
	@$(QEMU) -cdrom $(KERNEL_ISO) -serial stdio -no-reboot -no-shutdown -s -S

ifeq ($(RUN_KTEST),1)
test: iso
	@echo "  TEST  $(KERNEL_ISO)"
	@set +e; \
	$(QEMU) -cdrom $(KERNEL_ISO) \
		-serial stdio \
		-device isa-debug-exit,iobase=$(QEMU_DEBUG_PORT),iosize=0x04 \
		-no-reboot; \
	status=$$?; \
	if [ $$status -eq 1 ]; then \
		echo "  PASS TESTS"; \
		exit 0; \
	elif [ $$status -eq 3 ]; then \
		echo "  FAIL TESTS"; \
		exit 1; \
	else \
		echo "  QEMU exited $$status"; \
		exit $$status; \
	fi
else
test:
	@$(MAKE) RUN_KTEST=1 test
endif

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
	@echo "\tmake elf                 - build the user binary, then the kernel ELF"
	@echo "\tmake user                - build the user flat binary"
	@echo "\tmake iso                 - build the kernel as an ISO file"
	@echo "\tmake run                 - run the kernel in default emulator"
	@echo "\tmake qemu                - run the kernel in QEMU"
	@echo "\tmake qemu-gdb            - run the kernel in QEMU with GDB"
	@echo "\tmake test                - build KTEST and run all suites in QEMU"
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
