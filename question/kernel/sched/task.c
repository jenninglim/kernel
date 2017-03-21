#include "task.h"
/*
 * For manipulation of tasks.
 */

extern uint32_t tos_usr;

void TASK_INIT(task_t * new_task, pid_t pid, uint32_t pc, uint32_t offset) {
    ctx_t new_ctx = {
        .cpsr = 0x50,
        .pc = pc,
        .sp = (uint32_t) (&tos_usr + offset)
    };
    new_task->state = 0;
    INIT_LIST_HEAD(&new_task->node);
    new_task->prio = 0;
    new_task->ctx = new_ctx;
}

void task_clone(task_t * task, pid_t pid, ctx_t * ctx, uint32_t offset) {
    update_ctx(&task->ctx, ctx);
    task->ctx.sp = &tos_usr + offset;
    task->pid = pid;
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
