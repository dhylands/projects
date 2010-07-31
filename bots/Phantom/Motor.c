/****************************************************************************
*
*  Motor.c
*
*  Motor Control Code
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <avr/io.h>

#include "Phantom.h"
#include "Motor.h"
#include "Puts.h"
#include "Sensor.h"

// ---- Public Variables ----------------------------------------------------

speed_t  gSpeedL = SPEED_OFF;
speed_t  gSpeedR = SPEED_OFF;
speed_t  gPrevSpeedL = SPEED_OFF + 1;   // Anything different from initial gSpeedL
speed_t  gPrevSpeedR = SPEED_OFF + 1;

const char *gReason = "";

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

    // Set the 4 motor control pins as outputs

    {
        uns8    pins = ( 1 << MotorA_Dir ) | ( 1 << MotorA_PWM )
                     | ( 1 << MotorB_Dir ) | ( 1 << MotorB_PWM );
    
        PORTB |= pins;
        DDRB  |= pins;
    }

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

    // We need to run the left motor in reverse, so we
    // do the adjustment here.
    
    pulseL = -gActualSpeedL;
    pulseR =  gActualSpeedR;
    
    //puts( "PulseMotors called\n" );
    
#if DEBUG || 1
    if (( gSpeedL != gPrevSpeedL ) || ( gSpeedR != gPrevSpeedR ))
    {
        puts( "PulseMotors L:" );
        puti16( gSpeedL );
        puts( " R:" );
        puti16( gSpeedR );
        puts( " C: " );
        putu16( gHuntCounter );

        puts( " ");
        puts( gReason );

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
               PORTB &= ~( 1 << MotorA_Dir );
               TCCR1A &= ~( 1 << COM1A0 ); // Normal PWM
               OCR1A = pulseL;
            }
            else
            {
                PORTB |= ( 1 << MotorA_Dir );
                TCCR1A |= ( 1 << COM1A0 ); // Inverted PWM
                OCR1A = -pulseL;
            }
            prevPulseL = pulseL;
        }

        if ( pulseR != prevPulseR )
        {
            if ( pulseR >= 0 )
            {
                PORTB &= ~( 1 << MotorB_Dir );
                TCCR1A &= ~( 1 << COM1B0 ); // Normal PWM
                OCR1B = pulseR;
            }
            else
            {
                PORTB |= ( 1 << MotorB_Dir );
                TCCR1A |= ( 1 << COM1B0 ); // Inverted PWM
                OCR1B = -pulseR;
            }
            prevPulseR = pulseR;
        }
    }

} // PulseMotors

