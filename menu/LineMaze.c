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
#include <string.h>

#include "analog.h"
#include "Hardware.h"
#include "DebugKey.h"
#include "Delay.h"
#include "lcd.h"
#include "Log.h"
#include "Motor.h"
#include "Str.h"
#include "UART.h"
#include "Expansion.h"
#include "Encoder.h"
#include "Switch.h"
#include "Menu.h"
#include "DumpMem.h"
#include "param.h"

uint8_t gADC[ 8 ];
uint8_t gLineADC[ 8 ];

uint8_t gLineMap[ 5 ] = { 4, 1, 2, 3, 5 };

#define LED_MASK_0  EXP_OUT_LED1
#define LED_MASK_1  EXP_OUT_LED4
#define LED_MASK_2  EXP_OUT_LED5
#define LED_MASK_3  EXP_OUT_LED6
#define LED_MASK_4  EXP_OUT_LED3
#define LED_MASK_X  EXP_OUT_LED2

uint8_t     gLowMask;
uint8_t     gHighMask;

uint16_t    gLedMask[ 5 ] = 
{
    LED_MASK_0,
    LED_MASK_1,
    LED_MASK_2,
    LED_MASK_3,
    LED_MASK_4,
};

int16_t gPrevError;
int16_t gIntegral;

volatile    uint8_t gFollow;

/***************************************************************************/
/**
*   Core routine for calibration
*/

static void CalibrateCore( MENU_Event_t event, uint8_t *cal )
{
    if ( event == MENU_EVENT_SELECT )
    {
        memcpy( &cal[ 1 ], &gADC[ 1 ], 5 );

        return;
    }

    if ( event == MENU_EVENT_INIT )
    {
        LCD_Clear();
    }

    LCD_MoveTo( 0, 0 );
    LCD_Printf( "C %02x %02x %02x %02x %02x",  cal[ 4 ],  cal[ 1 ],  cal[ 2 ],  cal[ 3 ],  cal[ 5 ]);

    LCD_MoveTo( 0, 1 );
    LCD_Printf( "L %02x %02x %02x %02x %02x", gADC[ 4 ], gADC[ 1 ], gADC[ 2 ], gADC[ 3 ], gADC[ 5 ]);

} // CalibrateCore

/***************************************************************************/
/**
*   Called to save the white calibration
*/

static const prog_char *CalibrateSaveWhite( MENU_Event_t event )
{
    CalibrateCore( event, gMemParam.cal_white );

    if ( event == MENU_EVENT_SELECT )
    {
        return PSTR( "White Cal\nsaved" );
    }

    return NULL;

} // CalibrateSaveWhite

static const prog_char *Follow( MENU_Event_t event );

/***************************************************************************/
/**
*   Called to save the black calibration
*/

static const prog_char *CalibrateSaveBlack( MENU_Event_t event )
{
    CalibrateCore( event, gMemParam.cal_black );

    if ( event == MENU_EVENT_SELECT )
    {
        return PSTR( "Black Cal\nsaved" );
    }

    return NULL;

} // CalibrateSaveBlack

/***************************************************************************/
/**
*   The menu
*/

uint8_t     foo = 1;
uint16_t    bar = 2;

static  const char MainMenuStr[]    PROGMEM = "== Main Menu ==";

static  const char SaveWhiteStr[]   PROGMEM = "Save White";
static  const char SaveBlackStr[]   PROGMEM = "Save Black";
static  const char ThreshHighStr[]  PROGMEM = "Thresh High";
static  const char ThreshLowStr[]   PROGMEM = "Thresh Low";
static  const char KpStr[]          PROGMEM = "Kp";
static  const char KdStr[]          PROGMEM = "Kd";
static  const char KiStr[]          PROGMEM = "Ki";
static  const char KoStr[]          PROGMEM = "Ko";


static  const char CalibrateStr[]   PROGMEM = "Calibrate";
static  const char FollowStr[]      PROGMEM = "Follow";
static  const char fooStr[]         PROGMEM = "foo";
static  const char barStr[]         PROGMEM = "bar";

static MENU_ProgItem_t gCalibrateMenu[] PROGMEM =
{
    MENU_ITEM_PROC( SaveWhiteStr,   CalibrateSaveWhite ),
    MENU_ITEM_PROC( SaveBlackStr,   CalibrateSaveBlack ),
    MENU_ITEM_BYTE( ThreshHighStr,  &gMemParam.thresh_hi, 0, 254 ),
    MENU_ITEM_BYTE( ThreshLowStr,   &gMemParam.thresh_lo, 0, 254 ),
    MENU_ITEM_INT(  KpStr,          &gMemParam.Kp, -100, 100 ),
    MENU_ITEM_INT(  KdStr,          &gMemParam.Kd, -100, 100 ),
    MENU_ITEM_INT(  KiStr,          &gMemParam.Ki, -100, 100 ),
    MENU_ITEM_INT(  KoStr,          &gMemParam.Ko, -100, 100 ),
    MENU_END
};

static MENU_ProgItem_t gMainMenu[] PROGMEM =
{
    MENU_ITEM_MENU( CalibrateStr,   gCalibrateMenu ),
    MENU_ITEM_PROC( FollowStr,      Follow ),
    MENU_ITEM_BYTE( fooStr,         &foo, 0, 10 ),
    MENU_ITEM_UINT( barStr,         &bar, 1, 20 ),
    MENU_END
};

static MENU_ProgItem_t  gTopMenu[] PROGMEM =
{
    MENU_ITEM_MENU( MainMenuStr,    gMainMenu ),
    MENU_END
};

/***************************************************************************/
/**
*   Called when one of the switches is pressed.
*/

void SwitchTurnedOn( uint8_t switchNum )
{
    switch ( switchNum )
    {
        case SWITCH_JOY_DOWN:   MENU_Event( MENU_EVENT_DOWN );      break;
        case SWITCH_JOY_UP:     MENU_Event( MENU_EVENT_UP );        break;
        case SWITCH_JOY_SELECT: MENU_Event( MENU_EVENT_SELECT );    break;
        case SWITCH_JOY_RIGHT:  MENU_Event( MENU_EVENT_RIGHT);      break;
        case SWITCH_JOY_LEFT:   MENU_Event( MENU_EVENT_LEFT );      break;
    }
}

/***************************************************************************/
/**
*   Called when one of the switches is released.
*/

void SwitchTurnedOff( uint8_t switchNum )
{

}

/***************************************************************************/
/**
*   GetLineError
*/

int8_t GetLineError( void )
{
    uint16_t    ledMask = 0;
    int8_t      posn = 0;
    uint8_t     i;

    for ( i = 0; i < 8; i++ ) 
    {
        gADC[ i ] = GetADC( i );
    }

    // We have 5 sensors, which we'll assume even values, and the between
    // the sensors will be assigned odd values.
    //
    //    -4    -2    0    2    4
    // -5    -3    -1    1    3    5

    // Normalize the raw sensor values such that the low calibration value
    // gets assigned 0x00 and the high calibration value gets assigned 0xFF
    // and the other values are scaled proportionally.

    for ( i = 0; i < 5; i++ ) 
    {
        uint8_t     lineMap = gLineMap[ i ];
        uint16_t    lineADC = gADC[ lineMap ];
        uint16_t    calWhite = gMemParam.cal_white[ lineMap ];
        uint16_t    calBlack = gMemParam.cal_black[ lineMap ];

        if ( lineADC < calWhite )
        {
            lineADC = calWhite;
        }
        else
        if ( lineADC > calBlack )
        {
            lineADC = calBlack;
        }

        lineADC -= calWhite;
        lineADC *= 256;
        lineADC /= ( calBlack - calWhite );

        if ( lineADC > 0xFF )
        {
            lineADC = 0xFF;
        }
        gLineADC[ i ] = lineADC;
    }

    gLowMask = 0;
    gHighMask = 0;

    for ( i = 0; i < 5; i++ ) 
    {
        if ( gLineADC[ i ] > gMemParam.thresh_lo )
        {
            gLowMask |= ( 1 << i );
        }
        if ( gLineADC[ i ] > gMemParam.thresh_hi )
        {
            gHighMask |= ( 1 << i );
            ledMask   |= gLedMask[ i ];
        }
    }

    if (( gHighMask == 0 ) || (( gHighMask & ( gHighMask - 1 )) == 0 ))
    {
        // Exactly 0 or 1 bits are set in gHighMask. Check to see if two
        // bits are on in the low mask

        switch ( gLowMask )
        {
            case 0x03:
            {
                posn = -3;
                ledMask = LED_MASK_0 | LED_MASK_1;
                break;
            }

            case 0x06:
            {
                posn = -1;
                ledMask = LED_MASK_1 | LED_MASK_2;
                break;
            }

            case 0x0C:
            {
                posn = 1;
                ledMask = LED_MASK_2 | LED_MASK_3;
                break;
            }

            case 0x18:
            {
                posn = 3;
                ledMask = LED_MASK_3 | LED_MASK_4;
                break;
            }
        }

        if ( gHighMask == 0 )
        {
            // Check for the case that the line is just outside the outside
            // sensors

            if ( gLowMask == 0x01 )
            {
                posn = -4;
                ledMask = LED_MASK_0;
            }
            else
            if ( gLowMask == 0x10 )
            {
                posn = 4;
                ledMask = LED_MASK_4;
            }
        }

        if ( posn == 0 )
        {
            switch ( gHighMask )
            {
                case 0x01:  posn = -4;  break;
                case 0x02:  posn = -2;  break;
                case 0x04:  posn =  0;  break;
                case 0x08:  posn =  2;  break;
                case 0x10:  posn =  4;  break;
            }
        }
    }

    EXP_TransferWord( ~ledMask, EXP_OUT_LED_MASK );

    return posn;

} // GetLineError

/***************************************************************************/
/**
*   Main Loop
*/

int main( void )
{
    int         i;
    int         led = 0;
    uint16_t    prevSwitches = 0;

    InitHardware();
    InitADC();
    InitMotors();

    eeprom_read_block( &gMemParam, &gEEParam, sizeof( gMemParam ));

    if ( gMemParam.thresh_hi == 0xFF )
    {
        gMemParam.thresh_hi = 0x80;
    }
    if ( gMemParam.thresh_lo == 0xFF )
    {
        gMemParam.thresh_lo = 0x10;
    }

#if CFG_LOG_USE_STDIO
    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

    LogInit( stdout );
#endif

    // The first handle opened for read goes to stdin, and the first handle
    // opened for write goes to stdout. So u0 is stdin, stdout, and stderr

    Log( "*****\n" );
    Log( "***** Line Maze program\n" );
    Log( "*****\n" );

    LCD_Init( 2, 16 );
    LCD_Printf( " SRS Sample Bot " );
    LCD_MoveTo( 0, 1 );
    LCD_Printf( "Second line" );

    Log( "\n" );

    MENU_Init( gTopMenu );

    LED_OFF( GREEN );

    while( 1 )
    {
        uint16_t    switches;
        uint8_t     pinc;
        uint8_t     pind;
        int8_t      error;

        LED_TOGGLE( GREEN );

        led++;
        if ( led >= 6 )
        {
            led = 0;
        }

        error = GetLineError();
        
        if ( MENU_IsActive() )
        {
            MENU_Event( MENU_EVENT_TIMER );
        }
        else
        {
            if ( MENU_IsModified() )
            {
                eeprom_write_block( &gMemParam, &gEEParam, sizeof( gMemParam ));
                MENU_ClearModified();

                LCD_Clear();
                LCD_Printf( "EEPROM Updated\n" );
                Log( "EEPROM Updated\n" );
                ms_spin( 1000 );
            }

            //switches = EXP_TransferWord( ~( 1 << ( led + 2 )), EXP_OUT_LED_MASK );
            switches = EXP_TransferWord( 0, 0 );
            Log( "SW:%04x ", switches ); 

            if ( switches != prevSwitches )
            {
                LCD_Clear();
                prevSwitches = switches;
            }
            for ( i = 0; i < 8; i++ ) 
            {
                Log( "%02x ", gLineADC[ i ]);
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
                    LCD_Printf( "L %02x %02x %02x %02x %02x", 
                                gLineADC[ 0 ], 
                                gLineADC[ 1 ], 
                                gLineADC[ 2 ], 
                                gLineADC[ 3 ], 
                                gLineADC[ 4 ] );
                                           
                    LCD_MoveTo( 0, 1 );
                    LCD_Printf( "E %02x %02x %3d B %02x", gADC[ 0 ], gADC[ 7 ], error, gADC[ 6 ] );
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

            Log( " QL:%d%d QR:%d%d EC-L:%d EC-R:%d Err:%2d L:%5b H:%5b\n", 
                 ( ENCODER_L_A_PIN & ENCODER_L_A_MASK ) != 0,
                 ( ENCODER_L_B_PIN & ENCODER_L_B_MASK ) != 0,
                 ( ENCODER_R_A_PIN & ENCODER_R_A_MASK ) != 0,
                 ( ENCODER_R_B_PIN & ENCODER_R_B_MASK ) != 0,
                 gEncoderCountL,
                 gEncoderCountR,
                 error, gLowMask, gHighMask );
        }

        // Tick rate is 100/sec so waiting for 50 waits for 1/2 sec

        for ( i = 0; i < 50; i++ ) 
        {
            WaitForTimer0Rollover();
            CheckSwitches();

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

/***************************************************************************/
/**
*   Test mode to follow a line.
*/

static const prog_char *Follow( MENU_Event_t event )
{
    MENU_Exit();

    Log( "Starting Follow...\n" );

    TIMSK |= ( 1 << TOIE1 );

    gFollow = 1;

    while ( 1 )
    {
        ;
    }

    return NULL;

} // Follow

/***************************************************************************/
/**
*   Timer 1 Overflow. We use this to drive the PID loop.
*/

ISR( TIMER1_OVF_vect )
{
    static  uint8_t counter = 0;

    if ( gFollow )
    {
        if ( ++counter >= 128 )
        {
            counter = 0;
            LED_TOGGLE( GREEN );
            Log( "*" );
        }
    }

#if 0
    if ( gFollow )
    {
        int16_t     error;

        error = GetLineError();
    }
#endif
}

