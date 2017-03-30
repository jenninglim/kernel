#include "hilevel.h"

/* Since we *know* there will be 2 processes, stemming from the 2 user
 * programs, we can
 *
 * - allocate a fixed-size process table (of PCBs), and use a pointer
 *   to keep track of which entry is currently executing, and
 * - employ a fixed-case of round-robin scheduling: no more processes
 *   can be created, and neither is able to complete.
 */

shared_t shared;                                      // Shared memory.
runqueue_t rq;                                        // Scheduling structure.
pte_t  T[4096] __attribute__ ((aligned (1<<14)));     // Current page.

extern void     main_console();
extern uint32_t tos_shared;
extern uint32_t tos_console;
extern void     main_P3();
extern void     main_P4();
extern void     main_P5();

void hilevel_handler_pab() {
    PL011_putc( UART0, 'P', true );
    // TODO: Kill process.
    return;
}

void hilevel_handler_dab() {
    PL011_putc( UART0, 'D', true );
    // TODO: Kill process.
    return;
}

void hilevel_handler_rst(ctx_t* ctx) {
    
    // Initialised runqueue.
    init_rq(&rq);
    
    /* Configure the mechanism for interrupt handling by
     *
     * - configuring timer st. it raises a (periodic) interrupt for each
     *   timer tick,
     * - configuring GIC st. the selected interrupts are forwarded to the
     *   processor via the IRQ interrupt signal, then
     * - enabling IRQ interrupts.
     */
    
    TIMER0->Timer1Load  = 0x00100000; // select period = 2^20 ticks ~= 1 sec
    TIMER0->Timer1Ctrl  = 0x00000002; // select 32-bit   timer
    TIMER0->Timer1Ctrl |= 0x00000040; // select periodic timer
    TIMER0->Timer1Ctrl |= 0x00000020; // enable          timer interrupt
    TIMER0->Timer1Ctrl |= 0x00000080; // enable          timer
    
    GICC0->PMR          = 0x000000F0; // unmask all            interrupts
    GICD0->ISENABLER1  |= 0x00000010; // enable timer          interrupt
    GICC0->CTLR         = 0x00000003; // enable GIC interface
    GICD0->CTLR         = 0x00000001; // enable GIC distributor

    /* Initialise PCBs representing processes stemming from execution of
     * the two user programs.  Note in each case that
     *
     * - the CPSR value of 0x50 means the processor is switched into USR
     *   mode, with IRQ interrupts enabled, and
     * - the PC and SP values matche the entry point and top of stack.
     */
    
    // Add console to runqueue.
    rq_add_new_task(&rq,(uint32_t) (&main_console));
    
    // Scheduling runqueue.
    sched_rq(&rq, ctx);
    
    // Enable IRQ interrupt.
    int_enable_irq();
    
    // Set current page.
    memcpy(T,rq.current->T, sizeof(uint32_t) * 4096);
    
    //Enable MMU.
    enable_MMU(T);

    return;
}

void hilevel_handler_irq( ctx_t* ctx) {
    
    // FLUSH MMU and disable.
    mmu_flush();
    mmu_unable();
    
    // Step 2: read  the interrupt identifier so we know the source.
    uint32_t id = GICC0->IAR;
    
    // Step 4: handle the interrupt, then clear (or reset) the source.
    
    if( id == GIC_SOURCE_TIMER0 ) {
        PL011_putc( UART0, 'T', true ); TIMER0->Timer1IntClr = 0x01;

        //Timeslice passed
        time_passed(&rq);
        sched_update_ctx(&rq, ctx);
        sched_rq(&rq, ctx);
     }
    // Step 5: write the interrupt identifier to signal we're done.
    
    GICC0->EOIR = id;
    
    // Set current page.
    enable_page(rq.current->T, T);
    
    // Enable MMU.
    mmu_enable();
    return;
}

void hilevel_handler_svc( ctx_t* ctx, uint32_t id ) {
    
    // Disable MMU.
    mmu_unable();
    enable_kl_pg(rq.kernel_pg, T, rq.current->pid);
    mmu_enable();
    
    /* Based on the identified encoded as an immediate operand in the
     * instruction,
     *
     * - read  the arguments from preserved usr mode registers,
     * - perform whatever is appropriate for this system call,
     * - write any return value back to preserved usr mode registers.
     */
    
    switch( id ) {
        case 0x00 : { // 0x00 => yield()
            sched_update_ctx(&rq, ctx);
            current_expired(&rq);
            sched_rq(&rq, ctx);
            break;
        }
        case 0x01 : { // 0x01 => write( fd, x, n )
            int   fd = ( int   )( ctx->gpr[ 0 ] );
            char*  x = ( char* )( ctx->gpr[ 1 ] );
            int    n = ( int   )( ctx->gpr[ 2 ] );
            
            for( int i = 0; i < n; i++ ) {
                PL011_putc( UART0, *x++, true );
            }
            
            ctx->gpr[ 0 ] = n;
            break;
        }
        case 0x03 : { // 0x03 => fork()
            mmu_unable();
            task_t * child = rq_add_clone(&rq, ctx);
            ctx->gpr[0] = child->pid;
            child->ctx.gpr[0] = 0;
            mmu_enable();
            break;
        }
        case 0x04 : { // 0x04 => exit()
            task_t * removed = rq_remove_task(&rq, rq.current->pid);
            free(removed);
            sched_rq(&rq, ctx);
            break;
        }
        case 0x05 : { // 0x05 => exec( pc )
            ctx->pc = ctx->gpr[0];
            ctx->sp = ((uint32_t) 0x73100000);
            
            //TODO: SET STACK EMPTY
            break;
        }
        case 0x07 : { // 0x07 => set_prio( pid, prio )
            pid_t pid = (pid_t) ctx->gpr[0];
            int prio = ctx->gpr[1];
            rq_task_prio_change(&rq, pid, prio);
            
            //TODO: if no process found or exited.
            break; 
        }
        case 0x09 : { // 0x09 => SEM_OPEN ()
            ctx->gpr[0] = (uint32_t) add_sem( &shared, (uint32_t) ctx->gpr[0]);
            break;
        }
        case 0x0A : { // 0x0A => SHEM_OPEN ();
            ctx->gpr[0] = (uint32_t) add_shared( &shared, (void *) ctx->gpr[0], (uint32_t) ctx->gpr[1]);
            break;
        }
        default   : { // 0x?? => unknown/unsupported
            break;
        }
    }
    
    //Enable MMU.
    mmu_unable();
    enable_page(rq.current->T, T);
    mmu_enable();

    return;
}
