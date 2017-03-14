#ifndef __RUNQUEUE_H
#define __RUNQUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "sched/task.h"
#include "sched/prio_array.h"

typedef struct runqueue {
    task_t * current;
    task_t * idle;
    prio_array_t * active;
    prio_array_t * expired;
    prio_array_t arrays[2]; 
} runqueue_t;

/*
 * Initialise runqueue.
 */
void init_rq(runqueue_t * rq);

/*
 * Move to expired.
 */

void add_to_expired(task_t * new, runqueue_t * rq);

/*
 * Move to active.
 */ 

void add_to_active(task_t * new, runqueue_t * rq);

/*
 * Expired: Switches the current into expired.
 */

void current_expired(runqueue_t * rq);

/*
 * No more active tasks.
 */

void reallocated_time(runqueue_t * rq);

/*
 * Run idle process
 */

void idle(runqueue_t * rq);

#endif
