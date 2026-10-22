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

#include <kernel/compiler.h>
#include <kernel/macros.h>
#include <kernel/stddef.h>

/* Simple intrusive linked list implementation, originally from Linux kernel.
 * Embedded the node or head in the structure. */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_POISON                                                            \
    NULL /* Replace this value to an address that will trigger page fault if   \
            used in kernel. Ignore this if used in user space. */

/* Macros */
#define LIST_HEAD_INIT(name)
#define list_entry(ptr, type, member)
#define list_first_entry(head, type, member)
#define list_last_entry(head, type, member)
#define list_for_each(head, pos)
#define list_for_each_entry(head, pos, type, member)
#define list_for_each_safe(head, pos, tmp)
#define list_for_each_entry_safe(head, pos, tmp, type, member)

/* APIs */
static inline void list_init(struct list_head *head);
static inline int list_empty(const struct list_head *head);
static inline void list_add_head(struct list_head *head, struct list_head *new);
static inline void list_add_tail(struct list_head *head, struct list_head *new);
static inline void list_del(struct list_head *node);
static inline void list_del_init(struct list_head *node);
static inline struct list_head *list_del_head(struct list_head *head);
static inline struct list_head *list_del_tail(struct list_head *head);
static inline struct list_head *list_del_head_init(struct list_head *head);
static inline struct list_head *list_del_tail_init(struct list_head *head);

#undef LIST_HEAD_INIT
#undef list_entry
#undef list_first_entry
#undef list_last_entry
#undef list_for_each
#undef list_for_each_entry
#undef list_for_each_safe
#undef list_for_each_entry_safe

#define LIST_HEAD_INIT(name) {&(name), &(name)}

#define list_entry(ptr, type, member) container_of(ptr, type, member)
#define list_first_entry(head, type, member)                                   \
    list_entry((head)->next, type, member)
#define list_last_entry(head, type, member)                                    \
    list_entry((head)->prev, type, member)

#define list_for_each(head, pos)                                               \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_entry(head, pos, type, member)                           \
    for (pos = list_entry((head)->next, type, member); &pos->member != (head); \
         pos = list_entry(pos->member.next, type, member))

#define list_for_each_safe(head, pos, tmp)                                     \
    for (pos = (head)->next, tmp = pos->next; pos != (head);                   \
         pos = tmp, tmp = pos->next)

#define list_for_each_entry_safe(head, pos, tmp, type, member)                 \
    for (pos = list_entry((head)->next, type, member),                         \
        tmp = list_entry(pos->member.next, type, member);                      \
         &pos->member != (head);                                               \
         pos = tmp, tmp = list_entry(pos->member.next, type, member))

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

static inline void __list_add(struct list_head *new, struct list_head *prev,
                              struct list_head *next)
{
    new->next = next;
    new->prev = prev;
    prev->next = new;
    next->prev = new;
}

static inline void list_init(struct list_head *head)
{
    head->next = head;
    head->prev = head;
}

static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

static inline void list_add_head(struct list_head *head, struct list_head *new)
{
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *head, struct list_head *new)
{
    __list_add(new, head->prev, head);
}

static inline void list_del(struct list_head *node)
{
    __list_del(node->prev, node->next);
    node->next = LIST_POISON;
    node->prev = LIST_POISON;
}

static inline void list_del_init(struct list_head *node)
{
    __list_del(node->prev, node->next);
    list_init(node);
}

static inline struct list_head *list_del_head(struct list_head *head)
{
    struct list_head *node;

    if (list_empty(head))
        return NULL;

    node = head->next;
    list_del(node);
    return node;
}

static inline struct list_head *list_del_tail(struct list_head *head)
{
    struct list_head *node;

    if (list_empty(head))
        return NULL;

    node = head->prev;
    list_del(node);
    return node;
}

static inline struct list_head *list_del_head_init(struct list_head *head)
{
    struct list_head *node;

    if (list_empty(head))
        return NULL;

    node = head->next;
    list_del_init(node);
    return node;
}

static inline struct list_head *list_del_tail_init(struct list_head *head)
{
    struct list_head *node;

    if (list_empty(head))
        return NULL;

    node = head->prev;
    list_del_init(node);
    return node;
}

#endif /* _QUNIX_LIST_H_ */