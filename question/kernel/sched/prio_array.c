#include "prio_array.h"

void init_prio_array(prio_array_t * array) {
    array->nr_active = 0;
    for (int i = 0; i < MAX_PRIO; i ++) {
        INIT_LIST_HEAD(&array->queue[i]);
    }
    return;
}

void insert_prio_tail(task_t * new, prio_array_t * array) {
    int prio = new->prio;
    list_head * head = &array->queue[prio];
    add_task_last(new, head);
    return;
}
