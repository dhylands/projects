/****************************************************************************
*
*  Hardware.c
*
*   This header file contains all of the hardware definitions for the chip.
*   By centralizing everything in one header file, it allows us to keep
*   track of what's being used by whom.
*
****************************************************************************/

#include "Hardware.h"

void InitHardware( void )
{
    MCUCR = MCUCR_INIT;
    TCCR2 = TCCR2_INIT;
    GICR  = GICR_INIT;

    // Initialize the Ports

    DDRB  = DDRB_INIT;
    PORTB = PORTB_INIT;

    DDRC  = DDRC_INIT;
    PORTC = PORTC_INIT;

    DDRD  = DDRD_INIT;
    PORTD = PORTD_INIT;

    // Initialize the UART

    UBRRH = UBRR_INIT >> 8;
    UBRRL = UBRR_INIT & 0xFF;

    UCSRA = UCSRA_INIT;
    UCSRB = UCSRB_INIT;
    UCSRC = UCSRC_INIT;


} // InitHardware
