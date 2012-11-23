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

#include <avr/io.h>

#include <stdarg.h>

#include "a2d.h"
#include "Hardware.h"
#include "DebugKey.h"
#include "Delay.h"
#include "LCD.h"
#include "Log.h"
#include "Str.h"
#include "UART.h"
#include "Expansion.h"
#include "Encoder.h"

static int printfFunc( void *outParm, int ch )
{
    UART0_PutChar( ch );

    return 1;
}

uint8_t gADC[ 8 ];

#define C   2092
#define D   2348
#define E   2636
#define G   3136
#define R   0

uint16_t  MaryHadALittleLamb[ 29 ] =
{
    E,D,C,D,E,E,E,R,D,D,D,R,E,G,G,R,E,D,C,D,E,E,E,E,D,D,E,D,C
};

int main(void)
{
    int         i;
    int         led = 0;
    uint16_t    prevSwitches = 0;

    InitHardware();

    LogInit( printfFunc );

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

    Log( "*****\n" );
    Log( "***** Flash-LED program - 2\n" );
    Log( "*****\n" );

    Log( "SPCR = 0x%02x\n", SPCR );

    LCD_Init();
    LCD_Printf( " SRS Sample Bot " );
    LCD_MoveTo( 0, 1 );
    LCD_Printf( "Second line" );

    Log( "\n" );

    LED_OFF( GREEN );

    while( 1 )
    {
        uint16_t    switches;
        uint8_t     pinc;
        uint8_t     pind;

        LED_TOGGLE( GREEN );

        led++;
        if ( led >= 6 )
        {
            led = 0;
        }

        //switches = EXP_TransferWord( ~( 1 << ( led + 2 )), EXP_OUT_LED_MASK );
        switches = EXP_TransferWord( ~0, EXP_OUT_LED_MASK );
        Log( "SW:%04x ", switches ); 

        if ( switches != prevSwitches )
        {
            LCD_Clear();
            prevSwitches = switches;
        }
        // Read the ADC

        for ( i = 0; i < 8; i++ ) 
        {
            gADC[ i ] = a2d_8( i );

            Log( "%02x ", gADC[ i ]);
        }
        //Log( "C: %02x\n", PINC );


        switch (( switches & 0xF0 ) >> 4 )
        {
            case 0:
            {
                LCD_MoveTo( 0, 0 );
                LCD_Printf( " SRS Sample Bot " );
                LCD_MoveTo( 0, 1 );
                LCD_Printf( "Second line" );
                break;
            }

            case 1:
            {
                LCD_MoveTo( 0, 0 );
                LCD_Printf( "Joy: %c%c%c%c%c", 
                            (( switches & 0x4000 ) == 0 ) ? 'L' : ' ',
                            (( switches & 0x2000 ) == 0 ) ? 'R' : ' ',
                            (( switches & 0x0800 ) == 0 ) ? 'U' : ' ',
                            (( switches & 0x0400 ) == 0 ) ? 'D' : ' ',
                            (( switches & 0x1000 ) == 0 ) ? 'X' : ' ' );
                LCD_MoveTo( 0, 1 );
                LCD_Printf( "S1:%d S2:%d S3:%d",
                            ( switches & 0x0200 ) == 0,
                            ( switches & 0x0100 ) == 0,
                            ( PIND & ( 1 << 6 ))  == 0 );
                break;
            }

            case 2:
            {
                LCD_MoveTo( 0, 0 );
                LCD_Printf( "L %02x %02x %02x %02x %02x", gADC[ 4 ], gADC[ 1 ], gADC[ 2 ], gADC[ 3 ], gADC[ 5 ] );
                LCD_MoveTo( 0, 1 );
                LCD_Printf( "E %02x %02x     B %02x", gADC[ 0 ], gADC[ 7 ], gADC[ 6 ] );
                break;
            }

            default:
            {
                LCD_MoveTo( 0, 0 );
                LCD_Printf( "Setting: %d", ( switches & 0xF0 ) >> 4 );
                break;
            }
        }

        pinc = PINC;
        pind = PIND;

        Log( " QL:%d%d QR:%d%d EC-L:%d EC-R:%d\n", 
             ( ENCODER_L_A_PIN & ENCODER_L_A_MASK ) != 0,
             ( ENCODER_L_B_PIN & ENCODER_L_B_MASK ) != 0,
             ( ENCODER_R_A_PIN & ENCODER_R_A_MASK ) != 0,
             ( ENCODER_R_B_PIN & ENCODER_R_B_MASK ) != 0,
             gEncoderCountL,
             gEncoderCountR );


        // Tick rate is 100/sec so waiting for 50 waits for 1/2 sec

        for ( i = 0; i < 50; i++ ) 
        {
            WaitForTimer0Rollover();

#if 1
            if ( UART0_IsCharAvailable() )
            {
                char    ch = UART0_GetChar();

                if ( ch == ' ' )
                {
                    DebugKey();
                }
                else
                {
                    Log( "Read: '%c'\n", ch );
                }
            }
#endif
        }
    }

    return 0;
}

