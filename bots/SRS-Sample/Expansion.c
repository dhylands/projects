/****************************************************************************
*
*   Copyright (c) 2005 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
/**
*
*   @file    Expansion.c
*
*   @brief   Contains the code for communicating with the expansion board
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Expansion.h"

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */

#define INPUT_ENABLE_PORT   PORTB
#define INPUT_ENABLE_PIN    0
#define INPUT_ENABLE_MASK   ( 1 << INPUT_ENABLE_PIN )

#define OUTPUT_ENABLE_PORT  PORTB
#define OUTPUT_ENABLE_PIN   1
#define OUTPUT_ENABLE_MASK  ( 1 << OUTPUT_ENABLE_PIN )

/* ---- Private Variables ------------------------------------------------- */

static  uint16_t    gLastXferOut = 0;
static  uint16_t    gLastXferIn  = 0;


/* ---- Private Function Prototypes --------------------------------------- */
/* ---- Functions --------------------------------------------------------- */

uint16_t EXP_TransferWord( uint16_t outVal, uint16_t outMask )
{
    uint16_t    inVal;
    uint8_t     sreg;

    outVal &= outMask;  // Make sure they don't sneak any extra bits in :)
    outVal |= ( gLastXferOut & ~outMask );  // Preserve bits not being modified.

    // This routine could be called both from an ISR and the foreground, so
    // disable interrupts so only one place is accessing the hardware.

    sreg = SREG;    
    cli();

    // Enable the Output on the 589's

    INPUT_ENABLE_PORT |= INPUT_ENABLE_MASK;

    // Send the MSB

    SPDR = (uint8_t)(( outVal & 0xff00 ) >> 8 );

    // Wait for the transmission to complete

    while (( SPSR & ( 1 << SPIF )) == 0 )
    {
        ;
    }

    // Read the MSB

    inVal = SPDR;
    inVal <<= 8;

    // Send the LSB

    SPDR = (uint8_t)( outVal & 0x00ff );

    // Wait for the transmission to complete

    while (( SPSR & ( 1 << SPIF )) == 0 )
    {
        ;
    }

    // Read the LSB

    inVal |= SPDR;

    // Turn off the 589 enable

    INPUT_ENABLE_PORT &= ~INPUT_ENABLE_MASK;

    // Transfer the newly shifted output value in the output registers on the 595's

    OUTPUT_ENABLE_PORT |=  OUTPUT_ENABLE_MASK;
    OUTPUT_ENABLE_PORT &= ~OUTPUT_ENABLE_MASK;

    gLastXferOut = outVal;
    gLastXferIn  = inVal;

    SREG = sreg;    // Restore interrupts to previous condition

    return inVal;
}

