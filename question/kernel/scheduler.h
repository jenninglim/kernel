#ifndef __SCHEDULER_H
#define __SCHEDULER_H

// Include functionality relating to newlib

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <string.h>

/*
 * task_struct represents 
 */

typedef struct {
    volatile long state; // program state
    int state;
    pcb_t pcb;
} task_struct;

#endif
