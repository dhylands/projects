/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   adc.c
*
*   @brief  Interupt driven/Polled interface for A/D converters
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdint.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "Config.h"

/* ---- Public Variables -------------------------------------------------- */

#if !defined( CFG_ADC_AVERAGE )
#   define  CFG_ADC_AVERAGE 1
#endif

#if (( CFG_ADC_POLL != 0 ) && ( CFG_ADC_AVERAGE != 1 ))
#error  CFG_ADC_AVERAGE must be set to 1 when CFG_ADC_POLL is != 0
#endif

#if (( 8 * CFG_ADC_AVERAGE ) < 256 )
typedef uint8_t     ADC_Index_t;
#else
typedef uint16_t    ADC_Index_t;
#endif

#if ( CFG_ADC_POLL == 0 )
static           ADC_Index_t    gAdcChannel = 0;
static  volatile ADC_Sample_t   gAdcHistory[ 8 * CFG_ADC_AVERAGE ] = { 0 };
#endif

/* ---- Private Constants and Types --------------------------------------- */

#define ADMUX_REF_AREF      (( 0 << REFS1 ) | ( 0 << REFS0 ))
#define ADMUX_REF_AVCC      (( 0 << REFS1 ) | ( 1 << REFS0 ))
#define ADMUX_REF_INTERNAL  (( 1 << REFS1 ) | ( 1 << REFS0 ))

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

#if ( CFG_ADC_POLL == 0 )
//***************************************************************************
/**
*   ISR called when a conversion completes
*/

ISR( ADC_vect )
{
    // We only need the top 8 bits (left adjusted)

#if CFG_ADC_8_BIT
    gAdcHistory[ gAdcChannel ] = ADCH;
#else
    gAdcHistory[ gAdcChannel ] = ADC;
#endif

    // Advance to the next channel
    if ( ++gAdcChannel >= ( sizeof( gAdcHistory ) / sizeof( gAdcHistory[ 0 ])))
    {
        gAdcChannel = 0;
    }

    // Setup the mux for the next channel

    ADMUX = ( ADMUX & (( 1 << REFS1 ) | ( 1 << REFS0 ))) | ( gAdcChannel & 7 )
#if CFG_ADC_8_BIT
          | ( 1 << ADLAR )
#endif
          ;

    // Start the next conversion

    ADCSRA |= ( 1 << ADSC );

} // ADC_vect

#endif  // CFG_ADC_POLL == 0

//***************************************************************************
/**
*   Retrieves a value from the ADC
*/

ADC_Sample_t ADC_Read( uint8_t channel )
{
#if ( CFG_ADC_POLL == 0 )

    // We're using initerrupt driven ADC sampling

    uint16_t    avg = 0;
    ADC_Index_t i;

    i = ( CFG_ADC_AVERAGE * 8 ) + channel;

    while ( i > channel )
    {
        i -= 8;

        avg += gAdcHistory[ i ];
    }

    avg /= CFG_ADC_AVERAGE;

    return (ADC_Sample_t)avg;

#else

    // Select the channel in a manner which leaves REFS0 and REFS1 un touched.

    ADMUX = ( ADMUX & (( 1 << REFS1 ) | ( 1 << REFS0 )))
#if CFG_ADC_8_BIT
          | ( 1 << ADLAR )
#endif
          | channel;

    // Start the conversion
    ADCSRA = ADCSRA | ( 1 << ADSC );

    // Wait for it to complete
    while ( ADCSRA & ( 1 << ADSC ));

#if CFG_ADC_8_BIT
    // We only need the top 8 bits (left-adjusted)
    return ADCH;
#else
    // Return the 10 bit value.
    return ADC;
#endif

#endif
}

//***************************************************************************
/**
*   Initialize the ADC
*/

void ADC_Init( uint8_t prescalar )
{
    // Start off on channel 0

    ADMUX = ADMUX_REF_AVCC
#if defined( ADATE ) && ( CFG_ADC_POLL == 0 )
          | ( 1 << ADATE )
#endif
#if CFG_ADC_8_BIT
          | ( 1 << ADLAR )
#endif
          ;

    // Ensure that the prescalar is in range

    prescalar &= (( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ));

    // Kick things off

    ADCSRA = ( 1 << ADEN ) | ( 1 << ADSC )
#if ( CFG_ADC_POLL == 0 )
           | ( 1 << ADIE )
#endif
           | prescalar;

} // ADC_Init

