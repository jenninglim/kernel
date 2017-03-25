#include "scheduler.h"

/*
 * For scheduling tasks etc....
 */

void dispatch(task_t * new, ctx_t * ctx) {
    memcpy(ctx, &new->ctx, sizeof(ctx_t));
}

void undispatch(task_t * prev, ctx_t * ctx) {
    memcpy(&prev->ctx, ctx, sizeof(ctx_t));
}

void scheduler( ctx_t* ctx, list_head * head ) {
    list_head *node;
    list_for_each(node, head) {
        if (task_current_entry(node)->state == 1) {
            undispatch(task_current_entry(node), ctx);
            dispatch(task_next_entry(node), ctx);
            return;
        }
    }
    return;
}

void time_passed(runqueue_t * rq) {
    rq->elapsed_time = rq->elapsed_time + 1;
    if (rq->elapsed_time - 1 > rq->current->prio) {
        current_expired(rq);
        rq->elapsed_time = 0;
    }
}

bool sched_prio(task_t ** current, prio_array_t * array) {
    list_head * node;
    list_head * head;
    for (int i = 0; i < MAX_PRIO; i++) {
        head = &array->queue[i];
        list_for_each(node, head) {
            * current = task_current_entry(node);
            list_del(node);
            list_add_tail(node, head);
            return true;
        }
    }
    return false;
}

void sched_update_ctx(runqueue_t * rq, ctx_t * ctx) {
    update_ctx(&rq->current->ctx, ctx);
}

void sched_rq(runqueue_t * rq, ctx_t * ctx) {
    //update_ctx(&rq->current->ctx, ctx);
    if (sched_prio(&rq->current, rq->active) == false) {
        reallocate_time(rq);
        sched_rq(rq, ctx);
    }
    else {
        dispatch(rq->current, ctx);
    }
    return;
}
