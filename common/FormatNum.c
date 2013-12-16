#include <stdio.h>
#include <stdlib.h>

void FormatNum( uint32_t num, char *buf, size_t maxLen )
{
    uint32_t    divisor = 1000000000;
    char       *s = buf;
    size_t      lenRemaining = maxLen - 1;

    buf[0] = '\0';

    if ( num == 0 )
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while ( divisor >= 1 )
    {
        uint32_t    numSegment = num / divisor;
        size_t      n = 0;

        if ( s == buf )
        {
            if ( num >= divisor )
            {
                n = snprintf( s, lenRemaining, "%d", numSegment );
            }
        }
        else
        {
            n = snprintf( s, lenRemaining, "%03d", numSegment );
        }
        num -= ( numSegment * divisor );

        s += n;
        lenRemaining -= n;

        if (( n > 0 ) && ( divisor > 1 ))
        {
            *s++ = ',';
            *s = '\0';
            n++;
        }

        divisor /= 1000;
    }
}

int main( int argc, char **argv )
{
    int         i;
    unsigned    num;
    char        str[20];

    for ( i = 1; i < argc; i++ )
    {
        num = strtoul( argv[i], NULL, 0 );
        FormatNum( num, str, sizeof( str ));
        printf( "%9d %10s\n", num, str );
    }

    exit( 0 );
    return 0;
}
