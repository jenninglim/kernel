#ifndef __RUNQUEUE_H
#define __RUNQUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Include functionality relating to the platform.

#include   "GIC.h"
#include "PL011.h"
#include "SP804.h"


#include "hash/hash.h"
#include "sched/task.h"
#include "sched/prio_array.h"

extern void main_console();
extern uint32_t tos_console;

typedef struct runqueue {
    task_t * current;
    task_t * idle;
    pid_t upid;
    int elapsed_time;
    pid_table_t pid_table;
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

void reallocate_time(runqueue_t * rq);

/*
 *
 */

task_t * rq_add_clone(runqueue_t * rq, ctx_t * ctx);

task_t * rq_add_new_task(runqueue_t * rq, uint32_t pc);

task_t * rq_add_console(runqueue_t * rq);

task_t * rq_find_task_pid(runqueue_t * rq, pid_t pid);

void * rq_task_prio_change(runqueue_t * rq, pid_t pid, int prio);

task_t * rq_remove_task(runqueue_t * rq, pid_t pid);

/*
 * Run idle process
 */

void idle(runqueue_t * rq);

#endif
