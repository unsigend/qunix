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

#ifndef _QUNIX_LIST_H_
#define _QUNIX_LIST_H_

#include "kernel/stddef.h"
#include <kernel/macros.h>
#include <kernel/stdbool.h>
#include <kernel/types.h>

/* Simple intrusive linked list implementation, originally from Linux kernel
 * with changes of the naming convention and params order. */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define list_node list_head

static inline void list_init(struct list_head *head);
static inline void list_add_head(struct list_head *head, struct list_node *new);
static inline void list_add_tail(struct list_head *head, struct list_node *new);
static inline void list_del(struct list_head *head, struct list_node *node);
static inline struct list_node *list_del_tail(struct list_head *head);
static inline struct list_node *list_del_head(struct list_head *head);
static inline bool list_empty(const struct list_head *head);

#define LIST_HEAD_INIT() {NULL, NULL}
#define LIST_NODE_INIT() {NULL, NULL}
#define list_entry(ptr, type, member) container_of(ptr, type, member)
#define list_for_each(head, pos) for (pos = (head)->next; pos; pos = pos->next)

static inline void list_init(struct list_head *head)
{
    head->next = NULL;
    head->prev = NULL;
}

static inline void list_add_head(struct list_head *head, struct list_node *new)
{
    new->next = head->next;
    new->prev = NULL;
    if (new->next)
        new->next->prev = new;

    if (!head->prev)
        head->prev = new;
    head->next = new;
}

static inline void list_add_tail(struct list_head *head, struct list_node *new)
{
    new->next = NULL;
    new->prev = head->prev;
    if (new->prev)
        new->prev->next = new;

    if (!head->next)
        head->next = new;
    head->prev = new;
}

static inline void list_del(struct list_head *head, struct list_node *node)
{
    if (node->next)
        node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    if (node == head->next)
        head->next = node->next;
    if (node == head->prev)
        head->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
}

static inline struct list_node *list_del_tail(struct list_head *head)
{
    struct list_node *node = head->prev;
    if (node) {
        list_del(head, node);
        return node;
    }
    return NULL;
}

static inline struct list_node *list_del_head(struct list_head *head)
{
    struct list_node *node = head->next;
    if (node) {
        list_del(head, node);
        return node;
    }
    return NULL;
}

static inline bool list_empty(const struct list_head *head)
{
    return head->next == NULL;
}

#endif /* _QUNIX_LIST_H_ */