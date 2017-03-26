#ifndef __SEMA_H
#define __SEMA_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int sema_id;
    char * data;
    int lock; 
} sema_t;

typedef struct {
    int set[100];
    int nr_sem;
} semtable_t;

void SEMA_INIT (sema_t * sema, int sema_id, char * data, int lock);

int * add_sem( semtable_t * table, int initial );
#endif
