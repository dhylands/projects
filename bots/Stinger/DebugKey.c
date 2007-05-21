/****************************************************************************
*
*  DebugKey.c
*
*  Debug Processing of a character
*
****************************************************************************/

#define  DEBUG 1

#include "Stinger.h"
#include "Debug.h"
#include "DebugKey.h"
#include "Delay.h"
#include "Log.h"
#include "Motor.h"
#include "Sensor.h"
#include "Timer.h"
#include "Uart.h"

#if DEBUG

uns8  gRunMotors;
uns8  gDoneDebugKey;

#define  DEBUG_LEFT_MOTOR  1
#define  DEBUG_RIGHT_MOTOR 2
#define  DEBUG_BOTH_MOTORS 3

uns8  gDebugMotor;
uns8  gDebugCounter;

speed_t  gSpeedMap[10] = { 0, -255, -192, -128, -64, 0, 64, 128, 192, 255 };

void DebugKey( void )
{
    uns8    ch;

    speed_t saveSpeedR = gSpeedR;
    speed_t saveSpeedL = gSpeedL;

    gRunMotors = 0;
    gDoneDebugKey = 0;
    
    gDebugCounter = 0;

    DBG( "\nDebugKey\n" );

    while ( !gDoneDebugKey )
    {
        if ( UART_IsCharAvailable() )
        {
            ch = UART_GetChar();
            
            switch ( ch )
            {
                case ' ':
                {
                    DBG( "Go\n" );
                    gDoneDebugKey = 1;
                    break;
                }
                
                case '.':
                {
                    uns8    maxRamp = 1 << RAMP_SHIFT;
                    
                    if ( gRampDelta < maxRamp )
                    {
                        gRampDelta <<= 1;
                    }
                    DBG( "RampDelta = %3u\n", gRampDelta );
                    break;
                }
                
                case ',':
                {
                    if ( gRampDelta > 1 )
                    {
                        gRampDelta >>= 1;
                    }
                    DBG( "RampDelta = %3u\n", gRampDelta );
                    break;
                }
                
                case 'b':
                {
                    DBG( "B" );
                    gDebugMotor = DEBUG_BOTH_MOTORS;
                    break;
                }
                
                case 'c':   // Calibrate Servos
                {
                    DBG( "Calibrate Servos\n" );
                    
                    gRunMotors = 1;
                    gSpeedL = SPEED_OFF;
                    gSpeedR = SPEED_OFF;
                    break;
                }
                    
                case 'l':
                {
                    DBG( "L" );
                    gDebugMotor = DEBUG_LEFT_MOTOR;
                    break;
                }
                
                case 'r':
                {
                    DBG( "R" );
                    gDebugMotor = DEBUG_RIGHT_MOTOR;
                    break;
                }
                
                case '0':
                {
                    DBG( "%c", ch );
                    
                    gRunMotors = 0;
                    gSpeedL = SPEED_OFF;
                    gSpeedR = SPEED_OFF;
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
                    DBG( "%c", ch );
                    
                    gRunMotors = 1;
                    if ( gDebugMotor & DEBUG_LEFT_MOTOR )
                    {
                      gSpeedL = gSpeedMap[( ch - '0' )];
                    }
                    if ( gDebugMotor & DEBUG_RIGHT_MOTOR )
                    {
                        gSpeedR = gSpeedMap[( ch - '0' )];
                    }
                    break;
                }
            }
        }

        ReadSensors();
        
        if (( gDebugCounter & 0x0F ) == 0 )
        {
            gDeferredNewline = 0;

            DBG( "\rL[ %3u %3u %3u %3u ]R EL:%3u ER:%3u B:%d P:%u SL:%3u SR:%3u\n",
                 gLineLO, gLineLI, gLineRI, gLineRO, gEyeL, gEyeR, 
                IsBlackButtonPressed(), PgmSelect(), gSpeedL, gSpeedR );
            
            gDeferredNewline = 1;
        }
        
        if ( !gRunMotors )
        {
            gSpeedL = SPEED_OFF;
            gSpeedR = SPEED_OFF;
        }
        PulseMotors();
      
        WaitForTimer0Rollover();
        gDebugCounter++;
    }

    gSpeedL = saveSpeedL;
    gSpeedR = saveSpeedR;

} // DebugKey

#else

void DebugKeyDummy( void )
{
   ;
}

#endif   // DEBUG
