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

// ---- Include Files -------------------------------------------------------

#include <avr/io.h>

#include "Motor.h"

// ---- Public Variables ----------------------------------------------------

speed_t  gRawSpeedL = SPEED_OFF;
speed_t  gRawSpeedR = SPEED_OFF;
speed_t  gPrevRawSpeedL = SPEED_OFF + 1;   // Anything different from initial gRawSpeedL
speed_t  gPrevRawSpeedR = SPEED_OFF + 1;

#define DO_RAMP         0

speed_t    gRampDelta;

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

#define SPEED_OFF_FIXED ( SPEED_OFF << RAMP_SHIFT )

//***************************************************************************
/**
*   Initialize Timer 1 for PWM.
*/

void InitMotors( void )
{
    gPrevRawSpeedL = 0;
    gPrevRawSpeedR = 0;
    gRampDelta  = 2;

#if DO_RAMP
    gActualSpeedFixedL = 15 << RAMP_SHIFT;
    gActualSpeedFixedR = 15 << RAMP_SHIFT;
#endif

    // 8 bit PWM yields divide by 255

#define CHAN_A_NON_INVERTING_PWM    ( 1 << COM1A1 ) | ( 0 << COM1A0 )
#define CHAN_B_NON_INVERTING_PWM    ( 1 << COM1B1 ) | ( 0 << COM1B0 )
#define PWM_8_BIT_MODE              ( 0 << WGM11 )  | ( 1 << WGM10 )

    TCCR1A = CHAN_A_NON_INVERTING_PWM | CHAN_B_NON_INVERTING_PWM | PWM_8_BIT_MODE;

#define T1_PRESCALAR_8  ( 0 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 )

    TCCR1B = T1_PRESCALAR_8; // Divide by 8 prescalar

    OCR1A = 0;
    OCR1B = 0;

    // In order to use PortC pins 2, 3, 4, and 5 the JTAG needs to be disabled.
    // So, we set the JTD bit in MCUCSR to disable the JTAG.

    MCUCSR |= ( 1 << JTD );

    // Set the 4 motor control pins as outputs
    // Port C pin 4, is the Right Motor Direction
    // Port C pin 3, is the Left Motor Direction
    // Port D pin 5 (OC1A) is the Right Motor PWM
    // Port D pin 4 (OC1B) is the Left Motor PWM

    PORTC |= ( ( 1 << PC3 ) | ( 1 << PC4 ));

    DDRC |= ( ( 1 << DDC3 ) | ( 1 << DDC4 ));
    DDRD |= ( ( 1 << DDD4 ) | ( 1 << DDD5 ));

    PORTC &= ( ( 1 << PC3 ) | ( 1 << PC4 ));

    SetRawMotorSpeed( SPEED_OFF, SPEED_OFF );
}

//***************************************************************************
/**
*   Sets the speed 
*/

void SetRawMotorSpeed( speed_t motorSpeedL, speed_t motorSpeedR )
{
    gRawSpeedL = motorSpeedL;
    gRawSpeedR = motorSpeedR;
    
#if DEBUG
    if (( gRawSpeedL != gPrevRawSpeedL ) || ( gRawSpeedR != gPrevRawSpeedR ))
    {
        Log( "SetRawMotorSpeed L:%d R:%d\n", gRawSpeedL, gRawSpeedR );
        
    }
#endif

    if ( gRawSpeedL != gPrevRawSpeedL )
    {
        if ( gRawSpeedL >= 0 )
        {
           PORTC |= ( 1 << PC3 );       // Clear Port-C.3
           TCCR1A |= ( 1 << COM1B0 );   // Inverted PWM
           OCR1B = gRawSpeedL;
        }
        else
        {
            PORTC &= ~( 1 << PC3 );     // Set Port-C.3
            TCCR1A &= ~( 1 << COM1B0 ); // Normal PWM
            OCR1B = -gRawSpeedL;
        }
        gPrevRawSpeedL = gRawSpeedL;
    }

    if ( gRawSpeedR != gPrevRawSpeedR )
    {
        if ( gRawSpeedR >= 0 )
        {
            PORTC |= ( 1 << PC4 );      // Clear Port-C.4
            TCCR1A |= ( 1 << COM1A0 );  // Inverted PWM
            OCR1A = gRawSpeedR;
        }
        else
        {
            PORTC &= ~( 1 << PC4 );     // Set Port-C.4
            TCCR1A &= ~( 1 << COM1A0 ); // Normal PWM
            OCR1A = -gRawSpeedR;
        }
        gPrevRawSpeedR = gRawSpeedR;
    }

} // SetRawMotorSpeed

