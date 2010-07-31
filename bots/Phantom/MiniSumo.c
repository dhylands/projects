/****************************************************************************
*
*  MiniSumo.c
*
*  Mini Sumo code for Wedgy
*
****************************************************************************/

#include <avr/io.h>

#include "Phantom.h"
#include "a2d.h"
#include "Avoid.h"
#include "Debug.h"
#include "DebugKey.h"
#include "Delay.h"
#include "Motor.h"
#include "Puts.h"
#include "Sensor.h"
#include "Uart.h"

#define  EYE_MIN_SIGNAL    35 // Normal use
#define  EYE_MIN_SIGNAL2   65 // Used for shortly after we hit the line

#define  EYE_MIN_SIGNAL_SIDE    45

#define  EYE_MIN_DIFF      10

#define  COUNTER_TURN_COUNT      208
#define  COUNTER_SPIN_COUNT      32    // 0 - 50 = arc, > 50 = SPIN

// It takes about 6/10 of second to spin, so we'll ignore about 1/3 of a circle.

#define  COUNTER_EYE_RESET_COUNT ( COUNTER_SPIN_COUNT + 25 )

#define  COUNTER_STRAIGHT_COUNT  400   // Start going straight

uns8  gEyeMinSignal;
uns8  gEyeState;
uns8  gPrevEyeState;

uns8  gHuntDir;
uns16 gHuntCounter;

/***************************************************************************/
/**
*  Main program for the Mini Sumo
*/

int main( void )
{
    uns8  lineSide;
    int16 eyeDiff;
    uns8  i;
    uns8  sec;

    DBG( uns8  huntState; )
    DBG( uns8  prevHuntState; )

    // Initialization

    UART_Init();
    InitializeA2D();
    InitSensors();
    InitLED();

#if DEBUG
    UART_PutStr( "******************************\r\n" );
    UART_PutStr( "**                          **\r\n" );
    UART_PutStr( "** Phantom MiniSumo Program **\r\n" );
    UART_PutStr( "**                          **\r\n" );
    UART_PutStr( "******************************\r\n\r\n" );
#endif

    InitTimer();
    InitMotors();

    //puts( "Testing\n" );

    gDeferredNewline = 0;

    gHuntCounter = 0;

    gEyeState = 0;
    gPrevEyeState = 0;
    gEyeMinSignal = EYE_MIN_SIGNAL;

    DBG( huntState = 0; )
    DBG( prevHuntState = 0; )

#if 1
    if ( IsDirectionSwitchLeft() )
    {
        puts( "Initial Hunt Straight-Left\n" );
        gHuntDir = HUNT_LEFT;
        gHuntCounter = COUNTER_TURN_COUNT;
    }
    else
    {
        puts( "Initial Hunt Straight-Right\n" );
        gHuntDir = HUNT_RIGHT;
        gHuntCounter = COUNTER_TURN_COUNT;
    }
#else
    if ( IsDirectionSwitchLeft() )
    {
        puts( "Initial Hunt Straight-Right\n" );
        gHuntDir = HUNT_RIGHT;
        gHuntCounter = COUNTER_TURN_COUNT;
    }
    else
    {
        puts( "Initial Hunt Right\n" );
        gHuntDir = HUNT_RIGHT;
    }
#endif

    if ( IsRunStopStop() )
    {
        // We're in "stop" mode. This means that we need to wait for the
        // black button to be pressed before doing the 5 second countdown.
        // Toggle the LED's quickly while we're waiting.

        puts( "Stop Mode - Waiting for Black Button press\n" );

        while ( 1 )
        {
            if ( IsBlackButtonPressed() )
            {
                puts( "Black Button pressed\n" );
                break;
            }

            BlueLED( 1 );
            GreenLED( 0 );

            for ( i = 0; i < 12; i++ ) 
            {
                if ( IsBlackButtonPressed() )
                {
                    break;
                }
               
                WaitForTimer0Rollover();
            }

            BlueLED( 0 );
            GreenLED( 1 );
        
            for ( i = 0; i < 12; i++ ) 
            {
                if ( IsBlackButtonPressed() )
                {
                    break;
                }

                WaitForTimer0Rollover();
            }
        }

        // OK, the black button has been pressed. Do the 5 second countdown.

        puts( "Starting 5 second countdown\n" );

        for ( sec = 0; sec < 5; sec++ ) 
        {
            BlueLED( 1 );
            GreenLED( 0 );

            for ( i = 0; i < 50; i++ ) 
            {
                WaitForTimer0Rollover();
            }
        
            BlueLED( 0 );
            GreenLED( 1 );
        
            for ( i = 0; i < 50; i++ ) 
            {
                WaitForTimer0Rollover();
            }
        }
    }
    else
    {
        puts( "Run Mode\n" );
    }

    InitLineThresh();

    //gLineThresh = 12; // Special hack for Robothon

    DBG( puts( "LineThresh: " ); )
    DBG( putu8( gLineThresh ); )
    DBG( puts( "\n" ); )

    puts( "Go\n" );

    while ( 1 )
    {
        // Wait for 100 Hz timer

        WaitForTimer0Rollover();
        gHuntCounter++;

#if DEBUG || 1
        if ( UART_IsCharAvailable() )
        { 
            UART_GetChar();
#if 1
            DebugKey();
#else
            /*
             * A key was pressed. pause and wait for another
             */

            puts( "Paused\n" );

            while ( !UART_IsCharAvailable() ) ;

            puts( "Go\n" );
            UART_GetChar();
#endif
        }
#endif

        ReadSensors();

#if USE_TILT
        if ( gTiltState != 0 )
        {
            AvoidTilt();
        }
#endif

#if 1
        // Clamp distances which are more than across the ring
      
        //if ( !gIrDetected )
        {
            if ( gEyeL < gEyeMinSignal )
            {
                gEyeL = 0;
            }
            if ( gEyeR < gEyeMinSignal )
            {
                gEyeR = 0;
            }
            if ( gEyeRSide < EYE_MIN_SIGNAL_SIDE )
            {
                gEyeRSide = 0;
            }
            if ( gEyeLSide < EYE_MIN_SIGNAL_SIDE )
            {
                gEyeLSide = 0;
            }
        }
#endif

#if DEBUG
        if (( gHuntCounter & 0x0F ) == 0 )
        {
            gDeferredNewline = 0;

            puts( "\rLL: " );
            putu8( gLineLO );
            puts( " " );
            putu8( gLineLI );
            puts( " LR: " );
            putu8( gLineRI );
            puts( " " );
            putu8( gLineRO );
            
            puts( " EL: " );
            putu8( gEyeL );
            puts( " ER: " );
            putu8( gEyeR );

            puts( " ELS: " );
            putu8( gEyeLSide );
            puts( " ERS: " );
            putu8( gEyeRSide );

#if 0
            puts( " IR: " );
            putu8( gIrDetected );
            puts( " (" );
            putu8( gDetectCount );
            puts( ")" );
#endif
            puts( " SL: " );
            putu8( gSpeedL );
            puts( " SR: " );
            putu8( gSpeedR );
#if USE_TILT
            puts( " TX: " );
            putu8( gTiltX );
            puts( " TY: " );
            putu8( gTiltY );
#endif
            puts( " C: " );
            putu16( gHuntCounter );                   

            gDeferredNewline = 1;
        } 
#endif

        gSpeedL = SPEED_FWD;
        gSpeedR = SPEED_FWD;

        //if ( !gIrDetected )
        {
            // Check line sensors

            lineSide = 0;

            if ( LineDetectedRight() )
            {
                DBG( puts( "LineR detected (" ); )
                DBG( putu8( gLineRI ); )
                DBG( puts( " " ); )
                DBG( putu8( gLineRO ); )
                DBG( puts( ") C:" ); )
                DBG( putu16( gHuntCounter ); )
                DBG( puts( "\n" ); )
                
                lineSide |= LINE_DETECTED_RIGHT;
                //if ( !gIrDetected )
                {
                    // Set the hunt direction to be left so that we hunt "away" from the line.
    
                    gHuntDir = HUNT_LEFT;
                }
            }
            if ( LineDetectedLeft() )
            {
                DBG( puts( "LineL detected (" ); )
                DBG( putu8( gLineLO  ); )
                DBG( puts( " " ); )
                DBG( putu8( gLineLI  ); )
                DBG( puts( ") C:" ); )
                DBG( putu16( gHuntCounter ); )
                DBG( puts( "\n" ); )
                
                lineSide |= LINE_DETECTED_LEFT;
                //if ( !gIrDetected )
                {
                   // Set the hunt direction to be right so that we hunt "away" from the line.
                
                   gHuntDir = HUNT_RIGHT;
                }
            }
                
            if ( lineSide != 0 )
            {
                AvoidEdge( lineSide );  
                    
                // We set the Hunt Counter to 50 so that it will spin as opposed
                // to arc.
                
                gHuntCounter = COUNTER_SPIN_COUNT;
                
                // Raise the eye threshold since we hit the line. Otherwise, if
                // we've locked on to something outside the ring we'll just
                // keep running towards it.
                
                gEyeMinSignal = EYE_MIN_SIGNAL2;
            }
        }

      // Bigger number = closer distance

      gEyeState = 0;

#if 1
      if ( gEyeL >= gEyeMinSignal )
      {
         // We have some object in the left

         gEyeState |= EYE_DETECTED_LEFT;
      }
      if ( gEyeR >= gEyeMinSignal )
      {
         // We have some object in the right

         gEyeState |= EYE_DETECTED_RIGHT;
      }
#endif

      eyeDiff = gEyeL - gEyeR;
      if ( eyeDiff > EYE_MIN_DIFF )
      {
         // Object closer in left than in right. This has preference 
         // over mere "object" detected, so we blank out the right signal.

         gEyeState = EYE_DETECTED_LEFT;
      }
      else
      {
         eyeDiff = gEyeR - gEyeL;
         if ( eyeDiff > EYE_MIN_DIFF )
         {
            // Object closer in right than in left. This has preference
            // over mere "object" detected, so we blank out the left signal.

            gEyeState = EYE_DETECTED_RIGHT;
         }
      }

      if ( gEyeLSide >= EYE_MIN_SIGNAL_SIDE )
      {
          gSpeedL = SPEED_BWD_SPIN;
          gSpeedR = SPEED_FWD_SPIN;
          gHuntDir = HUNT_LEFT;
          gHuntCounter = COUNTER_SPIN_COUNT;

          gEyeState |= EYE_DETECTED_LEFT_SIDE;

          gReason = "EyeL-Side";
      }
      else
      if ( gEyeRSide >= EYE_MIN_SIGNAL_SIDE )
      {
          gSpeedL = SPEED_FWD_SPIN;
          gSpeedR = SPEED_BWD_SPIN;
          gHuntDir = HUNT_RIGHT;
          gHuntCounter = COUNTER_SPIN_COUNT;

          gEyeState |= EYE_DETECTED_RIGHT_SIDE;

          gReason = "EyeR-Side";
      }
      else
      if ( gEyeState == EYE_DETECTED_LEFT )
      {
         // Object detected on the left side only. Turn towards it

         gSpeedR = SPEED_FWD;
         gSpeedL = SPEED_FWD_MED;
         
         //if ( gHuntDir != HUNT_LEFT )
         {
            //DBG( puts( "Resetting gHuntCounter L\n" ); )
            gHuntCounter = 0;
         }
         gHuntDir = HUNT_LEFT;

         gReason = "EyeL";
      }
      else
      if ( gEyeState == EYE_DETECTED_RIGHT )
      {
         // Object detected on the right side only. Turn towards it

         gSpeedR = SPEED_FWD_MED;
         gSpeedL = SPEED_FWD;
         
         //if ( gHuntDir != HUNT_RIGHT )
         {
            //DBG( puts( "Resetting gHuntCounter R\n" ); )
            gHuntCounter = 0;
         }
         gHuntDir = HUNT_RIGHT;

         gReason = "EyeR";
      }
      else      
      //if (( gEyeState == 0 ) && !gIrDetected )
      if ( gEyeState == 0 )
      {
         // No object detected and the other guy isn't close, so go into
         // hunt mode.

         if ( gHuntCounter < COUNTER_TURN_COUNT )
         {
            if ( gHuntDir == HUNT_LEFT )
            {
               DBG( huntState = HUNT_STATE_SPIN_LEFT; )

               // Spin Left               
               if ( gHuntCounter < COUNTER_SPIN_COUNT )
               {
                  // Arc forward turning left

                  gSpeedL = SPEED_FWD_MED;
                  gSpeedR = SPEED_FWD;

                  gReason = "Hunt ArcL";
               }
               else
               {
                  // Spin to the left

                  gSpeedL = SPEED_BWD_SPIN;
                  gSpeedR = SPEED_FWD_SPIN;

                  gReason = "Hunt SpinL";
               }
            }
            else
            {
               DBG( huntState = HUNT_STATE_SPIN_RIGHT; )

               // Spin Right

               if ( gHuntCounter < COUNTER_SPIN_COUNT )
               {
                  // Arc forward turning right

                  gSpeedR = SPEED_FWD_MED;
                  gSpeedL = SPEED_FWD;

                  gReason = "Hunt SpinL";
               }
               else
               {
                  // Spin right

                  gSpeedR = SPEED_BWD_SPIN;
                  gSpeedL = SPEED_FWD_SPIN;

                  gReason = "Hunt SpinR";
               }
            }
         }
         else
         if ( gHuntCounter < COUNTER_STRAIGHT_COUNT )
         {
            DBG( huntState = HUNT_STATE_GO_STRAIGHT; )

            // Go forward

            gSpeedL = SPEED_FWD;
            gSpeedR = SPEED_FWD;

            gReason = "Hunt Straight";
         }
         else
         {
            // Counter wrapped and we still didn't find anybody.
            // Swap hunt direction so we don't get "stuck in a corner"

            gHuntCounter = 0;
            gHuntDir = SWAP_HUNT_DIR( gHuntDir );

            gReason = "Hunt Wrapped";
         }

         if ( gHuntCounter >= COUNTER_EYE_RESET_COUNT )
         {
            gEyeMinSignal = EYE_MIN_SIGNAL;
         }
      }
#if DEBUG
      if ( gEyeState != gPrevEyeState )
      {
         puts( "EyeState changed: " );
         if (( gEyeState & EYE_DETECTED_LEFT ) != 0 )
         {
            putc( 'L' );
         }
         else
         {
            putc( ' ' );
         }
         if (( gEyeState & EYE_DETECTED_RIGHT ) != 0 )
         {
            putc( 'R' );
         }
         else
         {
            putc( ' ' );
         }

         putc( ' ' );

         if (( gEyeState & EYE_DETECTED_LEFT_SIDE ) != 0 )
         {
            putc( 'L' );
            putc( 'S' );
         }
         else
         {
            putc( ' ' );
            putc( ' ' );
         }

         if (( gEyeState & EYE_DETECTED_RIGHT_SIDE ) != 0 )
         {
            putc( 'R' );
            putc( 'S' );
         }
         else
         {
            putc( ' ' );
            putc( ' ' );
         }

         puts( " EyeL:" );
         putu8( gEyeL );
         puts( " EyeR:" );
         putu8( gEyeR );
         puts( " ELS: " );
         putu8( gEyeLSide );
         puts( " ERS: " );
         putu8( gEyeRSide );
         puts( " C: " );
         putu16( gHuntCounter );
         puts( "\n" );
   
         gPrevEyeState = gEyeState;
      }
      if ( huntState != prevHuntState )
      {
         if ( huntState != 0 )
         {
            puts( "Hunt: " );

            switch ( huntState )
            {
               case HUNT_STATE_SPIN_LEFT:
               {
                  puts( "Spin Left" );
                  break;
               }

               case HUNT_STATE_SPIN_RIGHT:
               {
                  puts( "Spin Right" );
                  break;
               }

               case HUNT_STATE_GO_STRAIGHT:
               {
                  puts( "Go Forward" );
                  break;
               }

               default:
               {
                  puts( "*** Unknown ***" );
                  break;
               }
            }

            puts( " C: ");
            putu16( gHuntCounter );
            puts( "\n" );
         }
         prevHuntState = huntState;
      }
#endif
      
      PulseMotors();
   }

   return 0;

} // main

/** @} */

