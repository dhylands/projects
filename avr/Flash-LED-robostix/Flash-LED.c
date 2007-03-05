/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/

#include <avr/io.h>
#include <stdio.h>

#include "Hardware.h"
#include "Timer.h"
#include "UART.h"

uint8_t gADC[ 8 ];

int main(void)
{
    int     i;
    int     led = 0;
    FILE   *u0;
    FILE   *u1;

    InitHardware();

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

#if defined( __AVR_LIBC_VERSION__ )
    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
    u1 = fdevopen( UART1_PutCharStdio, NULL );
#else
    u0 = fdevopen( UART0_PutCharStdio, UART0_GetCharStdio, 0 );
    u1 = fdevopen( UART1_PutCharStdio, NULL, 0 );
#endif

    printf( "*****\n" );
    printf( "***** Flash-LED program\n" );
    printf( "*****\n" );

    fprintf( u1, "***** Output written to UART1 *****\n" );
    
    while( 1 )
    {
        // Turn all of the LEDs off

        LED_OFF( RED );
        LED_OFF( BLUE );
        LED_OFF( YELLOW );

        switch ( led )
        {
            case    0:  LED_ON( RED );      printf( "R " );      break;
            case    1:  LED_ON( BLUE );     printf( "B " );     break;
            case    2:  LED_ON( YELLOW );   printf( "Y " );   break;
        }

        if ( ++led > 2 )
        {
            led = 0;
        }

        // Tick rate is 100/sec so waiting for 100 waits for 1 sec

        for ( i = 0; i < 100; i++ ) 
        {
            WaitForTimer0Rollover();

            if ( UART0_IsCharAvailable() )
            {
                char    ch = getchar();

                printf( "Read: '%c'\n", ch );

                if ( ch == ' ' )
                {
                    printf( "*** Press a key to continue\n" );
                    ch = getchar();
                    printf( "*** Continuing...\n" );
                }
            }
        }
    }

    return 0;
}

