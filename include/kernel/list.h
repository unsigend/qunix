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

#include <kernel/macros.h>
#include <kernel/stddef.h>

/* Simple intrusive doubly linked list implementation, optimized for cache
 * locality and avoid malloc for list nodes. Originally from Linux kernel here
 * for simplified version. */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

/* Insert a new entry between two known consecutive entries. This is only for
 * internal list manipulation. */
static inline void __list_add(struct list_head *new, struct list_head *prev,
                              struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/* Delete a list entry by making the prev/next entries point to each other. */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_init - initialize a list head
 * @head: the list head to initialize
 *
 * Initialize a list head to an empty list.
 */
static inline void list_init(struct list_head *head)
{
    head->next = head;
    head->prev = head;
}

/**
 * list_empty - test if list is empty
 * @head: the list to test
 *
 * Return 1 if the list is empty, false otherwise.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

/**
 * list_add_head - add a new entry
 * @head: list head to add it after
 * @new: new entry to be added
 *
 * Insert a new entry at the head of the list.
 * Used for implementing stacks.
 */
static inline void list_add_head(struct list_head *head, struct list_head *new)
{
    __list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @head: list head to add it before
 * @new: new entry to be added
 *
 * Insert a new entry at the tail of the list.
 * Used for implementing queues.
 */
static inline void list_add_tail(struct list_head *head, struct list_head *new)
{
    __list_add(new, head->prev, head);
}

/**
 * list_del - delete an entry from the list
 * @entry: the entry to delete from the list
 *
 * Delete an entry from the list and set the next/prev pointers to NULL.
 */
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

/**
 * list_del_tail - delete the last entry from the list
 * @head: the list to delete the last entry from
 *
 * Delete the last entry from the list and set the prev pointer to the new last
 * entry.
 */
static inline void list_del_tail(struct list_head *head)
{
    if (list_empty(head))
        return;
    list_del(head->prev);
}

/**
 * list_del_head - delete the first entry from the list
 * @head: the list to delete the first entry from
 *
 * Delete the first entry from the list and set the next pointer to the new
 * first entry.
 */
static inline void list_del_head(struct list_head *head)
{
    if (list_empty(head))
        return;
    list_del(head->next);
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
 * list_for_each - iterate over a list
 * @head: the list to iterate over
 * @pos: the position variable
 *
 * Iterate over a list starting from the head.
 */
#define list_for_each(head, pos)                                               \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#endif /* _QUNIX_LIST_H_ */