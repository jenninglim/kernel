#include "runqueue.h"

void init_rq(runqueue_t * rq) {
    rq->current = malloc(sizeof(task_t));
    rq->idle = malloc(sizeof(task_t));

    init_prio_array(&rq->arrays[1]);
    init_prio_array(&rq->arrays[0]);

    rq->active = &rq->arrays[0];
    rq->expired = &rq->arrays[1];

    return;
}

void add_to_active(task_t * task, runqueue_t * rq) {
    int prio = task->prio;
    rq->arrays[0].nr_active ++;
    list_head * head = &rq->arrays[0].queue[prio];
    add_task_last(task, head);
    return;
}

void rq_add_new_task(runqueue_t * rq, pid_t pid, uint32_t pc, uint32_t offset) {
    task_t * task = malloc(sizeof(task_t));
    TASK_INIT(task, pid, pc, offset);
    add_to_active(task, rq);
}

void add_to_expired(task_t * task, runqueue_t * rq) {

    return;
}

void current_expired(runqueue_t * rq) {
    return;
}

void reallocated_time(runqueue_t * rq) {
    return;
}

void idle(runqueue_t * rq) {
    return;
}
