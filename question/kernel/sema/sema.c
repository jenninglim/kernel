#include "sema.h"

int * add_sem( semtable_t * table, int initial ) {
    table->set[table->nr_sem] = initial;
    table->nr_sem += 1;
    return &table->set[table->nr_sem - 1];
}


