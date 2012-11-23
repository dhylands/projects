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

/* ---- Include Files ----------------------------------------------------- */

#include "DebugKey.h"
#include "Log.h"
#include "Motor.h"
#include "UART.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

typedef enum
{
    SetLeftMotor    = 0x01,
    SetRightMotor   = 0x02,
    SetBothMotors   = 0x03
} MotorMode;

/* ---- Private Variables ------------------------------------------------- */

static  MotorMode   gMotorMode;

static  speed_t     gSpeedMap[10] = { 0, -255, -192, -128, -64, 0, 64, 128, 192, 255 };


/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Debugging menu
*/

void DebugKey( void )
{
    int     ch;
    speed_t leftSpeed = SPEED_OFF;
    speed_t rightSpeed = SPEED_OFF;

    Log( "DebugKey\n" );

    SetRawMotorSpeed( leftSpeed, rightSpeed );

    while ( 1 )
    {
        if ( UART0_IsCharAvailable() )
        {
            ch = UART0_GetChar();

            Log( "%c", ch );

            switch ( ch )
            {
                case ' ':
                {
                    Log( "Go\n" );
                    return;
                }

                case 'b':
                {
                    gMotorMode = SetBothMotors;
                    leftSpeed = SPEED_OFF;
                    rightSpeed = SPEED_OFF;
                    break;
                }

                case 'l':
                {
                    gMotorMode = SetLeftMotor;
                    leftSpeed = SPEED_OFF;
                    rightSpeed = SPEED_OFF;
                    break;
                }

                case 'r':
                {
                    gMotorMode = SetRightMotor;
                    leftSpeed = SPEED_OFF;
                    rightSpeed = SPEED_OFF;
                    break;
                }

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    if (( gMotorMode & SetLeftMotor ) != 0 )
                    {
                        leftSpeed = gSpeedMap[ ch - '0' ];
                    }
                    if (( gMotorMode & SetRightMotor ) != 0 )
                    {
                        rightSpeed = gSpeedMap[ ch - '0' ];
                    }
                    SetRawMotorSpeed( leftSpeed, rightSpeed );
                    break;
                }
            }
        }
    }

} // DebugKey

