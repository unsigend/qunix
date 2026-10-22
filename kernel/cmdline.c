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

#include <kernel/cmdline.h>
#include <kernel/errno.h>
#include <kernel/stddef.h>
#include <kernel/string.h>

#define CMDLINE_MAX 16
#define CMDLINE_KEY_MAX 32
#define CMDLINE_VAL_MAX 64

struct cmdline_entry {
    char key[CMDLINE_KEY_MAX];
    char val[CMDLINE_VAL_MAX];
};

static struct cmdline_entry cmdtable[CMDLINE_MAX];
static size_t cmdtable_sz = 0;

int cmdline_set(const char *key, const char *value)
{
    if (key == NULL || key[0] == '\0')
        return -EINVAL;
    if (strlen(key) >= CMDLINE_KEY_MAX ||
        (value && strlen(value) >= CMDLINE_VAL_MAX))
        return -ENAMETOOLONG;

    for (size_t i = 0; i < cmdtable_sz; i++) {
        if (strcmp(cmdtable[i].key, key) == 0) {
            if (value)
                strcpy(cmdtable[i].val, value);
            else
                cmdtable[i].val[0] = '\0';
            return 0;
        }
    }

    if (cmdtable_sz >= CMDLINE_MAX)
        return -ENOSPC;

    strcpy(cmdtable[cmdtable_sz].key, key);
    if (value)
        strcpy(cmdtable[cmdtable_sz].val, value);
    else
        cmdtable[cmdtable_sz].val[0] = '\0';
    cmdtable_sz++;
    return 0;
}

const char *cmdline_get(const char *key)
{
    size_t i;

    for (i = 0; i < cmdtable_sz; i++) {
        if (strcmp(cmdtable[i].key, key) == 0)
            return cmdtable[i].val;
    }
    return NULL;
}