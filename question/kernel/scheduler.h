#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Include functionality relating to newlib

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <list.h>
#include <hilevel.h>

#define task_current_entry(ptr) \
        list_entry(ptr, task_struct, tasks)

#define task_next_entry(ptr) \
        list_first_entry(ptr, task_struct, tasks)


typedef struct {
    volatile int state; // program state
    int prio;
    pcb_t pcb;
    //struct task_struct *parent;
    list_head tasks;
} task_struct;

/*
 * set a specific task to a specific state.
 */

static inline void set_task_state(task_struct *task, int state) {
    task->state = state;
    return;
}

static inline void set_task_prio(task_struct *task, int prio) {
    task->prio = prio;
    return;
}

static inline void set_task_pcb(task_struct *task, pcb_t pcb) {
    task->pcb = pcb;
    return;
}

static inline task_struct create_task(int state, int prio, pcb_t pcb) {
    task_struct new_task = {
		.state = state,
		.tasks = LIST_HEAD_INIT(new_task.tasks),
    	.prio = prio,
		.pcb = pcb};
    return new_task;

}

static inline void add_task_next(task_struct *new, task_struct *head) {
    list_add(&new->tasks, &head->tasks);
    return;
}

static inline void add_task_prev(task_struct *new, task_struct *head) {
    list_add_tail(&new->tasks, &head->tasks);
    return;
}


#endif
