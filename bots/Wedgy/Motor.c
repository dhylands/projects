/****************************************************************************
*
*  Motor.c
*
*  Motor Control Code
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <avr/io.h>

#include "Motor.h"
#include "Puts.h"
#include "Sensor.h"

// ---- Public Variables ----------------------------------------------------

speed_t  gSpeedL = SPEED_OFF;
speed_t  gSpeedR = SPEED_OFF;
speed_t  gPrevSpeedL = SPEED_OFF + 1;   // Anything different from initial gSpeedL
speed_t  gPrevSpeedR = SPEED_OFF + 1;

#define DO_RAMP         0

speed_t    gRampDelta;

#if DO_RAMP

#define RAMP_DELTA  gRampDelta

speed_t  gActualSpeedFixedL;
speed_t  gActualSpeedFixedR;

#endif  // DO_RAMP

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

#define SPEED_OFF_FIXED ( SPEED_OFF << RAMP_SHIFT )

#if DO_RAMP
uns8 AdjustSpeed( uns8 desiredSpeedFixed, uns8 actualSpeedFixed )
{
    if (( desiredSpeedFixed < actualSpeedFixed ) && ( desiredSpeedFixed >= SPEED_OFF_FIXED ))
    {
        // The speed is being reduced

        return desiredSpeedFixed;
    }

    if (( desiredSpeedFixed > actualSpeedFixed ) && ( desiredSpeedFixed <= SPEED_OFF_FIXED ))
    {
        // The speed is being reduced

        return desiredSpeedFixed;
    }

    if ( desiredSpeedFixed > actualSpeedFixed )
    {
        actualSpeedFixed += RAMP_DELTA;
    }
    else
    if ( desiredSpeedFixed < actualSpeedFixed )
    {
        actualSpeedFixed -= RAMP_DELTA;
    }

    return actualSpeedFixed;
}
#endif

extern void InitMotors( void )
{
    gPrevSpeedL = 0;
    gPrevSpeedR = 0;
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

    PulseMotors();
}

/****************************************************************************
*
*  Sets the motor speed of both motors.
*/

extern void PulseMotors( void )
{
    speed_t    pulseL;
    speed_t    pulseR;

#if DO_RAMP

    speed_t    gActualSpeedL;
    speed_t    gActualSpeedR;

    speed_t    gDesiredSpeedFixedL = gSpeedL << RAMP_SHIFT;
    speed_t    gDesiredSpeedFixedR = gSpeedR << RAMP_SHIFT;

    gActualSpeedFixedL = AdjustSpeed( gDesiredSpeedFixedL,  gActualSpeedFixedL );
    gActualSpeedFixedR = AdjustSpeed( gDesiredSpeedFixedR,  gActualSpeedFixedR );

    gActualSpeedL = ( gActualSpeedFixedL >> RAMP_SHIFT );
    gActualSpeedR = ( gActualSpeedFixedR >> RAMP_SHIFT );

#else

#define gActualSpeedL   gSpeedL
#define gActualSpeedR   gSpeedR

#endif

    // We need to run the right motor in reverse, so we
    // do the adjustment here.
    //
    // The way I wired up the motors, +ve goes backwards and -ve goes
    // forwards, so we just negate things here.
    
    pulseL = -gActualSpeedL;
    pulseR = -gActualSpeedR;
    
    //puts( "PulseMotors called\n" );
    
#if DEBUG
    if (( gSpeedL != gPrevSpeedL ) || ( gSpeedR != gPrevSpeedR ))
    {
        puts( "PulseMotors L:" );
        putu8( gSpeedL );
        puts( " R:" );
        putu8( gSpeedR );
        puts( " C: " );
        putu16( gHuntCounter );
        puts( "\n" );
        
        gPrevSpeedL = gSpeedL;
        gPrevSpeedR = gSpeedR;
    }
#endif

    {
        static  speed_t prevPulseL = SPEED_OFF + 1;
        static  speed_t prevPulseR = SPEED_OFF + 1;

        if ( pulseL != prevPulseL )
        {
            if ( pulseL >= 0 )
            {
               PORTC &= ~( 1 << PC3 );       // Clear Port-C.3
               TCCR1A &= ~( 1 << COM1B0 ); // Normal PWM
               OCR1B = pulseL;
            }
            else
            {
                PORTC |= ( 1 << PC3 );       // Set Port-C.3
                TCCR1A |= ( 1 << COM1B0 ); // Inverted PWM
                OCR1B = -pulseL;
            }
            prevPulseL = pulseL;
        }

        if ( pulseR != prevPulseR )
        {
            if ( pulseR >= 0 )
            {
                PORTC &= ~( 1 << PC4 );       // Clear Port-C.4
                TCCR1A &= ~( 1 << COM1A0 ); // Normal PWM
                OCR1A = pulseR;
            }
            else
            {
                PORTC |= ( 1 << PC4 );       // Set Port-C.4
                TCCR1A |= ( 1 << COM1A0 ); // Inverted PWM
                OCR1A = -pulseR;
            }
            prevPulseR = pulseR;
        }
    }

} // PulseMotors

