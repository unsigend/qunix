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

ARCH = i386
TOOLCHAIN = i686-elf-
MAX_CPUS = 1
SMP = 0
SCHED_POLICY = rr
DEBUG = 1
DEBUG_SPINLOCK = 0
TEST = 0
TEST_QEMU_EXIT = 0
VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_PATCH = 1
KERNEL_NAME = qunix
PAGESIZE = 4096
PAGESHIFT = 12
PMM_SIMPLE = 1
PMM_BITMAP = 0
PMM_BUDDY = 0
CLOCK_HZ = 1000
QEMU_DEBUG_PORT = 0xf4
