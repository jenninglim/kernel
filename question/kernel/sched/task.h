#ifndef __TASK_H
#define __TASK_H

#define task_current_entry(ptr) \
        list_entry(ptr, task_t, node)

#define task_next_entry(ptr) \
        list_first_entry(ptr, task_t, node)

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "list/list.h"
#include "page/page.h"

/*
 * Structure equivalent to PCB table
 */
typedef struct {
    volatile int state;        // program state
    int prio;                  // Priority.
    pid_t pid;                 // PID
    ctx_t ctx;                 // Context.
    list_head node;            // DLL Head
    pte_t T[4096] __attribute__ ((aligned (1 << 14)));   // Page Table.
} task_t;

// Initialise task structure.
void TASK_INIT(task_t * new_task, uint32_t pc, uint32_t offset);

// Set task state.
void set_task_state(task_t * task, int state);

// Set task priority.
void set_task_prio(task_t * task, int prio);

// Set task next of head.
void add_task_next(task_t * new, list_head * head);

// Add task previous of head.
void add_task_last(task_t * new, list_head * head);

// Set task PID.
void set_task_pid(task_t * task, int32_t pid);

// Create Task Clone.
void task_clone(task_t * new, ctx_t * source, uint32_t sp);

#endif
