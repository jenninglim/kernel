#include "task.h"
/*
 * For manipulation of tasks.
 */

void TASK_INIT(task_t * new_task, uint32_t pc, uint32_t sp) {
    ctx_t new_ctx = {
        .cpsr = 0x50,
        .pc = pc,
        .sp = sp
    };
    new_task->state = 0;
    INIT_LIST_HEAD(&new_task->node);
    new_task->prio = 0;
    new_task->ctx = new_ctx;
    INIT_TABLE(new_task->T);
}

void task_clone(task_t * task, ctx_t * ctx, uint32_t sp) {
    update_ctx(&task->ctx, ctx);
    task->ctx.sp = sp;
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

void set_task_pid(task_t * task, int32_t pid) {
    task->pid = pid;
    return;
}
