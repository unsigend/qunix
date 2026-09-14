/* qunix - A minimal Unix-like Kernel
 * Copyright (C) 2026 - 2027 Yixiang Qiu. All rights reserved.
 *
 * This file is part of qunix, distributed under the GNU GPL v3.
 * For full terms see the included LICENSE file.
 */

#include <boot/multiboot2.h>
#include <stdint.h>

int boot_main(uint32_t magic, uintptr_t mbi);

int boot_main(uint32_t magic, uintptr_t mbi)
{
    (void)magic;
    (void)mbi;

    /* TODO: later */

    while (1)
        ;

    return 0;
}