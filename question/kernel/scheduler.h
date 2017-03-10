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
    //struct task_t *parent;
    list_head tasks;
} task_t;

/*
 * set a specific task to a specific state.
 */
task_t create_task(pcb_t pcb); 

static inline void set_task_state(task_t *task, int state) {
    task->state = state;
    return;
}

static inline void set_task_prio(task_t *task, int prio) {
    task->prio = prio;
    return;
}

static inline void set_task_pcb(task_t *task, pcb_t pcb) {
    task->pcb = pcb;
    return;
}
/*
static inline task_t create_task(pcb_t pcb) {
    task_t new_task;
	new_task.state = 0;
    new_task.prio = 1;
	new_task.pcb = pcb;
    INIT_LIST_HEAD(&new_task.tasks);
    return new_task;
}
*/

static inline void add_task_next(task_t *new, list_head *head) {
    list_add(&new->tasks, head);
    return;
}

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
