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

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

// Port B

enum
{
	MotorA_PWM  = 1,	// PWM Wire   - B1
	MotorB_PWM  = 2,	// PWM Wire   - B2
	MotorA_Dir = 7,
	MotorB_Dir = 6
};

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

extern void InitMotors (uint8_t pwmType)
{
	// 8 bit PWM yields divide by 255

#define CHAN_A_NON_INVERTING_PWM	( 1 << COM1A1 ) | ( 0 << COM1A0 )
#define CHAN_B_NON_INVERTING_PWM	( 1 << COM1B1 ) | ( 0 << COM1B0 )
#define PWM_8_BIT_MODE			  ( 0 << WGM11 )  | ( 1 << WGM10 )

	TCCR1A = CHAN_A_NON_INVERTING_PWM | CHAN_B_NON_INVERTING_PWM | PWM_8_BIT_MODE;

#define T1_PRESCALAR_1  ( 0 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 )
#define T1_PRESCALAR_8  ( 0 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 )
#define T1_PRESCALAR_64  ( 0 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 )


	TCCR1B = T1_PRESCALAR_8; // Divide by 1 prescalar

	OCR1A = 0;
	OCR1B = 0;

	// Set the 4 motor control pins as outputs

	{
		uint8_t pins = (1 << MotorA_PWM) | (1 << MotorA_Dir) | (1 << MotorB_PWM) | (1 << MotorB_Dir);

		PORTB |= pins;
		DDRB  |= pins;
	}

	if (pwmType == SPEED_SIGN_MAGNITUDE)
		SetMotorSpeedSignMagnitude (SPEED_OFF_SM, SPEED_OFF_SM);
	else
		SetMotorSpeedLockedAntiPhase (SPEED_OFF_LAP, SPEED_OFF_LAP);
}

/****************************************************************************
*
*  Sets the motor speed of both motors.
*/

extern void SetMotorSpeedLockedAntiPhase (speed_t speedLeft, speed_t speedRight)
{
	static  speed_t prevSpeedLeft = SPEED_OFF_LAP + 1;
	static  speed_t prevSpeedRight = SPEED_OFF_LAP + 1;

	if ( speedLeft != prevSpeedLeft )
	{
		OCR1A = speedLeft;
		prevSpeedLeft = speedLeft;
	}

	if ( speedRight != prevSpeedRight )
	{
		OCR1B = speedRight;
		prevSpeedRight = speedRight;
	}

} // SetMotorSpeedLockedAntiPhase

/****************************************************************************
*
*  Sets the motor speed of both motors.
*/

extern void SetMotorSpeedSignMagnitude (speed_t speedL, speed_t speedR)
{
    static  speed_t prevSpeedL = SPEED_OFF_SM + 1;
    static  speed_t prevSpeedR = SPEED_OFF_SM + 1;

    // We need to run the left motor in reverse, so we
    // do the adjustment here.
    
    speedL = -speedL;
    
    if ( speedL != prevSpeedL )
    {
        if ( speedL >= 0 )
        {
           PORTB &= ~( 1 << MotorA_Dir );
           TCCR1A &= ~( 1 << COM1A0 ); // Normal PWM
           OCR1A = speedL;
        }
        else
        {
            PORTB |= ( 1 << MotorA_Dir );
            TCCR1A |= ( 1 << COM1A0 ); // Inverted PWM
            OCR1A = -speedL;
        }
        prevSpeedL = speedL;
    }

    if ( speedR != prevSpeedR )
    {
        if ( speedR >= 0 )
        {
            PORTB &= ~( 1 << MotorB_Dir );
            TCCR1A &= ~( 1 << COM1B0 ); // Normal PWM
            OCR1B = speedR;
        }
        else
        {
            PORTB |= ( 1 << MotorB_Dir );
            TCCR1A |= ( 1 << COM1B0 ); // Inverted PWM
            OCR1B = -speedR;
        }
        prevSpeedR = speedR;
    }

} // SetMotorSpeedSignMagnitude
