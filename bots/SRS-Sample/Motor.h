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

#if !defined( MOTOR_H )
#define  MOTOR_H

#include <inttypes.h>

// Motor Speeds are defined as a percentage

#define SPEED_BWD       -255
#define SPEED_BWD_MED   -150
#define SPEED_OFF          0
#define SPEED_FWD_MED    150
#define SPEED_FWD        255

#define SPEED_BWD_SPIN  -150
#define SPEED_FWD_SPIN   150

typedef int16_t speed_t;

extern  speed_t gRawSpeedL;
extern  speed_t gRawSpeedR;
extern  speed_t gPrevRawSpeedL;
extern  speed_t gPrevRawSpeedR;
extern  speed_t gRampDelta;

#define RAMP_SHIFT  3


void InitMotors( void );

void SetRawMotorSpeed( speed_t motorSpeedL, speed_t motorSpeedR );

#endif   // MOTOR_H

