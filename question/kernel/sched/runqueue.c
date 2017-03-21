#include "runqueue.h"

void init_rq(runqueue_t * rq) {
    rq->current = malloc(sizeof(task_t));
    rq->idle = malloc(sizeof(task_t));
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

void rq_add_new_task(runqueue_t * rq, uint32_t pc) {
    task_t * task = malloc(sizeof(task_t));

    int32_t upid = rq->upid;
    rq->upid = rq->upid + 1;
    
    int32_t offset = upid * 0x00010000;
    TASK_INIT(task, upid, pc, offset);
    
    add_to_active(task, rq);
    return;
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
