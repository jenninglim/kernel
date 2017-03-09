#include "hilevel.h"

#define NO_OF_PCB 10
/* Since we *know* there will be 2 processes, stemming from the 2 user
 * programs, we can
 *
 * - allocate a fixed-size process table (of PCBs), and use a pointer
 *   to keep track of which entry is currently executing, and
 * - employ a fixed-case of round-robin scheduling: no more processes
 *   can be created, and neither is able to complete.
 */

LIST_HEAD(llhead);

void dispatch(task_struct * new, ctx_t * ctx) {
    memcpy(ctx, &new->pcb.ctx, sizeof(ctx_t));
    new->state = 1;
}

void undispatch(task_struct * prev, ctx_t * ctx) {
    memcpy(&prev->pcb.ctx, ctx, sizeof(ctx_t));
    prev->state = 0;
}

void scheduler( ctx_t* ctx ) {
/*
 *round robin
 */
    list_head *node;

    list_for_each(node, & llhead) {
        if (task_current_entry(node)->state == 1) {
            undispatch(task_current_entry(node), ctx);
            dispatch(task_next_entry(node), ctx);
            return;
        }

    }
}

extern void     main_console();
extern uint32_t tos_console;
extern void     main_P3();
extern uint32_t tos_P3;
extern void     main_P4();
extern uint32_t tos_P4;
extern void     main_P5();
extern uint32_t tos_P5;

pcb_t create_pcb(pid_t pid, uint32_t cpsr, uint32_t pc, uint32_t sp) {
    pcb_t new_pcb;
    memset(&new_pcb, 0, sizeof(pcb_t));
    new_pcb.ctx.cpsr = cpsr;
    new_pcb.ctx.pc = pc;
    new_pcb.ctx.sp = sp;
    return new_pcb;
}

void hilevel_handler_rst(ctx_t* ctx) {
    
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

    pcb_t temp_pcb;
    task_struct * temp_task;
    
    /* Initialise PCBs representing processes stemming from execution of
     * the two user programs.  Note in each case that
     *
     * - the CPSR value of 0x50 means the processor is switched into USR
     *   mode, with IRQ interrupts enabled, and
     * - the PC and SP values matche the entry point and top of stack.
     */
    temp_pcb = create_pcb(4, 0x50, ( uint32_t )( &main_P5 ), ( uint32_t )( &tos_P5  ));
    temp_task = &create_task(0,1, temp_pcb);
    list_add(&temp_task.tasks, &llhead);
    
    temp_pcb = create_pcb(2, 0x50, ( uint32_t )( &main_P3 ), ( uint32_t )( &tos_P3  ));
    temp_task = &create_task(0,1, temp_pcb); 
    list_add(&temp_task2.tasks, &llhead);

    dispatch(task_current_entry(llhead.next), ctx);



    temp_pcb = create_pcb(3, 0x50, ( uint32_t )( &main_P4 ), ( uint32_t )( &tos_P4  ));
    temp_task = create_task(0,1, temp_pcb);
    list_add(&temp_task.tasks, &llhead);

        /* Once the PCBs are initialised, we (arbitrarily) select one to be
     * restored (i.e., executed) when the function then returns.
     */
    
        //int_enable_irq();
    
    return;
}

void hilevel_handler_irq( ctx_t* ctx) {
    // Step 2: read  the interrupt identifier so we know the source.
    
    uint32_t id = GICC0->IAR;
    
    // Step 4: handle the interrupt, then clear (or reset) the source.
    
    if( id == GIC_SOURCE_TIMER0 ) {
        PL011_putc( UART0, 'T', true ); TIMER0->Timer1IntClr = 0x01;
        scheduler( ctx );
        //Switch process
    }
    
    // Step 5: write the interrupt identifier to signal we're done.
    
    GICC0->EOIR = id;
    
    return;
}

void hilevel_handler_svc( ctx_t* ctx, uint32_t id ) {
    /* Based on the identified encoded as an immediate operand in the
     * instruction,
     *
     * - read  the arguments from preserved usr mode registers,
     * - perform whatever is appropriate for this system call,
     * - write any return value back to preserved usr mode registers.
     */
    
    switch( id ) {
        case 0x00 : { // 0x00 => yield()
            scheduler( ctx );
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
            //TODO
        }
        default   : { // 0x?? => unknown/unsupported
            break;
        }
    }
    
    return;
}
