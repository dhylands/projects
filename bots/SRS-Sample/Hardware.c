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
*   @file    Hardware.c 
*
*   @brief   Performs hardware initialization
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "Hardware.h"
#include "a2d.h"
#include "Delay.h"
#include "Uart.h"

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */
/* ---- Functions --------------------------------------------------------- */

/****************************************************************************/
/**
*   Initializes the hardware.
*
*   By havingg all of the hardware intitialization in one function, it allows
*   us to keep track of everything.
*/

void InitHardware( void )
{
    PORTA   = PORTA_INIT;
    DDRA    = DDRA_INIT;

    PORTB   = PORTB_INIT;
    DDRB    = DDRB_INIT;

    PORTC   = PORTC_INIT;
    DDRC    = DDRC_INIT;

    PORTD   = PORTD_INIT;
    DDRD    = DDRD_INIT;

    ASSR    = ASSR_INIT;

#if defined( SPCR_INIT )

    SPCR = SPCR_INIT;

#endif

#if CFG_USE_UART0

    // Initialize the UART

    UBRR0H = UBRR0_INIT >> 8;
    UBRR0L = UBRR0_INIT & 0xFF;

    UCSR0A = UCSR0A_INIT;
    UCSR0B = UCSR0B_INIT;
    UCSR0C = UCSR0C_INIT;

#endif

#if CFG_USE_UART1

    // Initialize the UART

    UCSR1C &= ~( 1 << URSEL );

    UBRR1H = UBRR1_INIT >> 8;
    UBRR1L = UBRR1_INIT & 0xFF;

    UCSR1A = UCSR1A_INIT;
    UCSR1B = UCSR1B_INIT;
    UCSR1C = UCSR1C_INIT;

#endif

#if CFG_USE_ADC

    ADCSR = ADCSR_INIT;

    if (( ADCSR_INIT & ADCSR ) != 0 )
    {
        // Wait for the initial conversion to complete. This initializes
        // the ADC.

        while (ADCSR & ( 1 << ADSC) )
            ;
    }

#endif

    MCUCR  = MCUCR_INIT;
    MCUCSR = MCUCSR_INIT;
    GICR   = GICR_INIT;

    InitTimer();

    TCCR1A = TCCR1A_INIT;
    TCCR1B = TCCR1B_INIT;

    OCR1A = OCR1A_INIT;
    OCR1B = OCR1B_INIT;

} /* InitHardware */

