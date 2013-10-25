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

/* ---- Include Files ---------------------------------------------------- */

#include <avr/io.h>
#include <stdio.h>

#include "Config.h"
#include "Timer.h"
#include "UART.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Uart Test
*/

int main(void)
{
    int     i;

    InitTimer();
    InitUART();

    LED_DDR |= LED_MASK;

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout.

    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

    printf( "*****\n" );
    printf( "***** Uart Test program\n" );
    printf( "*****\n" );

    while( 1 )
    {
        LED_PORT ^= LED_MASK;

        printf( "." );

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

