#include "shared.h"

int32_t * add_sem( shared_t * sm, uint32_t initial ) {
    uint32_t temp = initial;
    return add_shared(sm, &temp, sizeof(uint32_t));
}

int32_t * add_shared(shared_t * sm, void * data, uint32_t size) {
    uint32_t * phys_addr = (uint32_t *) ((uint32_t) ADDR_SHMEM_START_P + (uint32_t) (sm->offset));
    memcpy(phys_addr, data, size);
    
    uint32_t * alloc_addr = (uint32_t *) ((uint32_t) ADDR_SHMEM_START_V + (uint32_t) (sm->offset));
    sm->offset += size;
    return alloc_addr;
}
