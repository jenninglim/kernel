#ifndef __LIBC_H
#define __LIBC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <sys/types.h>
#include <errno.h>

#include <unistd.h>

// Define a type that that captures a Process IDentifier (PID).

typedef int pid_t;

/* The definitions below capture symbolic constants within these classes:
 *
 * 1. system call identifiers (i.e., the constant used by a system call
 *    to specify which action the kernel should take),
 * 2. signal identifiers (as used by the kill system call), 
 * 3. status codes for exit,
 * 4. standard file descriptors (e.g., for read and write system calls),
 * 5. platform-specific constants, which may need calibration (wrt. the
 *    underlying hardware QEMU is executed on).
 *
 * They don't *precisely* match the standard C library, but are intended
 * to act as a limited model of similar concepts.
 */

#define SYS_YIELD     ( 0x00 )
#define SYS_WRITE     ( 0x01 )
#define SYS_READ      ( 0x02 )
#define SYS_FORK      ( 0x03 )
#define SYS_EXIT      ( 0x04 )
#define SYS_EXEC      ( 0x05 )
#define SYS_KILL      ( 0x06 )
#define SYS_PRIO           ( 0x07 )
#define SYS_PID            ( 0x08 )
#define SYS_SEM_OPEN          ( 0x09 )
#define SYS_SHMEM_OPEN     ( 0x0A ) // Initialise Shared Memory
#define SYS_SHMEM_CLOSE    ( 0x0B )
#define SYS_SHMEM_READ     ( 0x0C )
#define SYS_SHMEM_WRITE    ( 0x0D )

#define SIG_TERM      ( 0x00 )
#define SIG_QUIT      ( 0x01 )

#define EXIT_SUCCESS  ( 0 )
#define EXIT_FAILURE  ( 1 )

// convert ASCII string x into integer r
extern int  atoi( char* x        );
// convert integer x into ASCII string r
extern void itoa( char* r, int x );

// cooperatively yield control of processor, i.e., invoke the scheduler
extern void yield();

// write n bytes from x to   the file descriptor fd; return bytes written
extern int write( int fd, const void* x, size_t n );
// read  n bytes into x from the file descriptor fd; return bytes read
extern int  read( int fd,       void* x, size_t n );

// perform fork, returning 0 iff. child or > 0 iff. parent process
extern int  fork();
// perform exit, i.e., terminate process with status x
extern void exit(       int   x );
// perform exec, i.e., start executing program at address x
extern void exec( const void* x );

// signal process identified by pid with signal x
extern int  kill( pid_t pid, int x );

// perform prority change.
extern void set_prio( int pid, int prio );

// get pid
extern int get_pid();

//opennn
extern int sem_open( int value);

// increament a semaphore.
extern void sem_post(int * sem);

extern void sem_wait(int * sem);

// open shared memory.
extern int shm_open( void * data, int size );

extern int sem_write(int sema_id, void * data);

extern char * sem_read( int sema_id);



#endif
