#ifndef __PAGE_H
#define __PAGE_H

#define NR_PAGES       4096

//AND THE MASK.
#define DOMAIN_MASK   ~0x000001E0
#define ACCESS_MASK   ~0x00008C00

//APPLY MASK BEFORE USE.
//OR THE MASK.
#define DOMAIN_CLIENT  0x00000020
#define DOMAIN_MANAGER 0x00000000 //0x000000060

#define MEM_ACCESSIBLE 0x00000010
#define AP_PRW_URW     0x00000C00
#define AP_PRW_URO     0x00000800
#define AP_PRW_UNA     0x00000400
#define AP_PNA_UNA     0x00000000

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "MMU.h"

typedef uint32_t pte_t;

typedef struct {
    pte_t table[NR_PAGES] __attribute__ ((aligned (1<<14)));
} pt_t;

void INIT_TABLE(pt_t * pt);

void enable_MMU(pte_t * pt);

void access_mem(pte_t * pt);

void user_page(pte_t * pt, int page_no);

#endif
