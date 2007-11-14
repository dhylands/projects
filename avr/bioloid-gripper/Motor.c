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
	MotorA_Enable  = 7,	// Enable Wire	- B7

	MotorB_PWM  = 2,	// PWM Wire   - B2
	MotorB_Enable  = 6,	// Enable Wire	- B6
};

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

extern void InitMotors( void )
{
	// 8 bit PWM yields divide by 255

#define CHAN_A_NON_INVERTING_PWM	( 1 << COM1A1 ) | ( 0 << COM1A0 )
#define CHAN_B_NON_INVERTING_PWM	( 1 << COM1B1 ) | ( 0 << COM1B0 )
#define PWM_8_BIT_MODE			  ( 0 << WGM11 )  | ( 1 << WGM10 )

	TCCR1A = CHAN_A_NON_INVERTING_PWM | CHAN_B_NON_INVERTING_PWM | PWM_8_BIT_MODE;

#define T1_PRESCALAR_1  ( 0 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 )
#define T1_PRESCALAR_8  ( 0 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 )

	TCCR1B = T1_PRESCALAR_1; // Divide by 1 prescalar

	OCR1A = 0;
	OCR1B = 0;

	// Set the 4 motor control pins as outputs

	{
		uint8_t pins = ( 1 << MotorA_Enable ) | ( 1 << MotorA_PWM )
					 | ( 1 << MotorB_Enable ) | ( 1 << MotorB_PWM );
	
		PORTB |= pins;
		DDRB  |= pins;
	}

	SetMotorSpeed( SPEED_OFF, SPEED_OFF );
}

/****************************************************************************
*
*  Sets the motor speed of both motors.
*/

extern void SetMotorSpeed( speed_t speedWrist, speed_t speedFinger )
{
	static  speed_t prevSpeedWrist = SPEED_OFF + 1;
	static  speed_t prevSpeedFinger = SPEED_OFF + 1;

	if ( speedWrist != prevSpeedWrist )
	{
		if (speedWrist == SPEED_OFF)
			PORTB |= ( 1 << MotorA_Enable );
		else
			PORTB &= ~( 1 << MotorA_Enable );
		OCR1A = speedWrist;
		prevSpeedWrist = speedWrist;
	}

	if ( speedFinger != prevSpeedFinger )
	{
		if (speedFinger == SPEED_OFF)
			PORTB |= ( 1 << MotorB_Enable );
		else
			PORTB &= ~( 1 << MotorB_Enable );
		OCR1B = speedFinger;
		prevSpeedFinger = speedFinger;
	}

} // SetMotorSpeed

extern void disableMotor( void )
{
	PORTB |= ( 1 << MotorA_Enable );
	PORTB |= ( 1 << MotorB_Enable );
} // Disable motor

extern void enableMotor( void )
{
	PORTB &= ~( 1 << MotorA_Enable );
	PORTB &= ~( 1 << MotorB_Enable );
} // Enable motor



