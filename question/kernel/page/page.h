#ifndef __PAGE_H
#define __PAGE_H

#define NR_PAGES       4096

//AND THE MASK.
#define DOMAIN_MASK   ~0x000001E0
#define ACCESS_MASK   ~0x00008C00

//APPLY MASK BEFORE USE.
//OR THE MASK.
#define DOMAIN_CLIENT  0x00000020 // Domain CLIENT.
#define DOMAIN_MANAGER 0x00000000 // Domain Manager.

#define MEM_ACCESSIBLE 0x00000010 // MEMORY ACESSABLE
#define AP_PRW_URW     0x00000C00 // Priviledge Read Write. User Read Write.
#define AP_PRW_URO     0x00000800 // Priviledge Read Write. User Read Only.
#define AP_PRW_UNA     0x00000400
#define AP_PNA_UNA     0x00000000

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "MMU.h"

// Page table entry.
typedef uint32_t pte_t;

// Enable MMU.
void enable_MMU(pte_t * pt);

// Full Access Page.
void access_mem(pte_t * pt);

// Create User Page.
void user_page(pte_t * pt, uint32_t sp);

// Enable Page.
void enable_page(pte_t * pt, pte_t * T);

// Enable Kernel Page.
void enable_kl_pg(pte_t * pt, pte_t * T, uint32_t pid);

#endif
