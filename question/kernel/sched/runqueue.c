#include "runqueue.h"

extern uint32_t tos_usr;

void init_rq(runqueue_t * rq) {
    rq->current = NULL;
    rq->idle = malloc(sizeof(task_t));
    
    HASH_INIT(&rq->pid_table);
    rq->elapsed_time = 0;
    rq->upid = 0;

    init_prio_array(&rq->arrays[1]);
    init_prio_array(&rq->arrays[0]);

    rq->active = &rq->arrays[0];
    rq->expired = &rq->arrays[1];

    return;
}

void add_to_active(task_t * task, runqueue_t * rq) {
    int prio = task->prio;
    rq->active->nr_active++;
    list_head * head = &rq->active->queue[prio];
    add_task_last(task, head);
    return;
}

void add_to_expired(task_t * task, runqueue_t * rq) {
    list_del(&task->node);
    int prio = task->prio;
    rq->expired->nr_active++;
    list_head * head = &rq->expired->queue[prio];
    add_task_last(task, head);
    return;
}

task_t * rq_add(runqueue_t * rq, pid_t pid, uint32_t pc, uint32_t sp) {
    task_t * task = malloc(sizeof(task_t));
    TASK_INIT(task, pc, sp);
    user_page(task->T,sp);
    set_task_pid(task, pid); 
    add_to_active(task, rq);
    add_hash_entry(&rq->pid_table, task);
    return task;
}

task_t * rq_find_task_pid(runqueue_t * rq, pid_t pid) {
    return find_task_pid(&rq->pid_table, pid);
}

task_t * rq_remove_task(runqueue_t * rq, pid_t pid) {
    task_t * task = rq_find_task_pid(rq,pid); 
    remove_hash_entry(&rq->pid_table, rq->current->pid);
    list_del(&task->node);
    return task;
}

void * rq_task_prio_change(runqueue_t * rq, pid_t pid, int prio) {
    task_t * task = rq_find_task_pid(rq, pid);
    set_task_prio(task, prio);
    list_del(&task->node);
    add_to_active(task, rq);
}

pid_t new_pid(runqueue_t * rq) {
    rq->upid++;
    return rq->upid - 1;
}

task_t * rq_add_new_task(runqueue_t * rq, uint32_t pc) {
    pid_t npid = new_pid(rq); 
    return rq_add(rq, npid, pc, (uint32_t) &tos_usr - npid * STACKSPACE);
}

task_t * rq_add_console(runqueue_t * rq) {
    pid_t npid = new_pid(rq);
    return rq_add(rq, npid, (uint32_t) (&main_console), (uint32_t) (&tos_console));
}

task_t * rq_add_clone(runqueue_t * rq, ctx_t * ctx) {
    pid_t npid = new_pid(rq); 
    task_t * clone = rq_add(rq, npid, ctx->pc, ctx->sp);
    uint32_t offset = ((uint32_t) &tos_usr) - ((uint32_t) rq->current->pid * STACKSPACE) - ctx->sp;
    task_clone(clone, ctx, ((uint32_t) &tos_usr) - npid * STACKSPACE - offset);
    memcpy( (char *) ((uint32_t) &tos_usr) - (npid + 1) * STACKSPACE, (char *) ((uint32_t) &tos_usr) - (rq->current->pid + 1) * STACKSPACE, STACKSPACE); 
    return clone;
}

void current_expired(runqueue_t * rq) {
    add_to_expired(rq->current, rq);
    rq->active->nr_active = rq->active->nr_active - 1;
    return;
}

void reallocate_time(runqueue_t * rq) {
    prio_array_t * temp = rq->active;
    rq->active = rq->expired;
    rq->expired = temp;
    return;
}

void idle(runqueue_t * rq) {
    return;
}
