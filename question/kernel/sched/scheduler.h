#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1

// Include functionality relating to newlib

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "sched/prio_array.h"
#include "sched/runqueue.h"
#include "sched/task.h"

/*
 * Dispatch a task.
 * Takes the specified process and copies it to ctx.
 */

void dispatch(task_t * new, ctx_t * ctx);

/*
 * Halts the task from running.
 */

void undispatch(task_t * prev, ctx_t * ctx);

void scheduler(ctx_t * ctx, list_head * head);

/*
 * Sets * current to point to the current running task from the prio_array.
 * Returns true if sucessful, false otherwise.
*/

bool sched_prio(task_t ** current, prio_array_t * array);

/*
 * Schedule the next avaible task depending on priority
 */

void sched_rq(runqueue_t * rq, ctx_t * ctx);

/*
 * Update rq with elapsed time.
 */
void time_passed(runqueue_t * rq);

#endif
