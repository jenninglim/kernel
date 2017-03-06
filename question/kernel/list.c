#include "list.h"

/*
 * Check if linked list structure invariant is true.
 * i.e. if prev->next == next && if next->prev == prev
 */

bool list_add_valid(list_head *prev, list_head *next) {
    if (prev->next != next || next->prev != prev)
    {
        return false;
    }
    return true;
}

void _list_add(list_head *new, list_head *prev, list_head *next) {
    if (!list_add_valid(prev, next)) {
        return;
    }
    next->prev = new;
    new->next = next;
    new->prev = prev;
    WRITE_ONCE(prev->next, new);
    return;
}

void list_add(list_head *new, list_head *head){
    _list_add(new, head, head->next);
}

void _list_del(list_head *prev, list_head *next) {
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
}

void list_del(list_head *entry) {
    _list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}
