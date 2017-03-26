#include "P6.h"
int isPrime( uint32_t x ) {
  if ( !( x & 1 ) || ( x < 2 ) ) {
    return ( x == 2 );
  }

  for( uint32_t d = 3; ( d * d ) <= x ; d += 2 ) {
    if( !( x % d ) ) {
      return 0;
    }
  }

  return 1;
}

void printInt(int id, state_e state) {
    char * str;
    char message[10]; 
    itoa(str, id);
    switch (state) {
        case T:
            strcpy(message, "THINKI: ");
            break;
        case H:
            strcpy(message, "HUNGRY: ");
            break;
        case E:
            strcpy(message, "EATING: ");
            break;
    }
    strcat(message, str);
    write( STDOUT_FILENO, "\n", 2);
    write( STDOUT_FILENO, message, 10);
    write( STDOUT_FILENO, "\n", 2);
    return;
}

void printState(state_e *stateTable) {
    write( STDOUT_FILENO, "\n", 2);
    for (int i = 0; i < NO_OF_PHILOSOPHERS; i++) {


    }
    return;
}
int left (int i ) {
    return (i - 1 + NO_OF_PHILOSOPHERS) % NO_OF_PHILOSOPHERS;
}

int right (int i ) {
    return (i + 1) % NO_OF_PHILOSOPHERS;
}

void test(int id, state_e * stateTable,int ** semaphore) {
    if (stateTable[left(id)] != E && 
            stateTable[right(id)] != E &&
            stateTable[id] == H)  {
        stateTable[id] = E;
        sem_post(&(*semaphore)[id]);
    }
}


void think() {
    int i = 0;
    while (i < 10000) {
        for (int j = 0; j < 300; j ++ ) {
            isPrime(i);
        }
        i++;
    }
}

void philosophers(int id,int * me, state_e * statetable, int * semtable[]) {
    while (1) {
        switch (statetable[id]) {
            case E:
                printInt((int) (id + 1), statetable[id]);
                yield();
                sem_wait(me);
                statetable[id] = T;
                test(left(id), statetable, semtable);
                test(right(id), statetable, semtable);
                sem_post(me);
                break;
            case T:
                printInt((int) (id + 1), statetable[id]);
                //think();
                yield();
                sem_wait(me);
                statetable[id] = H;
                break;
            case H:
                //printInt((int) (id + 1), statetable[id]);
                test(id, statetable, semtable);
                sem_post(me);
                sem_wait(&(* semtable) [id]);
                break;

        }
    }
        
}
    int * philo[NO_OF_PHILOSOPHERS];

void main_P6() {
    state_e state[NO_OF_PHILOSOPHERS];
    for (int i = 0; i < NO_OF_PHILOSOPHERS; i++ ) {
        state[i] = 0;
    }

    state_e * table;
    table = shm_open(state, sizeof(state));
    
    int pid = 0;
    int * sem_array = sem_open(1);

    //int * philo[NO_OF_PHILOSOPHERS];
    for (int i = 0; i < NO_OF_PHILOSOPHERS; i++ ) {
        philo[i] = sem_open(0);
    }
    for (int i = 0; i < NO_OF_PHILOSOPHERS; i ++ ) {
        pid = fork(); 
        if ( pid == 0 ) {
            philosophers( i, sem_array, table, philo );
        }
    }
    exit( EXIT_SUCCESS );
}
