# qunix

qunix is a clean, minimal Unix-like kernel for learning and research. 

Current version: `v0.1.0`

## Architecture

i386.

## Configuration

Project settings live in `config/config.cfg`. After editing, regenerate the build and header files:

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
make version             - show the version of the kernel
```

## Release

### qunix v0.1.0

#### Features

- Multiboot2 boot, higher-half kernel at `0xC0000000`, 4KB two-level paging
- Memory map detection, free-list page frame allocator, first-fit `kmalloc`
- GDT, IDT, generic trap layer, `#PF` diagnostics
- 8259 PIC, IRQ registration and dispatch, 8253 PIT tick and `jiffies`
- VGA console behind the `tty` interface, `printk`, log macros
- `task_struct`, context switch, kernel threads, idle task
- Preemptive round-robin scheduling on timer tick

## Contribution and Copyright

Contributions are welcome. Copyright (C) 2026–2027 Yixiang Qiu. qunix is released under the GNU GPL v3. See `LICENSE`.
