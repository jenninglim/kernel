#include "scheduler.h"

void set_task_state(task_struct *task, int state) {
    task->state = state;
    return;
}

void set_task_prio(task_struct *task, int prio) {
    task->prio = prio;
    return;
}

void set_task_pcb(task_struct *task, pcb_t pcb) {
    task->pcb = pcb;
    return;
}

task_struct create_task(int state, int prio, pcb_t pcb) {
    task_struct new_task;
    set_task_state(&new_task, state);
    set_task_prio(&new_task, prio);
    set_task_pcb(&new_task, pcb);
    return new_task;
}

void add_task(task_struct *new, task_struct *prev) {
}

