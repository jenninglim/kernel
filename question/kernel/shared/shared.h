#ifndef __SHARED_H
#define __SHARED_H

#define ADDR_SHMEM_START_V 0x73100000  //Start of shared memory.
#define ADDR_SHMEM_START_P 0x74000000  //Start of shared memory physical mapping.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


// Shared Memory structure.
typedef struct {
    uint32_t offset; //Next offset of available memory
} shared_t;

// Add semaphore to shared memory.
int32_t * add_sem( shared_t * sm, uint32_t initial );

// Add shared memory segment.
int32_t * add_shared(shared_t * sm, void * data, uint32_t size);

#endif
