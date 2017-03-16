#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>
#include <stdbool.h>

#include "types.h"

/* 
 * Returns the container of ptr by calculating its offset.
 */ 

#define  list_entry(ptr, type, member) ({  \
         const typeof ( ((type *)0) -> member ) *__mptr = (ptr); \
         (type *) ( (char *) __mptr - offsetof (type, member) ) ;})

/*
 * http://stackoverflow.com/questions/34988277/write-once-in-linux-kernel-lists
 */

#define WRITE_ONCE(x, val) x=(val)

#define list_first_entry(ptr, type, member) \
        list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/*
 * List structure.
 */
typedef struct list_head {
    struct list_head *prev, *next;
} list_head;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

void INIT_LIST_HEAD(list_head * list);

/*
 * Insert new entry between two known consecutive entries.
 * Use when we know the prev and next entries.
 */

void list_add(list_head *new, list_head *head);

void list_add_tail(list_head * new, list_head * head);

/*
 * Delete list entry.
 */
void list_del(list_head * entry);

/*
 * Moves specified entry to end.
 */

void list_move_to_end(list_head * entry, list_head * head);

#endif
