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
/**
*
*   @file   flasher.c
*
*   @brief  Simple LED flasher for the Baby Orangatang board from Pololu
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Delay.h"
#include "Hardware.h"
#include "UART.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Simple LED flasher
*/

int main( void )
{
    InitHardware();

#if 1
    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

    printf( "*****\n" );
    printf( "***** Baby Orangatang UART program\n" );
    printf( "*****\n" );
#endif

    // Initialize the two LED pins

    while ( 1 )
    {
        LED_TOGGLE( BLUE );

        printf( "tick " );

        ms_spin( 500 );

        LED_TOGGLE( BLUE );

        printf( "tock\n" );

        ms_spin( 500 );
    }
}
