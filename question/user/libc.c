#include "libc.h"

int  atoi( char* x        ) {
  char* p = x; bool s = false; int r = 0;

  if     ( *p == '-' ) {
    s =  true; p++;
  }
  else if( *p == '+' ) {
    s = false; p++;
  }

  for( int i = 0; *p != '\x00'; i++, p++ ) {
    r = s ? ( r * 10 ) - ( *p - '0' ) :
            ( r * 10 ) + ( *p - '0' ) ;
  }

  return r;
}

void itoa( char* r, int x ) {
  char* p = r; int t, n;

  if( x < 0 ) {
    p++; t = -x; n = 1;
  }
  else {
         t = +x; n = 1;
  }

  while( t >= n ) {
    p++; n *= 10;
  }

  *p-- = '\x00';

  do {
    *p-- = '0' + ( t % 10 ); t /= 10;
  } while( t );

  if( x < 0 ) {
    *p-- = '-';
  }

  return;
}

void yield() {
  asm volatile( "svc %0     \n" // make system call SYS_YIELD
              :
              : "I" (SYS_YIELD)
              : );

  return;
}

int write( int fd, const void* x, size_t n ) {
  int r;

  asm volatile( "mov r0, %2 \n" // assign r0 = fd
                "mov r1, %3 \n" // assign r1 =  x
                "mov r2, %4 \n" // assign r2 =  n
                "svc %1     \n" // make system call SYS_WRITE
                "mov %0, r0 \n" // assign r  = r0
              : "=r" (r) 
              : "I" (SYS_WRITE), "r" (fd), "r" (x), "r" (n)
              : "r0", "r1", "r2" );

  return r;
}

int  read( int fd,       void* x, size_t n ) {
  int r;

  asm volatile( "mov r0, %2 \n" // assign r0 = fd
                "mov r1, %3 \n" // assign r1 =  x
                "mov r2, %4 \n" // assign r2 =  n
                "svc %1     \n" // make system call SYS_READ
                "mov %0, r0 \n" // assign r  = r0
              : "=r" (r) 
              : "I" (SYS_READ),  "r" (fd), "r" (x), "r" (n) 
              : "r0", "r1", "r2" );

  return r;
}

int fork() {
  int r;

  asm volatile( "svc %1     \n" // make system call SYS_FORK
                "mov %0, r0 \n" // assign r  = r0 
              : "=r" (r) 
              : "I" (SYS_FORK)
              : "r0" );

  return r;
}

void exit( int x ) {
  asm volatile( "mov r0, %1 \n" // assign r0 =  x
                "svc %0     \n" // make system call SYS_EXIT
              :
              : "I" (SYS_EXIT), "r" (x)
              : "r0" );

  return;
}

void exec( const void* x ) {
  asm volatile( "mov r0, %1 \n" // assign r0 = x
                "svc %0     \n" // make system call SYS_EXEC
              :
              : "I" (SYS_EXEC), "r" (x)
              : "r0" );

  return;
}

int kill( int pid, int x ) {
  int r;

  asm volatile( "mov r0, %2 \n" // assign r0 =  pid
                "mov r1, %3 \n" // assign r1 =    x
                "svc %1     \n" // make system call SYS_KILL
                "mov %0, r0 \n" // assign r0 =    r
              : "=r" (r) 
              : "I" (SYS_KILL), "r" (pid), "r" (x)
              : "r0", "r1" );

  return r;
}

void set_prio( int pid, int prio  ) {
  asm volatile( "mov r0, %1 \n" // assign r0 =  pid
                "mov r1, %2 \n" // assign r1 = prio
                "svc %0     \n" // make system call SYS_KILL
              : 
              : "I" (SYS_PRIO), "r" (pid), "r" (prio)
              : "r0" , "r1" );
  return;
}

int get_pid() {
  int r;
  asm volatile( "svc %1 \n" // assign r0 =  pid
                "mov %0, r0 \n" // assign r1 = prio
              : "=r" (r)
              : "I" (SYS_PID)
              : "r0" );
  return r;
}

int * sem_open () {
    int * r;
    asm volatile( "svc %1 \n"
                  "mov %0, r0 \n"
                  : "=r" (r)
                  : "I" (SYS_SEMOP)
                  : "r0");
    return r;
}

void sem_post(int * sem) {
    asm volatile( "ldrex r1, [%0] \n"
                  "add r1, r1, #1 \n"
                  "strex r2, r1, [%0] \n"
                  "cmp r2, #0 \n"
                  "bne sem_post \n"
                  "dmb \n"
                  "bx lr \n"
                : 
                : "r" (sem)
                : "r1", "r2");
    return;
}

void sem_wait(int * sem) {
    asm volatile( "ldrex r1, [%0] \n"
                  "cmp r1, #0 \n"
                  "beq sem_wait \n"
                  "sub r1, r1, #1 \n"
                  "strex r2, r1, [%0] \n"
                  "cmp r2, #0 \n"
                  "bne sem_wait \n"
                  "dmb \n"
                  "bx lr \n"
                : 
                : "r" (sem)
                : "r1", "r2");
    return;
}

register char * stack_ptr asm ("sp");

/* Heap limit returned from SYS_HEAPINFO Angel semihost call.  */
uint __heap_limit = 0xcafedead;

int _sbrk (int incr) {
  extern char end asm ("end"); /* Defined by the linker.  */
  static char * heap_end;
  char * prev_heap_end;

  if (heap_end == NULL)
    heap_end = & end;

  prev_heap_end = heap_end;

  if ((heap_end + incr > stack_ptr)
      || (__heap_limit != 0xcafedead && (int) heap_end + incr > __heap_limit))
    {
      /* Some of the libstdc++-v3 tests rely upon detecting
	 out of memory errors, so do not abort here.  */
#if 0
      extern void abort (void);

      _write (1, "_sbrk: Heap and stack collision\n", 32);

      abort ();
#else
      errno = ENOMEM;
      return -1;
#endif
    }

  heap_end += incr;

  return (int) prev_heap_end;
}
