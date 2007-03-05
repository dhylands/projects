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
*   @file   bioloid.c
*
*   @brief  Program which interfaces between the gumstix and the bioloid
*           AX-12 servos.
*
*           The AX-12 servos use a half-duplex protocol. We use UART-0
*           to connect to the gumstix, and UART-1 to connect to the servos.
*
*           To support the half duplex, we assume that the UART-1 Rx and Tx
*           are wired together. 
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Config.h"

#include "bioloid-uart.h"
#include "CBUF.h"
#include "Delay.h"
#include "Timer.h"
#include "UART.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define	START_OF_PACKET		0xFF	// Two in a row is the start of a packet

//--------------------------------------------------------------------------
// LED Constants

#if !defined( ROBOSTIX )
#  error ROBOSTIX should be defined to be 0 or 1
#endif

#if ROBOSTIX

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

#else

#define RED_LED_PIN     2
#define RED_LED_MASK    ( 1 << RED_LED_PIN )
#define RED_LED_DDR     DDRC
#define RED_LED_PORT    PORTC

#define BLUE_LED_PIN    1
#define BLUE_LED_MASK   ( 1 << BLUE_LED_PIN )
#define BLUE_LED_DDR    DDRC
#define BLUE_LED_PORT   PORTC

#define YELLOW_LED_PIN  0
#define YELLOW_LED_MASK ( 1 << YELLOW_LED_PIN )
#define YELLOW_LED_DDR  DDRC
#define YELLOW_LED_PORT PORTC

#endif

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
#define LED_TOGGLE( color ) do { color ## _LED_PORT ^= color ## _LED_MASK; } while (0)

/* ---- Private Variables ------------------------------------------------ */

static  volatile    uint8_t     gTxActivity;
static  volatile    uint8_t     gRxActivity;

#define	STATE_OFF		0
#define	STATE_FIRST_FF	1
#define	STATE_ON		2

static	uint8_t		gState = STATE_OFF;

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Main loop, which really just manages the activity LEDs
*/

int main( void )
{
    int     hearbeat;

    PORTA   = 0xFF;   // enable pullups for inputs
    PORTB   = 0xFF;
    PORTC   = 0xFF;
    PORTD   = 0xFF;
    PORTE   = 0xFF;
    PORTF   = 0xFF;
    PORTG   = 0xFF;

    // TOSC1 & TOSC2 are connected to the Red/Blue LEDs so we need to set
    // AS0 to 0. AS0 is the only writable bit.

    ASSR = 0;

    // Set LEDs outputs

	YELLOW_LED_DDR |= YELLOW_LED_MASK;
	RED_LED_DDR    |= RED_LED_MASK;
	BLUE_LED_DDR   |= BLUE_LED_MASK;

    InitUART();
    BLD_InitUART();

    InitTimer();

    UART0_PutStr( "*****\n" );
    UART0_PutStr( "***** bioloid serial test\n" );
    UART0_PutStr( "*****\n" );

    sei();  // Enable interrupts.

    // The foreground copies data from one FIFO to the other

    hearbeat = 0;
    while ( 1 )
    {
        uint8_t i;

        // Delay for 50 msec

        for ( i = 0; i < 5; i++ ) 
        {
            tick_t prevCount = gTickCount;

            // Each tick is 10 msec

            while ( gTickCount == prevCount )
            {
                if ( UART0_IsCharAvailable() )
                {
                    gTxActivity = 1;
                    UART1_PutChar( UART0_GetChar() );
                }

                if ( UART1_IsCharAvailable() )
                {
                    gRxActivity = 1;
                    UART0_PutChar( UART1_GetChar() );
                }
            }
        }

        if ( gRxActivity )
        {
            LED_TOGGLE( RED );

            gRxActivity = 0;
        }
        else
        {
            LED_OFF( RED );
        }

        if ( gTxActivity )
        {
            LED_TOGGLE( BLUE );

            gTxActivity = 0;
        }
        else
        {
            LED_OFF( BLUE );
        }

        if ( ++hearbeat >= 4 )
        {
            LED_TOGGLE(  YELLOW );
            hearbeat = 0;
        }
    }

    return 0;

} // main

