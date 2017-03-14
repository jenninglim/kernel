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

bool sched_prio(task_t ** current, prio_array_t * array) {
    list_head * node = malloc(sizeof(list_head));
    list_head head;
    for (int i = 0; i < MAX_PRIO; i++) {
        head = array->queue[i];
        list_for_each(node, &head) {
            * current = task_current_entry(node);
            list_move_to_end(node, &head);
            return true;
        }
    }
    free(node);
    return false;
}

void sched_rq(runqueue_t * rq) {
    sched_prio(&rq->current, &rq->arrays[0]);
    return;
}
