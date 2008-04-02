/****************************************************************************
*
*   Copyright (c) 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   simple-flasher.c
*
*   @brief  Simple LED flasher for the robostix
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#define F_CPU   16000000UL

#include <avr/io.h>
#include <util/delay.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

//--------------------------------------------------------------------------
// LED Constants

#define RED_LED_PIN     4
#define RED_LED_MASK    ( 1 << RED_LED_PIN )
#define RED_LED_DDR     DDRG
#define RED_LED_PORT    PORTG

#define BLUE_LED_PIN    3
#define BLUE_LED_MASK   ( 1 << BLUE_LED_PIN )
#define BLUE_LED_DDR    DDRG
#define BLUE_LED_PORT   PORTG

#define YELLOW_LED_PIN  4
#define YELLOW_LED_MASK ( 1 << YELLOW_LED_PIN )
#define YELLOW_LED_DDR  DDRB
#define YELLOW_LED_PORT PORTB

//--------------------------------------------------------------------------
// Some convenience macros to turn the LEDs on/off.
//
//  Usage:  LED_ON( BLUE );
//
// to turn on the blue LED.
//
// Note: Setting the pin to 0 turns the LED on.

#define LED_ON( color )     do { color ## _LED_PORT &= ~color ## _LED_MASK; } while (0)
#define LED_OFF( color )    do { color ## _LED_PORT |= color ## _LED_MASK; } while (0)

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Simple LED flasher
*/

int main( void )
{
    int     i;
    int     led = 0;

    // Initialize the two LED pins

    RED_LED_DDR    |= RED_LED_MASK;
    YELLOW_LED_DDR |= YELLOW_LED_MASK;
    BLUE_LED_DDR   |= BLUE_LED_MASK;

    while ( 1 )
    {
        // Turn all of the LEDs off

        LED_OFF( RED );
        LED_OFF( BLUE );
        LED_OFF( YELLOW );

        switch ( led )
        {
            case    0:  LED_ON( RED );      break;
            case    1:  LED_ON( BLUE );     break;
            case    2:  LED_ON( YELLOW );   break;
        }
        if ( ++led > 2 )
        {
            led = 0;
        }

        for ( i = 0; i < 100; i++ )
        {
            _delay_ms( 1 );
        }
    }
}
