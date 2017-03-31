#include "P8.h"

int fcall( uint32_t x ) {
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

void main_P8() {
    volatile int vairable= 1000;
    volatile int * pointer = &vairable;
    fcall( 1000 );
    exit( EXIT_SUCCESS );
}
