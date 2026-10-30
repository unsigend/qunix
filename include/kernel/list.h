/* collection - A high-performance C data structure library
 * Copyright (C) 2026-2027 Yixiang Qiu
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

#ifndef _LIST_H_
#define _LIST_H_

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

#define LIST_HEAD_INIT(name)
#define list_entry(ptr, type, member)
#define list_first_entry(head, type, member)
#define list_last_entry(head, type, member)
#define list_for_each(head, pos)
#define list_for_each_entry(head, pos, type, member)
#define list_for_each_safe(head, pos, tmp)
#define list_for_each_entry_safe(head, pos, tmp, type, member)

static inline void __list_add(struct list_head *node, struct list_head *prev,
                              struct list_head *next);
static inline void __list_del(struct list_head *prev, struct list_head *next);
static inline void __list_splice(struct list_head *prev, struct list_head *next,
                                 struct list_head *list);

static inline void list_init(struct list_head *head);

static inline int list_empty(const struct list_head *head);
static inline int list_is_head(const struct list_head *head,
                               const struct list_head *node);
static inline int list_is_last(const struct list_head *head,
                               const struct list_head *node);
static inline int list_is_first(const struct list_head *head,
                                const struct list_head *node);

static inline void list_add_head(struct list_head *head,
                                 struct list_head *node);
static inline void list_add_tail(struct list_head *head,
                                 struct list_head *node);
static inline void list_del(struct list_head *node);
static inline void list_del_init(struct list_head *node);
static inline struct list_head *list_del_head(struct list_head *head);
static inline struct list_head *list_del_tail(struct list_head *head);
static inline struct list_head *list_del_head_init(struct list_head *head);
static inline struct list_head *list_del_tail_init(struct list_head *head);
static inline void list_replace(struct list_head *old, struct list_head *node);
static inline void list_replace_init(struct list_head *old,
                                     struct list_head *node);
static inline void list_move_head(struct list_head *head,
                                  struct list_head *node);
static inline void list_move_tail(struct list_head *head,
                                  struct list_head *node);
static inline void list_splice_head(struct list_head *head,
                                    struct list_head *list);
static inline void list_splice_tail(struct list_head *head,
                                    struct list_head *list);
static inline void list_splice_head_init(struct list_head *head,
                                         struct list_head *list);
static inline void list_splice_tail_init(struct list_head *head,
                                         struct list_head *list);

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

static inline void __list_add(struct list_head *node, struct list_head *prev,
                              struct list_head *next)
{
    node->next = next;
    node->prev = prev;
    prev->next = node;
    next->prev = node;
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

static inline void __list_splice(struct list_head *prev, struct list_head *next,
                                 struct list_head *list)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;
    last->next = next;
    next->prev = last;
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

static inline int list_is_head(const struct list_head *head,
                               const struct list_head *node)
{
    return node == head;
}

static inline int list_is_last(const struct list_head *head,
                               const struct list_head *node)
{
    return node->next == head;
}

static inline int list_is_first(const struct list_head *head,
                                const struct list_head *node)
{
    return node->prev == head;
}

static inline void list_add_head(struct list_head *head, struct list_head *node)
{
    __list_add(node, head, head->next);
}

static inline void list_add_tail(struct list_head *head, struct list_head *node)
{
    __list_add(node, head->prev, head);
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

static inline void list_replace(struct list_head *old, struct list_head *node)
{
    node->next = old->next;
    node->prev = old->prev;
    node->next->prev = node;
    node->prev->next = node;
}

static inline void list_replace_init(struct list_head *old,
                                     struct list_head *node)
{
    list_replace(old, node);
    list_init(old);
}

static inline void list_move_head(struct list_head *head,
                                  struct list_head *node)
{
    __list_del(node->prev, node->next);
    list_add_head(head, node);
}

static inline void list_move_tail(struct list_head *head,
                                  struct list_head *node)
{
    __list_del(node->prev, node->next);
    list_add_tail(head, node);
}

static inline void list_splice_head(struct list_head *head,
                                    struct list_head *list)
{
    if (!list_empty(list))
        __list_splice(head, head->next, list);
}

static inline void list_splice_tail(struct list_head *head,
                                    struct list_head *list)
{
    if (!list_empty(list))
        __list_splice(head->prev, head, list);
}

static inline void list_splice_head_init(struct list_head *head,
                                         struct list_head *list)
{
    if (!list_empty(list)) {
        __list_splice(head, head->next, list);
        list_init(list);
    }
}

static inline void list_splice_tail_init(struct list_head *head,
                                         struct list_head *list)
{
    if (!list_empty(list)) {
        __list_splice(head->prev, head, list);
        list_init(list);
    }
}

#endif /* _LIST_H_ */