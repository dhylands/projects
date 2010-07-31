/****************************************************************************
*
*  DebugKey.c
*
*  Debug Processing of a character
*
****************************************************************************/

#define  DEBUG 1

#include "DebugKey.h"
#include "Delay.h"
#include "Motor.h"
#include "Puts.h"
#include "Sensor.h"
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

    gRunMotors = 0;
    gDoneDebugKey = 0;
    
    gDebugCounter = 0;
    
    puts( "\nDebugKey\n" );
    
    while ( !gDoneDebugKey )
    {
        if ( UART_IsCharAvailable() )
        {
            ch = UART_GetChar();
            
            switch ( ch )
            {
                case ' ':
                {
                    puts( "Go\n" );
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
                    puts( "RampDelta = " );
                    putu8( gRampDelta );
                    puts( "\n" );
                    break;
                }
                
                case ',':
                {
                    if ( gRampDelta > 1 )
                    {
                        gRampDelta >>= 1;
                    }
                    puts( "RampDelta = " );
                    putu8( gRampDelta );
                    puts( "\n" );
                    break;
                }
                
                case 'b':
                {
                    putc( 'B' );
                    gDebugMotor = DEBUG_BOTH_MOTORS;
                    break;
                }
                
                case 'c':   // Calibrate Servos
                {
                    puts( "Calibrate Servos\n" );
                    
                    gRunMotors = 1;
                    gSpeedL = SPEED_OFF;
                    gSpeedR = SPEED_OFF;
                    break;
                }
                    
                case 'l':
                {
                    putc( 'L' );
                    gDebugMotor = DEBUG_LEFT_MOTOR;
                    break;
                }
                
                case 'r':
                {
                    putc( 'R' );
                    gDebugMotor = DEBUG_RIGHT_MOTOR;
                    break;
                }
                
                case '0':
                {
                    putc( ch );
                    
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
                    putc( ch );
                    
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
            
            puts( "\rL[ " );
            putu8( gLineL );
            puts( " " );
            putu8( gLineR );
            puts( " ]R " );
            
            puts( " EL: " );
            putu8( gEyeL );
            puts( " ER: " );
            putu8( gEyeR );

#if 0
            puts( " IR: " );
            putu8( gIrDetected );
            puts( " (" );
            putu8( gDetectCount );
            puts( ")"
#endif
            puts( " SL: " );
            putu8( gSpeedL );
            puts( " SR: " );
            putu8( gSpeedR );
            
            gDeferredNewline = 1;
        }
        
        if ( gRunMotors )
        {
            PulseMotors();
        }
      
        WaitForTimer0Rollover();
        gDebugCounter++;
    }

} // DebugKey

#else

void DebugKeyDummy( void )
{
   ;
}

#endif   // DEBUG
