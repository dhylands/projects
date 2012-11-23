/****************************************************************************
*
*  Motor.h
*
*  Motor Control Code
*
****************************************************************************/

#if !defined( MOTOR_H )
#define  MOTOR_H

#if !defined( TYPES_H )
#   include "Types.h"
#endif

// Motor Speeds are defined as a percentage

#define  SPEED_BWD_FAST -255
#define  SPEED_BWD      -175
#define  SPEED_BWD_MED   -75
#define  SPEED_OFF         0
#define  SPEED_FWD_MED    75
#define  SPEED_FWD       175
#define  SPEED_FWD_FAST  255

#define  SPEED_BWD_SPIN -100
#define  SPEED_FWD_SPIN  100

typedef  int16 speed_t;

extern   speed_t  gSpeedL;
extern   speed_t  gSpeedR;
extern   speed_t  gPrevSpeedL;
extern   speed_t  gPrevSpeedR;
extern   speed_t  gRampDelta;

extern  const char *gReason;

#define RAMP_SHIFT  3


void InitMotors( void );
void PulseMotors( void );

#endif   // MOTOR_H

