#ifndef __PRIO_ARRAY_H
#define __PRIO_ARRAY_H

#define MAX_PRIO 10

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "sched/task.h"
#include "list/list.h"

typedef struct prio_array {
    int nr_active; // # of active tasks 
    list_head queue[MAX_PRIO];
} prio_array_t;

/*
 * Initialise prio_array.
 */

void init_prio_array(prio_array_t * array);

/*
 * Add to prio_array depending on priority level.
 */

void insert_prio_tail(task_t * new, prio_array_t * prio_array);

#endif
