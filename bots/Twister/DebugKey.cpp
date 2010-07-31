/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   DebugKey.cpp
*
*   @brief  Implements simple debugging features
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <68332/qsm.h>

#include "DebugKey.h"
#include "Motor.h"
#include "Twister.h"

#include <assert.h>
#include <ctype.h>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------

extern void StraightLine();

static int TranslateSpeed( char ch );

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Debug
 * @{
 */

#include "68332/sim.h"

void WatchDogReset( void )
{
    set_watchdog( 1, 0, 0 );
    reset_watchdog();

    while ( 1 )
    {
        for ( int i = 0; i < 1000000; i++ ) 
        {
            iprintf( "+" );
        }
    }

}

//***************************************************************************
/**
*  Processes a keystroke for debugging
*
*  @param   ch   (in)  Character to process
*/

void DebugKey()
{
    char    ch;
    int     done = 0;
    int     leftMotor = 0;
    int     rightMotor = 0;

    iprintf( "\nDebugKey\n" );

    while ( !done )
    {
        assert(( (unsigned long)gRightMotor.m_name & 0xFFFF0000 ) == 0 );

        if ( havebyte() )
        {
            ch = inbyte();
            ch = tolower( ch );

            switch ( ch )
            {
                case ' ':
                {
                    iprintf( "Go\n" );
                    done = 1;
                    break;
                }

                case 'b':
                {
                    putchar( 'B' );
                    leftMotor = 1;
                    rightMotor = 1;
                    break;
                }

                case 'l':
                {
                    putchar( 'L' );
                    leftMotor = 1;
                    rightMotor = 0;
                    break;
                }

                case 'r':
                {
                    putchar( 'R' );
                    leftMotor = 0;
                    rightMotor = 1;
                    break;
                }

                case 's':
                {
                    StraightLine();
                    break;
                }

                case 'w':
                {
                    WatchDogReset();
                    break;
                }

                case '0':
                {
                    putchar( '0' );

                    if ( leftMotor )
                    {
                        gLeftMotor.Coast();
                    }
                    if ( rightMotor )
                    {
                        gRightMotor.Coast();
                    }
                    break;
                }

                case '-':
                {
                    putchar( '-' );

                    if ( leftMotor )
                    {
                        gLeftMotor.Brake();
                    }
                    if ( rightMotor )
                    {
                        gRightMotor.Brake();
                    }
                    break;
                }

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
                    putchar( ch );

                    int speed = TranslateSpeed( ch );

                    if ( leftMotor )
                    {
                        gLeftMotor.SetSpeed( speed );    
                    }
                    if ( rightMotor )
                    {
                        gRightMotor.SetSpeed( speed );
                    }

                    break;
                }

                case ',':   // Decrease Frequency
                {
                    putchar( ',' );
                    unsigned freq = gLeftMotor.GetFrequency();
                    if ( freq >= 512 )
                    {
                        freq /= 2;

                        gLeftMotor.SetFrequency( freq );
                        gRightMotor.SetFrequency( freq );
                    }
                    break;
                }

                case '.':   // Increase Frequency
                {
                    putchar( '.' );
                    unsigned freq = gLeftMotor.GetFrequency();

                    if ( freq <= 16384 )
                    {
                        freq *= 2;

                        gLeftMotor.SetFrequency( freq );
                        gRightMotor.SetFrequency( freq );
                    }
                    break;
                }

                case '!':
                {
                    iprintf( "\nResetting...\n" );

                    asm( " trap #15" );
                    asm( " dc.w 0x0063" );

                    break;
                }
            }
            fflush( stdout );
        }
    }

} // DebugKey

int TranslateSpeed( char ch )
{
    int speed = 0;
    int speedRange = Motor::MaxSpeed - Motor::StartSpeed;

    switch ( ch )
    {
        case '1':   speed = -Motor::MaxSpeed;                               break;
        case '2':   speed = -( Motor::StartSpeed + ( speedRange * 2 / 3 )); break;
        case '3':   speed = -( Motor::StartSpeed + ( speedRange * 1 / 3 )); break;
        case '4':   speed = -Motor::StartSpeed;                             break;
        case '5':   speed = 0;                                              break;
        case '6':   speed = Motor::StartSpeed;                              break;
        case '7':   speed = Motor::StartSpeed + ( speedRange * 1 / 3 );     break;
        case '8':   speed = Motor::StartSpeed + ( speedRange * 2 / 3 );     break;
        case '9':   speed = Motor::MaxSpeed;                                break;
    }

    return speed;

}   // TranslateSpeed

/** @} */

