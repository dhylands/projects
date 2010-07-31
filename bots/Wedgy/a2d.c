/****************************************************************************
*
*  a2d.c
*
****************************************************************************/

#include <avr/io.h>
                  
#include "a2d.h"

/***************************************************************************/
/**
*  Initializes the A2D
*/

void InitializeA2D( void )
{
    ADCSR = BV(ADEN) | BV(ADSC) | BV(ADPS1) | BV(ADPS2);
    while (ADCSR & BV(ADSC));

} // InitializeA2D

/***************************************************************************/
/**
*  Reads an A/D pin
*/

uns8 a2d( uns8 Channel )
{
#define ADLAR   5   // Give Left Adjusted result

    // Select the channel
    ADMUX = Channel | BV( ADLAR );

    // Start the conversion
    ADCSR = ADCSR | BV(ADSC);

    // Wait for it to complete
    while (ADCSR & BV(ADSC));

    // We only need the top 8 bits (left-adjusted)
    return ADCH;

} // a2d 


