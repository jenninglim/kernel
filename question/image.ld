SECTIONS {
/* assign load address (per  QEMU) */
  .       =     0x70010000; 
/*          ------KERNEL SPACE-----------     */
    /* place text segment(s)           */
  kernel.text : {
      kernel/lolevel.o(.text)
      kernel/*(.text .rodata)
      devices/*(.text .rodata)
  }
  /* place data segment(s)           */        
  kernel.data : {
      kernel/*(.data) 
      devices/*(.data)
  }

  /* place bss  segment(s)           */        
  kernel.bss  : {
      kernel/*(.bss)
      devices/*(.bss)
  }
  /* heap for dynamic allocation */
  kernel.heap : {
      end = .;
      _heap_start = .;
      . = . + 0x00010000 ;
      _heap_end = .;
  }
  /* align       address (per AAPCS) */
  .       = ALIGN( 8 );
  /* allocate stack for irq mode     */
  .       =  0x72000000;
  tos_irq = .;
  /* allocate stack for svc mode     */
  .       =  0x72100000;
  tos_svc = .;
  /* allocate stack for abt mode     */
  .       =  0x72200000;
  tos_abt = .;
  tos_kernel = .;
  /*        -----USER SPACE----------       */
  /* place user text segment */
  user.text : {
     user/*(.text .rodata)
  }
  /* place user data segment */
  user.data : {
     user/*(.data)
  }
  /* place user bss */
  user.bss : {
     user/*(.bss) 
  }
  /* align       address (per AAPCS) */
  .       = ALIGN( 8 );
.       =  0x73000000;
tos_user_seg = .;
  /* place stack usr */
  .       =  0x75000000;
  tos_usr = .;
  /* place stack shared */
  .       =  0x75100000;

  tos_shared = .;
  /* allocate stack for console */
  .       =  0x75200000;
  tos_console = .;
}
