/****************************************************************************
*
*  Motor.h
*
*  Motor Control Code
*
****************************************************************************/

#if !defined( MOTOR_H )
#define  MOTOR_H

#include <inttypes.h>

// Motor Speeds are defined as a percentage

#define  SPEED_OFF_LAP		127
#define SPEED_OFF_SM		0

#define SPEED_SIGN_MAGNITUDE 0
#define SPEED_LOCKED_ANTIPHASE 1

typedef  int16_t  speed_t;

void InitMotors (uint8_t pwmType);
void SetMotorSpeedSignMagnitude (speed_t speedLeft, speed_t speedRight);
void SetMotorSpeedLockedAntiPhase (speed_t speedLeft, speed_t speedRight);

#endif   // MOTOR_H











