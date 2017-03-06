#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>
#include <stdbool.h>

/* 
 * Returns the container of ptr by calculating its offset.
 */ 

#define  list_entry(ptr, type, member) \
    ((type *) ((char *) (ptr) - offsetof(type, member)))

/*
 * http://stackoverflow.com/questions/34988277/write-once-in-linux-kernel-lists
 */

#define WRITE_ONCE(x, val) x=(val)

/*
 * List structure.
 */
typedef struct list_head {
    struct list_head *prev, *next;
} list_head;

/*
 * Insert new entry between two known consecutive entries.
 * Use when we know the prev and next entries.
 */

void _list_add(list_head *new, list_head *prev, list_head *next);

/*
 * Insert new entry after a specific head.
 */

void list_add(list_head *new, list_head *head);

/*
 * Delete list entry by using previous and next entries.
 */

void _list_del(list_head *prev, list_head *next);

/*
 * Delete list entry.
 */

void list_del(list_head *entry);

#endif

