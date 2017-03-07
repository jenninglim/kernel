#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Include functionality relating to newlib

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <list.h>
#include <hilevel.h>
/*
 * task_struct represents 
 */

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

void set_task_state(task_struct *task, int state);

void set_task_prio(task_struct *task, int prio);

void set_task_pcb(task_struct *task, pcb_t pcb);

task_struct create_task(int state, int prio, pcb_t pcb);

void add_task(task_struct *new, task_struct *prev);

void next_task(task_struct *current);

#endif
