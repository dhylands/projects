/****************************************************************************
*
*   Pulse-Stretcher.c
*
*   Code for stretching out an RC pulse
*
****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Debug.h"
#include "Hardware.h"
#include "LED.h"
#include "RCPulse.h"
#include "Uart.h"

#include <stdlib.h>

typedef struct
{
    uns8    start;
    uns8    mid;
    uns8    end;
}  SetPoint;

// 0 - Channel 1 Input
// 1 - Channel 2 Input
// 3 - Channel 1 Output
// 4 - Channel 2 Output

SetPoint    gSetPoint[ 4 ] =
{
    {
        123,    // 0.984 msec - Forward
        181,    // 1.448 msec
        207,    // 1.656 msec - Reverse
    },

    {
        149,    // 1.192 msec - Left
        191,    // 1.528 msec - Straight
        237,    // 1.896 msec - Right
    },

    {
        125,    // 1.000 msec
        187,    // 1.496 msec
        250,    // 2.000 msec
    },

    {
        125,    // 1.000 msec
        187,    // 1.496 msec
        250,    // 2.000 msec
    },
};

char gMsg[ 80 ];
char *gMsgGet = gMsg;
char *gMsgPut = gMsg;

/***************************************************************************/
/**
*  Format a number of 8us ticks into a string
*/

void FormatRCTicks( uns8 ticks, char **strPtr )
{
    uns16   usecs = ticks;
    usecs *= 8;
    char *digitStr = "0123456789";
    char *s = *strPtr;

    uns8    digit;

    digit = 0;
    while ( usecs >= 1000 )
    {
        digit++;
        usecs -= 1000;
    }
    *s++ = digitStr[ digit ];
    *s++ = '.';

    digit = 0;
    while ( usecs >= 100 )
    {
        digit++;
        usecs -= 100;
    }
    *s++ = digitStr[ digit ];

    digit = 0;
    while ( usecs >= 10 )
    {
        digit++;
        usecs -= 10;
    }
    *s++ = digitStr[ digit ];
    *s++ = digitStr[ usecs ];
    *s = '\0';

    *strPtr = s;

} // FormatRCTicks

/***************************************************************************/
/**
*  Format a string into a string
*/

void FormatStr( const char *inStr, char **strPtr )
{
    char *s = *strPtr;

    while ( *inStr != '\0' )
    {
        *s++ = *inStr++;
    }
    *s = '\0';

    *strPtr = s;

} // FormatStr

/***************************************************************************/
/**
*  Maps an input channel to an output channel
*/

uns8 MapPulse( uns8 pulse, SetPoint *in, SetPoint *out )
{
    int16 inPulse;
    int16 outPulse;
    int16 inWidth;
    int16 outWidth;

    if ( pulse < in->mid )
    {
        // out->mid - ( in->mid - pulse ) / ( in->mid - in->start ) * ( out->mid - out->start )

        inWidth  = (int16)in->mid  - (int16)in->start;
        outWidth = (int16)out->mid - (int16)out->start;
        inPulse  = (int16)in->mid  - (int16)pulse;

        outPulse = (int16)out->mid - (( inPulse * outWidth ) / inWidth );
    }
    else
    {
        // out->mid + ( pulse - in->mid ) / ( in->end - in->mid ) * ( out->end - out->mid )

        inWidth  = (int16)in->end  - (int16)in->mid;
        outWidth = (int16)out->end - (int16)out->mid;
        inPulse  = (int16)pulse - (int16)in->mid;

        outPulse = (int16)out->mid + (( inPulse * outWidth ) / inWidth );
    }

    return outPulse;

} // MapPulse

/***************************************************************************/
/**
*  Generates a pulse of the indicated width on the indicated pin
*/

void GeneratePulse( uns8 width, int pin )
{
    // Wait for the timer/counter to roll over

    while ( TCNT2 != 0xFF )
    {
        ;
    }

    // Wait for the 0xFF --> 0x00 rollover

    while ( TCNT2 != 0 )
    {
        ;
    }

    PORTD |= BV( pin );

    // Now wait for the width of the pulse

    while ( TCNT2 < width )
    {
        ;
    }

    PORTD &= ~BV( pin );

} // GeneratePulse


// Throttle (Channel 0)
//  Off: 1.448
//  Full: 0.984
//  Rev:  1.656
//
// Steering (Channel 1)
//  Left:       1.192
//  Straight:   1.528
// Right:       1.896

/***************************************************************************/
/**
*  Main program for the Mini Sumo
*/

int main( void )
{
    int pulseCount = 0;

    // Initialization

    InitHardware();

    BlueLED( 1 );

#if DEBUG
    UART_PutStr( "\r\n\r\n" );
    UART_PutStr( "*****************************\r\n" );
    UART_PutStr( "**                         **\r\n" );
    UART_PutStr( "** Pulse Stretcher Program **\r\n" );
    UART_PutStr( "**                         **\r\n" );
    UART_PutStr( "*****************************\r\n\r\n" );
#endif

    sei();  // Enable interrupts

    while ( 1 )
    {
        if ( RC0.done && RC1.done )
        {
            BeginCritical();
            {
                RC0.width = RC0.end - RC0.start;
                RC1.width = RC1.end - RC1.start;

                RC0.done = FALSE;
                RC1.done = FALSE;
            }
            EndCritical();

            pulseCount++;

            GeneratePulse( MapPulse( RC0.width, &gSetPoint[ 0 ], &gSetPoint[ 2 ]), 5 );
            GeneratePulse( MapPulse( RC1.width, &gSetPoint[ 1 ], &gSetPoint[ 3 ]), 6 );
        }

        if ( gMsgGet == gMsgPut )
        {
            gMsgGet = gMsg;
            gMsgPut = gMsg;

            FormatStr( "\r0: ", &gMsgPut );
            FormatRCTicks( RC0.width, &gMsgPut );
            FormatStr( "ms 1: ", &gMsgPut );
            FormatRCTicks( RC1.width, &gMsgPut );
            FormatStr( "ms", &gMsgPut );

            pulseCount = 0;
        }
        else
        {
            if ( pulseCount > 10 )
            {
                if ( UART_IsTxBufferEmpty() )
                {
                    UART_PutChar( *gMsgGet++ );
                }
            }
        }
    }

    return 0;

} // main

/** @} */

