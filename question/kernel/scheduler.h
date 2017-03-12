#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Include functionality relating to newlib

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <list.h>
#include <hilevel.h>

#define task_current_entry(ptr) \
        list_entry(ptr, task_t, tasks)

#define task_next_entry(ptr) \
        list_first_entry(ptr, task_t, tasks)

typedef struct {
    volatile int state; // program state
    int prio;
    pcb_t pcb;
    list_head tasks;
} task_t;

pcb_t create_pcb(pid_t pid, uint32_t cpsp, uint32_t pc, uint32_t sp);
task_t create_task(pcb_t pcb); 

void dispatch(task_t * new, ctx_t * ctx);

void undispatch(task_t * prev, ctx_t * ctx);

void scheduler(ctx_t * ctx, list_head * head);

void set_task_state(task_t *task, int state);

void set_task_prio(task_t *task, int prio);

void set_task_pcb(task_t *task, pcb_t pcb);

void add_task_next(task_t *new, list_head *head);

/*
static inline void add_task_prev(task_t *new, list_head *head) {
    list_add_tail(&new->tasks, head);
    return;
}
*/
/*
static inline void add_task(list_head * head, pcb_t pcb) {
    task_t temp_task = create_task(pcb);
    list_add(&temp_task.tasks, head);
    return;
}
*/

#endif
