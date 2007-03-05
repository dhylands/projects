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
*   @file    Hardware.c 
*
*   @brief   Performs hardware initialization
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "Hardware.h"
#include "Delay.h"
#include "Timer.h"

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

    PORTE   = PORTE_INIT;
    DDRE    = DDRE_INIT;

    PORTF   = PORTF_INIT;
    DDRF    = DDRF_INIT;

    PORTG   = PORTG_INIT;
    DDRG    = DDRG_INIT;

    ASSR    = ASSR_INIT;

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

    UBRR1H = UBRR1_INIT >> 8;
    UBRR1L = UBRR1_INIT & 0xFF;

    UCSR1A = UCSR1A_INIT;
    UCSR1B = UCSR1B_INIT;
    UCSR1C = UCSR1C_INIT;

#endif

#if CFG_USE_ADC

    ADCSR = ADCSR_INIT;
    ADMUX = ADMUX_INIT;

    if (( ADCSR_INIT & ADCSR ) != 0 )
    {
        // Wait for the initial conversion to complete. This initializes
        // the ADC.

        while (ADCSR & ( 1 << ADSC) )
            ;
    }

#endif

    InitTimer();

} /* InitHardware */

