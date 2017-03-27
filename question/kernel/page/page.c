#include "page.h"

void INIT_TABLE(pt_t * pt) {
   for (int i = 0; i < NR_PAGES; i++ ) {
        pt->table[i] = 0; //TODO decide here.
   }
   return;
}
/*
void enable_MMU(pt_t * pt) {
  mmu_set_ptr0( pt->table );

  mmu_set_dom( 0, 0x3 ); // set domain 0 to 11_{(2)} => manager (i.e., not checked)
  mmu_set_dom( 1, 0x1 ); // set domain 1 to 01_{(2)} => client  (i.e.,     checked)

  mmu_enable();

}*/


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


void user_page(pte_t * pt, int page_no) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt[i] = ((pte_t) (i) << 20) | 0x00000002;
        pt[i] &= DOMAIN_MASK;
        if (i >= 0 && i <= 300) {
            pt[i] |= DOMAIN_MANAGER;
        }
        else if ( i >= 1826) {
            pt[i] |= DOMAIN_MANAGER;
        }
        else {
            pt[i] |= DOMAIN_CLIENT;
            pt[i] &= ACCESS_MASK;
            pt[i] |= AP_PRW_URO;
        }
    }
}
