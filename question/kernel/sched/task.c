#include "task.h"
/*
 * For manipulation of tasks.
 */

task_t create_task(pid_t pid, uint32_t cpsr, uint32_t pc, uint32_t sp) {
    ctx_t new_ctx = {
        .cpsr = cpsr,
        .pc = pc,
        .sp = sp
    };
    task_t new_task = {
        .state = 0,
        .node = LIST_HEAD_INIT(new_task.node),
        .prio = 1,
        .ctx = new_ctx
    };
    return new_task;
}

void set_task_state(task_t * task, int state) {
    task->state = state;
    return;
}

void set_task_prio(task_t * task, int prio) {
    task->prio = prio;
    return;
}

void add_task_next(task_t *new, list_head *head) {
    list_add(&new->node, head);
    return;
}

void add_task_last(task_t * new, list_head * head) {
    list_add_tail(&new->node, head);
}
