/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Hardware.h"
#include "Timer.h"
#include "UART.h"

#include "QD.h"

// Note: the Dial_xxx defines are in Config.h

#include "QD.h"

#define  Dial_A_PIN  PINC
#define  Dial_B_PIN  PINC
#define  Dial_A_BIT  1
#define  Dial_B_BIT  0

volatile    QD_Counter_t    Dial_Count;
            QD_State_t      Dial_PrevState;

ISR( TIMER2_OVF_vect )
{
    QD_Update( Dial );
}

int main(void)
{
    int     i;
    int     led = 0;

    InitHardware();

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

    printf( "*****\n" );
    printf( "***** QD-Test program\n" );
    printf( "*****\n" );

    // Setup Timer2 to overflow about 7812.5 times per second
    // The actual update rate required will depend largely on the type of 
    // input. In my case, I was using a dial that could be spun by hand and 
    // updating 1000/sec wasn't fast enough. The dial would increment 400 
    // times in a single revolution and could be spun faster then one 
    // rev/second, so a faster update was required. With the faster updates, 
    // no counts were lost.

    TCCR2 = TIMER2_CLOCK_SEL_DIV_8;
    TIMSK |= ( 1 << TOIE2 );

    while( 1 )
    {
        // Turn all of the LEDs off

        LED_OFF( RED );
        LED_OFF( BLUE );
        LED_OFF( YELLOW );

        switch ( led )
        {
            case    0:  LED_ON( RED );      break;
            case    1:  LED_ON( BLUE );     break;
            case    2:  LED_ON( YELLOW );   break;
        }

        if ( ++led > 2 )
        {
            led = 0;
        }

        printf( "Dial: %d\n", QD_Count( Dial ));

        // Tick rate is 100/sec so waiting for 100 waits for 1 sec

        for ( i = 0; i < 100; i++ ) 
        {
            WaitForTimer0Rollover();

            if ( UART0_IsCharAvailable() )
            {
                getchar();

                printf( "Resetting Count\n" );
                QD_SetCount( Dial, 0 );
            }
        }
    }

    return 0;
}

