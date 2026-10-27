# qunix

qunix is a clean, minimal Unix-like kernel for learning and research, aiming at POSIX compatibility. It is written from scratch in C and assembly, with a arch layer at the edge and a portable generic kernel in the middle.


Current version: `v0.2.0`

For copyright information, see the file `LICENSE` in this directory.

## Architecture

i386.

## Configuration

Project settings (arch, toolchain prefix, version, …) live in `config/config.cfg`. After editing, regenerate the build and header files:

```
make gen-config
```

## Build

```
make all                 - build all object files
make elf                 - build the kernel as an ELF file
make iso                 - build the kernel as an ISO file
make run                 - run the kernel in default emulator
make qemu                - run the kernel in QEMU
make qemu-gdb            - run the kernel in QEMU with GDB
make bochs               - run the kernel in Bochs

make clean               - clean the build directory
make help                - show this help message
make clang               - generate compile_commands.json
make gen-config          - generate the config files
make mb-check            - check multiboot2 compliance of ELF
make check-includes      - check header include conventions
make line                - count source lines by module
make version             - show the version of the kernel
```

Build output goes to `build/`

## Source Roadmap

| Directory | Description |
| --- | --- |
| `arch` | Architecture-specific sources. |
| `arch/$ARCH/asm` | Low-level hardware support. |
| `arch/$ARCH/boot` | Boot code. |
| `arch/$ARCH/kernel` | Architecture implementations of kernel interfaces. |
| `kernel` | Generic kernel sources. |
| `driver` | Hardware drivers. |
| `lib` | Kernel libraries. |
| `lib\libc` | Kernel freestanding libc. |
| `include/arch/$ARCH` | Low-level hardware headers. |
| `include/driver` | Driver headers. |
| `include/kernel` | Kernel headers. |
| `config` | Build and emulator configuration. |
| `ld` | Linker scripts. |
| `mk` | Makefile fragments. |
| `scripts` | Build helper scripts. |

## Release

### [qunix v0.1.0](https://github.com/unsigend/qunix/tree/v0.1.0)

#### Boot, Memory, Interrupts, Basic Kernel Threads

**Features**
- Multiboot2 boot, higher-half kernel at `0xC0000000`, 4KB two-level paging
- Memory map detection, free-list page frame allocator, first-fit `kmalloc`
- GDT, IDT, generic trap layer, `#PF` diagnostics
- 8259 PIC, IRQ registration and dispatch, 8253 PIT tick and `jiffies`
- VGA console behind the `tty` interface, `printk`, log macros
- `task_struct`, context switch, kernel threads, idle task
- Preemptive round-robin scheduling on timer tick

----

### [qunix v0.1.1](https://github.com/unsigend/qunix/tree/v0.1.1)

#### Synchronization, Preemption, Thread Lifecycle

Complete re-write based on v0.1.0

**Features**
- Synchronization primitives: wait queue, semaphore, mutex, condition variable, readers-writer lock, completion
- IRQ-safe spinlock (`lock` / `irq` / `irqsave`, holder checks), interrupt save/restore, preempt count
- `struct cpu`: current and idle task, `need_resched`, `in_sched`, IRQ depth
- `task_exit` / `task_join`, `TASK_BLOCKED` / `TASK_ZOMBIE`, `sched_wakeup`
- `sched_class` (`enqueue`, `dequeue`, `pick_next`, `tick`), round-robin is the first policy
- `irq_chip`, per-line `irq_desc`, `irq_action`, nested irq line mask
- Intrusive list
- atomic and bitosp 
- Physical memory map: ordered disjoint regions, split and merged on update

**Rebuild**
- Flattened `arch/x86` (i386 and x86_64), Multiboot2 higher-half boot
- GDT, IDT, 4KB paging, one shared `trap_frame`
- VGA `tty` / `printk`, `panic` writes without taking the tty lock
- PMM behind `pmm_ops`, `kmalloc` remains a heap on that allocator
- 8259 as an `irq_chip`, 8253/PIT still drives `jiffies`
- irqsave locks on PMM, `kmalloc`, memmap, sched, IRQ lines, and tty
- Interrupts enabled only after core init

----

### [qunix v0.2.0](https://github.com/unsigend/qunix/tree/v0.2.0)

#### Sleep and TTY

**Features**
- `assert` / `BUG_ON`; frame-pointer build; symbolized `panic` backtrace (embedded `nm` table)
- Command line `key=value` lookup (`cmdline_get` / `cmdline_set`)
- KTEST: linker-section tests, `CONFIG_TEST`, QEMU `isa-debug-exit`, `make test`; suites for sync primitives, `ksleep`, `cdev`
- Ordered `ksleep`; the tick wakes expired heads only
- Character devices: major/minor table, `open` / `read` / `write` / `ioctl` ops
- CMOS RTC, PS/2 keyboard (scancode set 1), 16550 UART (serial console, IRQ RX)
- TTY: one input queue fed by keyboard and UART; registered as a character device
- Generic ring buffer

**Refactor**
- `tty_ops` list: VGA and serial register as consoles; `printk` writes to all of them
- Per-CPU `sched_rq`; boot thread is idle; `sched_yield` is `schedule()`
- `waitq_sleep` / `sched_wakeup` are the only entry and exit of `TASK_BLOCKED`
- Nested `panic` halts without a second print

## Contribution and Copyright

Contributions are welcome. Copyright (C) 2026–2027 Yixiang Qiu. qunix is released under the GNU GPL v3. See `LICENSE`.
