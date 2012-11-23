/****************************************************************************
*
*  Program for Twister, a Line Maze robot.
*
****************************************************************************/

#include <stdio.h>
#include <68332/isr.h>
#include <68332/qsm.h>
#include <68332/sim.h>
#include <68332/tpu.h>
#include <68332/fqd.h>
#include <68332/mrm/led.h>
#include <68332/mrm/lcd.h>
#include <../libsim/68332/sim_regs.h>
#include <assert.h>

#include "Motor.h"
#include "DebugKey.h"
#include "Tick.h"

#define BAUD_RATE 0     // 0 means use the current baud rate

int gTickFlag = 0;
int gTickOverrun = 0;
volatile int gTickCounter  = 0;

Motor    gLeftMotor(  "Left " );
Motor    gRightMotor( "Right" );
TPUChannel  *gFqdA;
TPUChannel  *gFqdB;

void ReadAD( void );
void TickHandler( void );
void StraightLine( void );

typedef unsigned int    sample_t;

volatile sample_t gAD[ 8 ];
volatile sample_t gMaxAD[ 8 ];

int  gADScale[ 8 ]; // 100 = 1.0
int  gADOffset[ 8 ];

#define CENTER_L    0
#define CENTER_R    1
#define OUTSIDE_L   2
#define OUTSIDE_R   3

#define WHITE_THRESH    ((sample_t)15)
#define BLACK_THRESH    ((sample_t)40)
#define STRAIGHT_THRESH ((sample_t)10)  // Diff allowed to keep going straight

bool IsWhite( sample_t sample )
{
    return sample < WHITE_THRESH;
}

bool IsBlack( sample_t sample )
{
    return sample > BLACK_THRESH;
}

enum SampleClass
{
    Black,
    Gray,
    White
};

const char gSampleClassChar[ 3 ] = { 'b', '-', 'w' };

SampleClass ClassifySample( sample_t sample )
{
    if ( IsBlack( sample ))
    {
        return Black;
    }
    if ( IsWhite( sample ))
    {
        return White;
    }
    return Gray;
}

int main( void )
{
    int  nextTick;
    int  ledState = 0;
    SIM_T *sim = (SIM_T *)SIM_BASE;
    
    set_processor_speed( CLOCK25MHZ );
    
    /*
     * since sciinit is required for all programs that do i/o
     * perhaps it should be in mrm_crt0.S also?  It would still
     * be available if the user wanted to change baud rate.
     */
    
    sciinit( BAUD_RATE );
    
    led_init();
    lcd_init( 2, 24, 0, FALSE );
    
    tpu_init( 0 );
    iprintf( "TCR1 freq = %d, TCR2 freq = %d\n", tpu_get_tcr1_freq( 0 ), tpu_get_tcr2_freq( 0 ));
    
    // For the Solarbotics Secret Motor Controller, we've got them plugged in
    // as follows:
    //             
    // Right              Left
    // ------------ -------------
    // Green  D2  6  7 D2 Green
    // Yellow En  8  9 En Yellow
    // Orange D1 10 11 D1 Orange
    
    gRightMotor.Init( 10, 6, 8 );
    gLeftMotor.Init( 11, 7, 9 );
    
    /*
     * Initialize the periodic timer. A value of 8 should cause a 1024 Hz
     * interrupt.
     *
     * With PTP == 0, you get interrupted 8192/PITM times per second.
     * With PTP == 1, you get interrupted   16/PITM times per second.
     */
    
    gTickCounter = 0;
    install_isr( 64, TickHandler );
    sim->PITR.BITS.PTP   = 0;
    sim->PITR.BITS.PITM  = 8192 / TICK_HZ;
    sim->PICR.BITS.PIRQL = 6;
    sim->PICR.BITS.PIV   = 64;

    gFqdA = tpu_request_channel( 0, 2 );
    gFqdB = tpu_request_channel( 0, 3 );

    for ( int i = 0; i < 8; i++ ) 
    {
        gMaxAD[ i ] = 0;
        gADScale[ i ] = 100;
        gADOffset[ i ] = 0;
    }
    gADScale[ CENTER_R ] = 150;
    gADOffset[ CENTER_R ] = -4;

    while ( 1 )
    {
        if ( havebyte() )
        {
            inbyte();
            DebugKey();
        }
        
        nextTick = gTickCounter >> 8;
        while ( nextTick == ( gTickCounter >> 8 ))
        {
            ;
        }
        
        if ( ledState )
        {
            int min = nextTick / 120;
            int sec = ( nextTick / 2 ) % 60;
            uint16 fqdCount = fqd( gFqdA, gFqdB );
            
            iprintf( "\r%02d:%02d", min, sec );
            
            //lcd_printf( "\f%02d:%02d", min, sec );
            lcd_printf( "\f" );
            
            iprintf( " %3d~%3d~%3d~%3d~%3d~%3d~%3d~%3d", 
                    gAD[0], gAD[1], gAD[2], gAD[3], gMaxAD[0], gMaxAD[1], gMaxAD[2], gMaxAD[3] );

            lcd_printf( "AD: %3d %3d %3d %3d\nMx: %3d %3d %3d %3d",
                    gAD[0], gAD[1], gAD[2], gAD[3], gMaxAD[0], gMaxAD[1], gMaxAD[2], gMaxAD[3] );

            iprintf( " FQD: %5u", fqdCount );
            //lcd_printf( " F:%u", fqdCount );
            
            fflush( stdout );
            
            led_on( GreenLED );
            led_off( RedLED );
        }
        else
        {
            led_off( GreenLED );
            led_on( RedLED );
        }
        ledState = !ledState;
   }

   exit( 0 );
}

/****************************************************************************
*
*  Reads A/D channels 0 and 1, and prints the results on the LCD
*
****************************************************************************/

void ReadAD( void )
{
   gAD[ 0 ] = *(unsigned char *)0xf00000; // Latch address 0
   gAD[ 0 ] = *(unsigned char *)0xf00001; // Start conversion 0, latch 1
   gAD[ 0 ] = *(unsigned char *)0xf00002; // Read 0, start conversion 1, latch 2
   gAD[ 1 ] = *(unsigned char *)0xf00003; // Read 1, start conversion 2, latch 3
   gAD[ 2 ] = *(unsigned char *)0xf00004; // Read 2, start conversion 3, latch 4
   gAD[ 3 ] = *(unsigned char *)0xf00005; // Read 3, start conversion 4, latch 5
   gAD[ 4 ] = *(unsigned char *)0xf00006; // Read 4, start conversion 5, latch 6
   gAD[ 5 ] = *(unsigned char *)0xf00007; // Read 5, start conversion 6, latch 7
   gAD[ 6 ] = *(unsigned char *)0xf00000; // Read 6, start conversion 7
   gAD[ 7 ] = *(unsigned char *)0xf00000; // Read 7

   for ( int i = 0; i < 8; i++ ) 
   {
       sample_t newSample = ( ( gAD[ i ] + gADOffset[ i ] ) * gADScale[ i ] ) / 100;

       if ( newSample > 512 )
       {
           iprintf( "i = %d gAD = %d gADOffset = %d gADScale = %d\n",
                    i, gAD[ i ], gADOffset[ i ], gADScale[ i ] );
       }
       
       gAD[ i ] = newSample;
   }

} /* ReadAD */

/****************************************************************************
*
*  Called for each tick of the Periodic Interval Timer.
*
****************************************************************************/

void TickHandler( void )
{

   if( gTickFlag )
   {
      gTickOverrun++;
      return;
   }
   gTickFlag = 1;

   gTickCounter++;

   ReadAD();

   for ( int i = 0; i < 7; i++ ) 
   {
       if ( gAD[ i ] > gMaxAD[ i ] )
       {
           gMaxAD[ i ] = gAD[ i ];
       }
   }

   gLeftMotor.ProcessTick();
   gRightMotor.ProcessTick();
   
   gTickFlag = 0;
}

void StraightLine()
{
    iprintf( "Straight Line\n" );

    tick_t prevTick = gTickCounter;

    gLeftMotor.SetSpeed( Motor::MaxSpeed );
    gRightMotor.SetSpeed( Motor::MaxSpeed );

    enum Action
    {
        MoveLeft,
        MoveRight,
        KeepGoing
    };
       
    Action  action = KeepGoing;
    Action  lastAction = KeepGoing;

    SampleClass classL = Gray;
    SampleClass classR = Gray;
    SampleClass lastClassL = Gray;
    SampleClass lastClassR = Gray;

    #define ADJUST_SPEED_LOW    500
    #define ADJUST_SPEED_HIGH   500

    int     adjustSpeed = 0;
    int     lastAdjustSpeed = 0;

    tick_t  lastLineTime = gTickCounter;

    while ( true )
    {
        if ( havebyte() )
        {
            inbyte();
            gLeftMotor.SetSpeed( 0 );
            gRightMotor.SetSpeed( 0 );
            return;
        }
        while ( prevTick == gTickCounter )
        {
            if ( havebyte() )
            {
                inbyte();
                gLeftMotor.SetSpeed( 0 );
                gRightMotor.SetSpeed( 0 );
                return;
            }
        }
        prevTick = gTickCounter;
        
        sample_t centerL = gAD[ CENTER_L ];
        sample_t centerR = gAD[ CENTER_R ];

        classL = ClassifySample( centerL );
        classR = ClassifySample( centerR );
        
        if (( classL == White ) && ( classR == White ))
        {
            // Left and Right are both white

            if (( gTickCounter - lastLineTime ) > Milliseconds2Ticks( 500 ))
            {
                // We've run out of line

                gLeftMotor.Coast();
                gRightMotor.Coast();
                return;
            }
        }
        else
        {
            lastLineTime = gTickCounter;

            if ( classL == White )
            {
                // Left is white, Right is gray or black.

                // We're veering to the left. Adjust to the right.

                action = MoveRight;
                adjustSpeed = ADJUST_SPEED_HIGH;
            }
            else
            if ( classR == White)
            {
                // Right is white, Left is gray or black.

                // We're veering to the right. Adjuts to the left.

                action = MoveLeft;
                adjustSpeed = ADJUST_SPEED_HIGH;
            }
            else
            {
#if 0
                if (( classL == Black ) || ( classR == Black ))
                {
                    // We're probably over a corner, T, or cross

                    // Keep going

                    action = KeepGoing;
                }
                else
#endif
                {
                    if (( centerL + STRAIGHT_THRESH ) < centerR )
                    {
                        action = MoveRight;
                        adjustSpeed = ADJUST_SPEED_LOW;
                    }
                    else
                    if (( centerR + STRAIGHT_THRESH ) < centerL )
                    {
                        action = MoveLeft;
                        adjustSpeed = ADJUST_SPEED_LOW;
                    }
                }
            }
        }

#if 1
        if (( action != lastAction ) || ( adjustSpeed != lastAdjustSpeed ) || ( classL != lastClassL ) || ( classR != lastClassR ))
        {
            iprintf( "Action: %s LSpd: %4d RSpd: %4d Adj: %4d CL:%3d%c CR:%3d%c\n\n",
                     ( action == MoveLeft ) ? "ML" : ( action == MoveRight ) ? "MR" : "KG",
                     gLeftMotor.GetSpeed(), gRightMotor.GetSpeed(),
                     adjustSpeed,
                     centerL, gSampleClassChar[ classL ],
                     centerR, gSampleClassChar[ classR ] );

            lastAction = action;
            lastAdjustSpeed = adjustSpeed;
            lastClassL = classL;
            lastClassR = classR;
        }
#endif

        switch ( action )
        {
            case MoveRight:
            {
                // Need to move right (i.e. speed up left side/slow down right side)

#if 1
                iprintf( "CL:%3d%c ",
                         centerL, gSampleClassChar[ classL ] );
                
                gLeftMotor.SetSpeed( Motor::MaxSpeed );
                
                iprintf( "CR:%3d%c ",
                         centerR, gSampleClassChar[ classR ] );

                gRightMotor.SetSpeed( 800 );
#else
                iprintf( "CL:%3d%c ",
                         centerL, gSampleClassChar[ classL ] );
                
                int unused = gLeftMotor.AdjustSpeed( adjustSpeed );
                
                iprintf( "CR:%3d%c ",
                         centerR, gSampleClassChar[ classR ] );

                gRightMotor.AdjustSpeed( -unused );
#endif
                iprintf( "\n" );
                break;
            }

            case MoveLeft:
            {
                // Need to move left (i.e. speed up right side/slow down left side)

#if 1
                iprintf( "CR:%3d%c ",
                         centerR, gSampleClassChar[ classR ] );

                gRightMotor.SetSpeed( Motor::MaxSpeed );

                iprintf( "CL:%3d%c ",
                         centerL, gSampleClassChar[ classL ] );

                gLeftMotor.SetSpeed( 800 );
#else
                iprintf( "CR:%3d%c ",
                         centerR, gSampleClassChar[ classR ] );

                int unused = gRightMotor.AdjustSpeed( adjustSpeed );

                iprintf( "CL:%3d%c ",
                         centerL, gSampleClassChar[ classL ] );

                gLeftMotor.AdjustSpeed( -unused );
#endif
                iprintf( "\n" );
                break;
            }

            default:
            {
                // Keep going
                break;
            }
        }
    }

} // StraightLine

