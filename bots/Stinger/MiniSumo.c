/****************************************************************************
*
*  MiniSumo.c
*
*  Mini Sumo code for Wedgy
*
****************************************************************************/

#define DEBUG 1

#include <avr/io.h>
#include <stdlib.h>

#include "Stinger.h"
#include "adc.h"
#include "Avoid.h"
#include "Debug.h"
#include "DebugKey.h"
#include "Delay.h"
#include "Log.h"
#include "Motor.h"
#include "Sensor.h"
#include "Timer.h"
#include "UART.h"

#define  EYE_MIN_SIGNAL    35 // Normal use
#define  EYE_MIN_SIGNAL2   65 // Used for shortly after we hit the line
#define  EYE_MIN_DIFF      10

#define COUNTER_TURN_COUNT  200     // Stop turning and go straight
#define COUNTER_ARC_COUNT   25      // 0 - COUNTER_ARC_COUNT = arc, > COUNTER_ARC_COUNT = SPIN

// After this many ticks of no change in eye state, we push with both motors
// because that gives us the max amount of pushing power. Since there has been 
// no change in eye state, we're probably butting heads
                                                           
#define COUNTER_NO_STATE_CHANGE_PUSH    50

// When we randomize the turn count, we set gHuntTurnCount to a value between
// the min & max and set gHuntCount to the base.

#define AVOID_COUNT_BASE    ( COUNTER_ARC_COUNT )   // Base at Arc count so bot spins
#define AVOID_COUNT_MIN     ( AVOID_COUNT_BASE + 20 )
#define AVOID_COUNT_MAX     ( AVOID_COUNT_BASE + 100 )

// It takes about 6/10 of second to spin, so we'll ignore about 1/3 of a circle.

#define  COUNTER_EYE_RESET_COUNT ( COUNTER_ARC_COUNT + 25 )

#define  COUNTER_STRAIGHT_COUNT  400   // Stop going straight

uns8  gEyeMinSignal;
uns8  gEyeState;
uns16 gEyeStateSameCounter;
uns8  gPrevEyeState;

uns8  gHuntDir;
uns16 gHuntCounter;

uns16 gHuntTurnCount = COUNTER_TURN_COUNT;

static void RandomizeTurnCount( uns16 minCount, uns16 maxCount )
{
    uns32 randNum = rand();

#if 1
    randNum *= ( maxCount - minCount );
    randNum /= RAND_MAX;
    randNum += minCount;
#else
    randNum = maxCount;
#endif
    
    gHuntTurnCount = (uns16)randNum;
}

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

#if DEBUG
    uns8  huntState = 0;
    uns8  prevHuntState = 0;
#endif

    // Initialization

    InitUART();

    InitSensors();
    InitLED();


    Log( "**\n" );
    Log( "** Stinger MiniSumo Program **\n" );
    Log( "**\n" );

    InitTimer();
    InitMotors();

    gDeferredNewline = 0;

    //gLineThresh = 12; // Special hack for Robothon

    DBG( "LineThresh: %u\n", gLineThresh );

    gHuntCounter = 0;

    gEyeState = 0;
    gPrevEyeState = 0;
    gEyeMinSignal = EYE_MIN_SIGNAL;


    if ( IsBlackButtonPressed() )
    {
        // We want to wait for the black button to be released and re-pressed
        // to start the 5 second countdown.

        DBG( "Waiting for Black Button to be released\n" );
        
        while ( 1 )
        {
            if ( !IsBlackButtonPressed() )
            {
                break;
            }

            BlueLED( 1 );
            GreenLED( 1 );

            for ( i = 0; i < 12; i++ ) 
            {
                if ( !IsBlackButtonPressed() )
                {
                    break;
                }
               
                WaitForTimer0Rollover();
            }

            BlueLED( 0 );
            GreenLED( 0 );
        
            for ( i = 0; i < 12; i++ ) 
            {
                if ( !IsBlackButtonPressed() )
                {
                    break;
                }

                WaitForTimer0Rollover();
            }
        }

        // Wait for 100 mSec debounce time
                
        for ( i = 0; i < 10; i++ ) 
        {
            WaitForTimer0Rollover();
        }
        
        DBG( "Waiting for Black Button press to start 5 second delay\n" );

        while ( 1 )
        {
            if ( IsBlackButtonPressed() )
            {
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

        DBG( "Starting 5 second countdown\n" );

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
        
    InitLineThresh();

    // Turn on the eyes, so we can find our opponent.

    EnableLeftEye( TRUE );
    EnableRightEye( TRUE );

    DBG( "Go\n" );

    //*********************************************************************
    //
    // How the gHuntCount works - because I always forget.
    //  
    //  gHuntCounter is incremented each time through the main loop (100 Hz)
    //
    //  Values between 0 and gHuntTurnCount cause the bot to turn.
    //  Values between 0 an COUNTER_ARC_COUNT cause the bot to arc
    //  Values between COUNTER_ARC_COUNT and gHuntTurnCount cause the bot to spin.
    //
    //  Values between gHuntTurnCount and COUNTER_STRAIGHT_COUNT cause the
    //  bot to move forward in a straight line.
    //
    //  When gHuntCount reaches COUNTER_STRAIGHT_COUNT, then the counter
    //  is reset tp zero.
    //
    //  COUNTER_STRAIGHT
    //
    //*********************************************************************

    switch ( PgmSelect() )
    {
        case 0: // Spin Left
        {
            DBG( "Initial Hunt Spin Left\n" );
            gHuntDir = HUNT_LEFT;
            gHuntCounter = COUNTER_ARC_COUNT;       // Start out spinning
            gHuntTurnCount = COUNTER_TURN_COUNT;    // Time we'll revert to straight
            break;
        }
        
        case 1: // Go Straight
        {
            DBG( "Initial Hunt Straight\n" );
            gHuntDir = HUNT_RIGHT;                  // This will get overridden when we hit the line
            gHuntTurnCount = 0;                     // Gives max straight (basically until we hit the line)
            gHuntCounter = gHuntTurnCount;          // Go straight
            break;
        }
        
        case 2: // Spin Right
        {
            DBG( "Initial Hunt Spin Right\n" );
            gHuntDir = HUNT_RIGHT;
            gHuntCounter = COUNTER_ARC_COUNT;       // Start out spinning
            gHuntTurnCount = COUNTER_TURN_COUNT;    // Time we'll revert to straight
            break;                        
        }

        case 3: // Arc Left
        {
            DBG( "Initial Hunt Arc Left\n" );
            gHuntDir = HUNT_LEFT;
            gHuntCounter = 0;                       // Start out arcing
            gHuntTurnCount = COUNTER_TURN_COUNT;    // Time we'll revert to straight
            break;
        }

        case 4: // Arc Right
        {
            DBG( "Initial Hunt Arc Right\n" );
            gHuntDir = HUNT_RIGHT;
            gHuntCounter = 0;                       // Start out arcing
            gHuntTurnCount = COUNTER_TURN_COUNT;    // Time we'll revert to straight
            break;
        }
    }

    //*********************************************************************
    //*********************************************************************
    //**
    //**    Start of main loop
    //**
    //*********************************************************************
    //*********************************************************************

    gEyeStateSameCounter = 0;

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

            DBG( "Paused\n" );

            while ( !UART_IsCharAvailable() ) ;

            DBG( "Go\n" );
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
        }
#endif

#if DEBUG
        if (( gHuntCounter & 0x0F ) == 0 )
        {
            gDeferredNewline = 0;

            DBG( "\rLL: %3u %3u LR: %3u %3u EL: %3u ER: %3u SL: %3u SR: %3u C: %5u",
                 gLineLO, gLineLI, gLineRI, gLineRO, gEyeL, gEyeR, gSpeedL, gSpeedR, gHuntCounter );

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
                DBG( "LineR detected (%3u %3u) C:%3u\n", gLineRI, gLineRO, gHuntCounter );
                
                lineSide |= LINE_DETECTED_RIGHT;
                //if ( !gIrDetected )
                {
                    // Set the hunt direction to be left so that we hunt "away" from the line.
    
                    gHuntDir = HUNT_LEFT;
                }
            }
            if ( LineDetectedLeft() )
            {
                DBG( "LineL detected (%3u %3u) C:%3u\n", gLineLO, gLineLI, gHuntCounter );
                
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
                
                gHuntCounter = AVOID_COUNT_BASE;
                
                RandomizeTurnCount( AVOID_COUNT_MIN, AVOID_COUNT_MAX );
                
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

        if ( gEyeState == EYE_DETECTED_LEFT )
        {
            // Object detected on the left side only. Turn towards it
            
            gSpeedL = SPEED_FWD_MED;
            
            //if ( gHuntDir != HUNT_LEFT )
            {
                //DBG( "Resetting gHuntCounter L\n" );
                gHuntCounter = 0;
                gHuntTurnCount = COUNTER_TURN_COUNT;
            }
            gHuntDir = HUNT_LEFT;
        }
        else
        if ( gEyeState == EYE_DETECTED_RIGHT )
        {
            // Object detected on the right side only. Turn towards it
            
            gSpeedR = SPEED_FWD_MED;
            
            //if ( gHuntDir != HUNT_RIGHT )
            {
                //DBG( "Resetting gHuntCounter R\n" );
                gHuntCounter = 0;
                gHuntTurnCount = COUNTER_TURN_COUNT;
            }
            gHuntDir = HUNT_RIGHT;
        }
        else      
        //if (( gEyeState == 0 ) && !gIrDetected )
        if ( gEyeState == 0 )
        {
            // No object detected and the other guy isn't close, so go into
            // hunt mode.
            
            if ( gHuntCounter < gHuntTurnCount )
            {
                if ( gHuntDir == HUNT_LEFT )
                {
#if DEBUG
                    huntState = HUNT_STATE_SPIN_LEFT;
#endif
                    
                    // Spin Left
                    
                    if ( gHuntCounter < COUNTER_ARC_COUNT )
                    {
                        // Arc forward turning left
                        
                        gSpeedL = SPEED_FWD_MED;
                        gSpeedR = SPEED_FWD;
                    }
                    else
                    {
                        // Spin to the left
                        
                        gSpeedL = SPEED_BWD_SPIN;
                        gSpeedR = SPEED_FWD_SPIN;
                    }
                }
                else
                {
#if DEBUG
                    huntState = HUNT_STATE_SPIN_RIGHT;
#endif
                    
                    // Spin Right
                    
                    if ( gHuntCounter < COUNTER_ARC_COUNT )
                    {
                        // Arc forward turning right
                        
                        gSpeedL = SPEED_FWD;
                        gSpeedR = SPEED_FWD_MED;
                    }
                    else
                    {
                        // Spin right
                        
                        gSpeedL = SPEED_FWD_SPIN;
                        gSpeedR = SPEED_BWD_SPIN;
                    }
                }
            }
            else
            if ( gHuntCounter < COUNTER_STRAIGHT_COUNT )
            {
#if DEBUG
                huntState = HUNT_STATE_GO_STRAIGHT;
#endif
                
                // Go forward
                
                gSpeedL = SPEED_FWD;
                gSpeedR = SPEED_FWD;
            }
            else
            {
                // Counter wrapped and we still didn't find anybody.
                // Swap hunt direction so we don't get "stuck in a corner"
                
                gHuntCounter = 0;
                gHuntDir = SWAP_HUNT_DIR( gHuntDir );

                // Toggle between going straight and turning

                if ( gHuntTurnCount > 0 )
                {
                    gHuntTurnCount = 0;
                }
                else
                {
                    gHuntTurnCount = COUNTER_TURN_COUNT;
                }
            }
            
            if ( gHuntCounter >= COUNTER_EYE_RESET_COUNT )
            {
                gEyeMinSignal = EYE_MIN_SIGNAL;
            }
        }

        if ( gEyeState == gPrevEyeState )
        {
            gEyeStateSameCounter++;
        }
        else
        {
            DBG( "EyeState changed: %c%c EyeL:%3u EyeR:%3u C: %3u SC: %3u\n",
                 (( gEyeState & EYE_DETECTED_LEFT  ) != 0 ) ? 'L' : ' ',
                 (( gEyeState & EYE_DETECTED_RIGHT ) != 0 ) ? 'R' : ' ',
                 gEyeL, gEyeR, gHuntCounter, gEyeStateSameCounter );

            gEyeStateSameCounter = 0;
            gPrevEyeState = gEyeState;
        }

//      if ( gIrDetected )
        if (( gEyeState != 0 ) && ( gEyeStateSameCounter > COUNTER_NO_STATE_CHANGE_PUSH ))
        {
            // No change in the eye sensors, or closeup IR detector means that 
            // we're probably butting heads.
            //
            // We get more pushing power if both motors are going forward. 
            // We still want to maintain our last-seen information, which 
            // is why we put this after the normal processing and override the
            // speed rather than overriding the eye state.
            
            gSpeedL = SPEED_FWD;
            gSpeedR = SPEED_FWD;
        }
        
#if DEBUG
        if ( huntState != prevHuntState )
        {
            if ( huntState != 0 )
            {
                Log( "Hunt: " );
                
                switch ( huntState )
                {
                    case HUNT_STATE_SPIN_LEFT:
                    {
                        Log( "Spin Left" );
                        break;
                    }
                    
                    case HUNT_STATE_SPIN_RIGHT:
                    {
                        Log( "Spin Right" );
                        break;
                    }
                    
                    case HUNT_STATE_GO_STRAIGHT:
                    {
                        Log( "Go Forward" );
                        break;
                    }
                    
                    default:
                    {
                        Log( "*** Unknown ***" );
                        break;
                    }
                }
                
                Log( " C: %3u\n");
            }
            prevHuntState = huntState;
        }
#endif
      
        PulseMotors();
    }

    return 0;

} // main

/** @} */

