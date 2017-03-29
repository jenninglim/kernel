SECTIONS {
/* assign load address (per  QEMU) */
  .       =     0x70010000; 
/*          ------KERNEL SPACE-----------     */
    /* place text segment(s)           */
  .kernel_text : {
      kernel/lolevel.o(.text)
      kernel/*(.text .rodata)
      device/*(.text .rodata)
  }
  /* place data segment(s)           */        
  .kernel_data : {
      kernel/hilevel.o(.data)
      kernel/*(.data) 
      device/*(.data)
  }

  /* place bss  segment(s)           */        
  .kernel_bss  : {
      kernel/*(COMMON)
      kernel/*(.bss)
      device/*(COMMON)
      device/*(.bss)
  }
  /* heap for dynamic allocation */
  .kernel_heap : {
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
  /*        -----USER SPACE----------       */
  /* place user text segment */
  .user_text : {
     user/*(.text .rodata)
  }
  /* place user data segment */
  .user_data : {
     user/*(.data)
  }
  /* place user bss */
  .user_bss : {
     user/*(COMMON)
     user/*(.bss) 
  }
  /* align       address (per AAPCS) */
  .       = ALIGN( 8 );
  .       =  0x74000000;
  /* place stack usr */
  tos_usr = .;
  /* place stack shared */
  .       =  0x74100000;
  tos_shared = .;
}
