#include "hilevel.h"

/* Since we *know* there will be 2 processes, stemming from the 2 user
 * programs, we can
 *
 * - allocate a fixed-size process table (of PCBs), and use a pointer
 *   to keep track of which entry is currently executing, and
 * - employ a fixed-case of round-robin scheduling: no more processes
 *   can be created, and neither is able to complete.
 */

runqueue_t rq;         //For schedulling
semtable_t semtable;   //For shared memory
int32_t * sharedmem;
pte_t  T[4096] __attribute__ ((aligned (1<<14)));

extern void     main_console();
extern uint32_t tos_shared;
extern uint32_t tos_console;
extern void     main_P3();
extern void     main_P4();
extern void     main_P5();

void hilevel_handler_pab() {
    return;
}

void hilevel_handler_dab() {
      return;
}

void hilevel_handler_rst(ctx_t* ctx) {
    
/*
    kernel_page(T); */
    // Initialised runqueue.
    
    init_rq(&rq);

    // Initialise shared mem
    sharedmem = &tos_shared;
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
    
    rq_add_console(&rq);

    sched_rq(&rq, ctx);

    /* Once the PCBs are initialised, we (arbitrarily) select one to be
     * restored (i.e., executed) when the function then returns.
     */
    
    int_enable_irq();

    access_mem(T);
    enable_MMU(T); 

    return;
}

void hilevel_handler_irq( ctx_t* ctx) {
    // Step 2: read  the interrupt identifier so we know the source.
    //mmu_set_ptr0(T);
    //mmu_flush();
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
    
    return;
}

void hilevel_handler_svc( ctx_t* ctx, uint32_t id ) {
    mmu_set_ptr0(T);
    mmu_flush();
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
            task_t * child = rq_add_clone(&rq, ctx);
            ctx->gpr[0] = child->pid;
            child->ctx.gpr[0] = 0;

            //Copy stack
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
            //TODO CHANGE SP
            //TODO SET STACK EMPTY
            break;
        }
        case 0x07 : { // 0x07 => set_prio( pid, prio )
            pid_t pid = (pid_t) ctx->gpr[0];
            int prio = ctx->gpr[1];
            rq_task_prio_change(&rq, pid, prio); 
            break; 
        }
        case 0x09 : { // 0x09 => SEM_OPEN ()
            ctx->gpr[0] = (int) add_sem(&semtable, ctx->gpr[0]);
            break;
        }
        case 0x0A : { // 0x0A => SHEM_OPEN ();
            ctx->gpr[0] = (int) memcpy(sharedmem, (void * ) ctx->gpr[0], ctx->gpr[1]);
            //TODO :: SHARED MEMORY OFFSET
            break;
        }


        default   : { // 0x?? => unknown/unsupported

            break;
        }
    }
    
    return;
}
