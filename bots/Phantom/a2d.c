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
    ADCSR = ( 1 << ADEN ) | ( 1 << ADSC ) | ( 1 << ADPS1 ) | ( 1 << ADPS2 );
    while ( ADCSR & ( 1 << ADSC ));

} // InitializeA2D

/***************************************************************************/
/**
*  Reads an A/D pin
*/

uns8 a2d( uns8 Channel )
{
#define ADLAR   5   // Give Left Adjusted result

    // Select the channel
    ADMUX = Channel | ( 1 << ADLAR );

    // Start the conversion
    ADCSR = ADCSR | ( 1 << ADSC );

    // Wait for it to complete
    while (ADCSR & ( 1 << ADSC ));

    // We only need the top 8 bits (left-adjusted)
    return ADCH;

} // a2d 


