#include "list.h"

void _list_add(list_head *new, list_head *prev, list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    WRITE_ONCE(prev->next, new);
}

void list_add_tail(list_head *new, list_head *head) {
	_list_add(new, head->prev, head);
    return;
}

void list_add(list_head *new, list_head *head) {
    _list_add(new, head, head->next);
    return;
}

void INIT_LIST_HEAD(list_head *list) {
	WRITE_ONCE(list->next, list);
	list->prev = list;
    return;
}

void _list_del(list_head *prev, list_head *next) {
    next->prev = prev;
    WRITE_ONCE(prev->next, next);
    return;
}

void list_del(list_head *entry) {
    _list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
    return;
}

void list_move_to_end(list_head * entry, list_head * head) {
    list_del(entry);
    list_add_tail(entry, head);
    return;
}
