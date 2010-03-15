#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

uint32_t int_pow( uint32_t x, uint32_t n )
{
    uint32_t    z;
    uint32_t    result;

    if ( x < 2 )
    {
        // 0 ^ n = 0, regardless of n
        // 1 ^ n = 1, regardless of n

        return x;
    }

    if ( n > 32 )    // 2 ^ 32  can't fit, so anything > 2 obviously can't fit
    {
        // Error

        return 0;
    }

    z = x;
    result = 1;

    // Note: We don't deal with overflow in 'z * z', nor in 'result * z'.
    //       z * z is easy to detect, 65536 ^ 2 overflows a 32-bit result.
    //  however, result * z is a little trickier to detect.
    //
    // For example, 5 ^ 13 fits in a 32-bit result, but 5 ^ 14 doesn't.
    //
    // 5 ^ 13 requires multiplying  3125 x 390625 = 1220703125 which is < 2 ^ 32
    // 5 ^ 14 requires multiplying 15625 x 390625 = 6103515625 which is > 2 ^ 32
    //
    // One solution is to upcast to 64-bit.
    //
    // The other is to break down and do the multiplication in parts (i.e. break it into 
    // two 16 x 32-bit multiplies, and then just be careful about overflow in the addition)

    for ( ; n != 0; n >>= 1 )
    {
        if (( n & 1 ) != 0 )
        {
            result = result * z;
        }
        z = z * z;
    }

    return result;
}

int main( int argc, char **argv )
{
    uint32_t    x, n;

    if ( argc != 3)
    {
        fprintf( stderr, "Usage: power x n\n" );
        exit( 1 );
    }

    x = atoi( argv[1] );
    n = atoi( argv[2] );

    printf( "pow( %u, %u ) = %f\n", x, n, pow( (double)x, (double)n ));
    printf( "int_pow( %u, %u ) = %u\n", x, n, int_pow( x, n ));

    exit( 0 );
    return 0;
}
