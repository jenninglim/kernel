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

void enable_MMU(pte_t * pt) {
    mmu_set_ptr0( pt );
    
    mmu_set_dom( 0, 0x3 ); // set domain 0 to 11_{(2)} => manager (i.e., not checked)
    mmu_set_dom( 1, 0x1 ); // set domain 1 to 01_{(2)} => client  (i.e.,     checked)
    
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

void kernel_page(pte_t * pt) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt[i] = ((pte_t) (i) << 20) | 0x00000002;
        
        if (i >= 700 && i < 828) {
            pt[i] &= DOMAIN_MASK;
            pt[i] &= ACCESS_MASK;
            pt[i] |= AP_PRW_URW;
        }
    }
}
/*
void user_page(pt_t * pt, int page_no) {
    for (int i = 0; i < NR_PAGES; i ++) {
        pt->table[i] = ((pte_t) (i) << 20);
        if ( i == page_no ) {
            pt->table[i] &= DOMAIN_MASK;
            pt->table[i] |= MEM_ACCESSIBLE;
            pt->table[i] |= AP_PRW_URW;
        }
        else if (i >= 700 && i < 828) {
            pt->table[i] &= DOMAIN_MASK;
            pt->table[i] |= MEM_ACCESSIBLE;
            pt->table[i] |= AP_PRW_URO;
        }
    }
}*/
