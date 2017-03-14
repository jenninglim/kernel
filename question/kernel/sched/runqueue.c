#include "runqueue.h"

void init_rq(runqueue_t * rq) {
    rq->current = NULL;
    rq->idle = NULL;

    init_prio_array(&rq->arrays[1]);
    init_prio_array(&rq->arrays[0]);

    rq->active = &rq->arrays[0];
    rq->expired = &rq->arrays[1];

    return;
}

void add_to_active(task_t * task, runqueue_t * rq) {

    return;
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
