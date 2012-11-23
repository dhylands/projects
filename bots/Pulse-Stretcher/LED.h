/****************************************************************************
*
*  LED.h
*
*
****************************************************************************/

#if !defined( LED_H )
#define LED_H

#if !defined( HARDWARE_H )
#   include "Hardware.h"
#endif

static inline void InitLED()
{
    DDRB |= ( BV( BlueLEDPin ) | BV( GreenLEDPin ));
}

static inline void BlueLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= BV( BlueLEDPin );
    }
    else
    {
        PORTB &= ~BV( BlueLEDPin );
    }
}

static inline void GreenLED( int onOff )
{
    if ( onOff )
    {
        PORTB |= BV( GreenLEDPin );
    }
    else
    {
        PORTB &= ~BV( GreenLEDPin );
    }
}

#endif  // LED_H
