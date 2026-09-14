# qunix - A minimal Unix-like Kernel
# Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
#
# This file is part of qunix, distributed under the GNU GPL v3.
# For full terms see the included LICENSE file.

include mk/config.mk
include mk/toolchain.mk

BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
BINDIR := $(BUILDDIR)/bin


.PHONY: clean help
.DEFAULT_GOAL := help

clean:
	@rm -rf $(BUILDDIR)

help:
	@echo "qunix - A minimal Unix-like Kernel v$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"
	@echo "Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved."
	@echo ""
	@echo "USAGE:"
	@echo "\tmake elf                 - build the kernel as an ELF file"
	@echo "\tmake iso                 - build the kernel as an ISO file"
	@echo "\tmake run                 - run the kernel in default emulator"
	@echo "\tmake qemu                - run the kernel in QEMU"
	@echo "\tmake qemu-gdb            - run the kernel in QEMU with GDB"
	@echo "\tmake bochs               - run the kernel in Bochs"
	@echo ""
	@echo "\tmake clean               - clean the build directory"
	@echo "\tmake help                - show this help message"
	@echo "\tmake gen-config          - generate the config files"
	@echo "\tmake version             - show the version of the kernel"
	@echo ""

gen-config:
	@$(PYTHON) scripts/gen-config.py

version:
	@echo "qunix version $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) arch $(ARCH)"