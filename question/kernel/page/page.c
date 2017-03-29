#include "page.h"

extern uint32_t tos_usr;
extern uint32_t tos_kernel;

void INIT_TABLE(pte_t * pt) {
   for (int i = 0; i < NR_PAGES; i++ ) {
        pt[i] = 0; //TODO decide here.
   }
   return;
}

void set_domain_range() {
    //Set doamin access
    mmu_set_dom( 0, 0x3 ); // set domain 0 to 11_{(2)} => manager (i.e., not checked)
    mmu_set_dom( 1, 0x1 ); // set domain 1 to 01_{(2)} => client  (i.e.,     checked)
    

}

void enable_MMU(pte_t * pt) {
    //Set translation table.
    mmu_set_ptr0( pt );
    
    set_domain_range();
    
    //Enable MMU
    mmu_enable();
}

/*
void kernel_page(pt_t * pt) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt->table[i] = ((pte_t) (i) << 20);

        if (i >= 700 && i < 828) {
            pt->table[i] &= DOMAIN_MASK;
            pt->table[i] &= ACCESS_MASK;
            pt->table[i] |= AP_PRW_URW;
        }
    }
}
*/

void access_mem(pte_t * pt) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt[i] = ((pte_t) (i) << 20) | 0x00000002;
        pt[i] &= DOMAIN_MASK;
        pt[i] |= DOMAIN_MANAGER;
    }
}

void user_page(pte_t * pt, uint32_t pid) {
    uint32_t page_no = (sp & 0xFFF00000 >> 20);
    for (int i = 0; i < NR_PAGES; i ++) {
        if (i < 0x720 ) {
            pt[i] = ((pte_t) (i) << 20) | 0x00000002;
            pt[i] &= DOMAIN_MASK;
            if (i >= 0 && i <= 300) {
                pt[i] |= DOMAIN_MANAGER;
            }
            /*
             * Stack access
             */
            else if ( i >= (((uint32_t) &tos_kernel) >> 20) - 1) {
                pt[i] |= DOMAIN_MANAGER;
            }
            /*
             * USER domain access
             */
            else if ( i <= (((uint32_t) &tos_usr) >> 20) -1 && i >= (((uint32_t) &tos_kernel ) >> 20) -1) {
                pt[i] |= DOMAIN_MANAGER;
            }
            else {
                pt[i] |= DOMAIN_CLIENT;
                pt[i] &= ACCESS_MASK;
                pt[i] |= AP_PRW_URO;
            }
        }
        else if (i < (&tos_user_seg >> 20)) {
            pt[i] = ((pte_t) (i) << 20) | 0x00000002;
        }
        else if (i == (&tos_user_seg)) {
            pt[i] = (((pte_t) (&tos_usr >> 20) -1 - pid) << 20) |0x00000002;
        }
        else if (i == (&tos_user_seg) + 1) {
            pt[i] = ((pte_t ) (&tos_usr >> 20)  << 20)
        }
        else if ( i >= (((uint32_t) &tos_kernel) >> 20) - 1) {
            pt[i] |= DOMAIN_MANAGER;
        }
    }
}
