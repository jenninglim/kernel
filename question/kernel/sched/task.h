#ifndef __TASK_H
#define __TASK_H

#define task_current_entry(ptr) \
        list_entry(ptr, task_t, node)

#define task_next_entry(ptr) \
        list_first_entry(ptr, task_t, node)

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "list/list.h"

/*
 * For manipulating tasks
 */

typedef struct {
    volatile int state; // program state
    int prio;
    pid_t pid;
    ctx_t ctx;
    list_head node;
} task_t;

void TASK_INIT(task_t * new_task, uint32_t pc, uint32_t offset);

//void task_clone(task_t * task, pid_t pid, ctx_t * ctx, uint32_t offset);

void set_task_state(task_t * task, int state);

void set_task_prio(task_t * task, int prio);

void add_task_next(task_t * new, list_head * head);

void add_task_last(task_t * new, list_head * head);

void set_task_pid(task_t * task, int32_t pid);

void task_clone(task_t * new, ctx_t * source, uint32_t sp);

#endif
