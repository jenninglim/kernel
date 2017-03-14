#include "list.h"

void _list_add(list_head *new, list_head *prev, list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    WRITE_ONCE(prev->next, new);
}

void list_add_tail(list_head *new, list_head *head) {
	_list_add(new, head->prev, head);
}

void list_add(list_head *new, list_head *head) {
    _list_add(new, head, head->next);
}

void INIT_LIST_HEAD(list_head *list) {
	WRITE_ONCE(list->next, list);
	list->prev = list;
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
