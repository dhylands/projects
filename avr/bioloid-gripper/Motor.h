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

#define  SPEED_OFF         127

typedef  uint16_t  speed_t;

void InitMotors( void );
void SetMotorSpeed( speed_t speedLeft, speed_t speedRight );
void disableMotor( void );
void enableMotor( void );

#endif   // MOTOR_H











