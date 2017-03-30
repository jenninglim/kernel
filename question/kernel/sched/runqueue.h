#ifndef __RUNQUEUE_H
#define __RUNQUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Include functionality relating to the platform.

#include   "GIC.h"
#include "PL011.h"
#include "SP804.h"

#define STACKSPACE 0x00100000
#define VIRTUAL_MEM 0x73100000

#include "hash/hash.h"
#include "sched/task.h"
#include "sched/prio_array.h"

typedef struct runqueue {
    task_t * current;        //Active task.
    pid_t upid;              //Next unique PID.
    int elapsed_time;        //Total time ran by current process.
    pte_t  kernel_pg[4096];  //Kernel page?
    pid_table_t pid_table;   //PID hash table.
    prio_array_t * active;   //Processes with active timeslices.
    prio_array_t * expired;  //Processes with no timeslice remaining.
    prio_array_t arrays[2];  //Arrays storing the active and expired queue.
} runqueue_t;

// Initialises a runqueue.
void init_rq(runqueue_t * rq);

// Adds a task to expired priority queue.
void add_to_expired(task_t * new, runqueue_t * rq);

// Adds a task to active priority queue.
void add_to_active(task_t * new, runqueue_t * rq);

// Moves current task to expired queue.
void current_expired(runqueue_t * rq);

// Switches active and expired priority queues.
void reallocate_time(runqueue_t * rq);

// Creates a new task duplicating the context.
task_t * rq_add_clone(runqueue_t * rq, ctx_t * ctx);

// Add task to runqueue.
task_t * rq_add_new_task(runqueue_t * rq, uint32_t pc);

// Return task from PID.
task_t * rq_find_task_pid(runqueue_t * rq, pid_t pid);

// Changes priority of a task given a PID
void * rq_task_prio_change(runqueue_t * rq, pid_t pid, int prio);

// Removes a task from runqueue given a pid.
task_t * rq_remove_task(runqueue_t * rq, pid_t pid);


#endif
