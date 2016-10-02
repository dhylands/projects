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
#include <avr/interrupt.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

// PB5 is D13 on Arduino
#define LED_DDR        DDRB
#define LED_PORT       PORTB
#define LED_MASK       ( 1 << 5  )

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Spin for ms milliseconds
*/

#define LOOPS_PER_MS ( CFG_CPU_CLOCK / 1000 / 4)

static void ms_spin( unsigned short ms )
{
   if (!ms)
           return;

   /* the inner loop takes 4 cycles per iteration */
   __asm__ __volatile__ (
           "1:                     \n"
           "       ldi r26, %3     \n"
           "       ldi r27, %2     \n"
           "2:     sbiw r26, 1     \n"
           "       brne 2b         \n"
           "       sbiw %1, 1      \n"
           "       brne 1b         \n"
           : "+w" (ms)
           : "w" (ms), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
           : "r26", "r27"
           );

} // ms_spin

//***************************************************************************
/**
*   Simple LED flasher
*/

int main( void )
{
    // Initialize the LED pin

    LED_DDR |= LED_MASK;

    while ( 1 )
    {
        LED_PORT |=  LED_MASK;
        ms_spin( 100 );
        LED_PORT &= ~LED_MASK;
        ms_spin( 100 );
        LED_PORT |=  LED_MASK;
        ms_spin( 100 );
        LED_PORT &= ~LED_MASK;
        ms_spin( 700 );
    }
}
