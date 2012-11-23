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

#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define LED_DDR        DDRD
#define LED_PORT       PORTD
#define LED_MASK       ( 1 << 6  )

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Simple LED flasher
*/

int main( void )
{
    clock_prescale_set( clock_div_1 );  // Sets clock to 16 MHz

    // Initialize the two LED pins

    LED_DDR |= LED_MASK;

    while ( 1 )
    {
        LED_PORT |=  LED_MASK;
        _delay_ms( 100 );

        LED_PORT &= ~LED_MASK;
        _delay_ms( 100 );

        LED_PORT |=  LED_MASK;
        _delay_ms( 100 );

        LED_PORT &= ~LED_MASK;
        _delay_ms( 700 );
    }
}
