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

#include "adc.h"
#include "Log.h"
#include "Timer.h"
#include "UART.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define LED_DDR        DDRD
#define LED_PORT       PORTD
#define LED_MASK       ( 1 << 3  )

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

    ADC_Init( ADC_PRESCALAR_AUTO );

    LED_DDR |= LED_MASK;

    // Note: CFG_LOG_USE_STDIO is defined in Config.h

#if CFG_LOG_USE_STDIO

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout.

    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
    LogInit( stdout );

#define GET_CHAR    getchar
#else
#define GET_CHAR    UART0_GetChar

#endif

    Log( "*****\n" );
    Log( "***** ADC Test program\n" );
    Log( "*****\n" );

    // Sleep for a bit to allow the ADC to initialize

    WaitForTimer0Rollover();    
    WaitForTimer0Rollover();    // 10 msec

    while( 1 )
    {
        LED_PORT ^= LED_MASK;

        Log( "ADC %4d %4d %4d %4d %4d %4d %4d %4d\n", 
             ADC_Read( 0 ), ADC_Read( 1 ),
             ADC_Read( 2 ), ADC_Read( 3 ),
             ADC_Read( 4 ), ADC_Read( 5 ),
             ADC_Read( 6 ), ADC_Read( 7 ));

        // Tick rate is 100/sec so waiting for 100 waits for 1 sec

        for ( i = 0; i < 100; i++ ) 
        {
            WaitForTimer0Rollover();    

            if ( UART0_IsCharAvailable() )
            {
                char    ch;

                ch = GET_CHAR();

                Log( "Read: '%c'\n", ch );

                if ( ch == ' ' )
                {
                    Log( "*** Press a key to continue\n" );
                    ch = GET_CHAR();
                    Log( "*** Continuing...\n" );
                }
            }
        }
    }

    return 0;
}

