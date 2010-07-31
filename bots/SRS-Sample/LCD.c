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
/**
*
*   @file    LCD.c
*
*   @brief   Implementation of an HD44780 LCD Controller
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdarg.h>

#include <avr/io.h>

#include "LCD.h"
#include "Log.h"
#include "Delay.h"
#include "Expansion.h"
#include "Str.h"

#undef  LCD_Printf
#define LCD_Printf  LCD_Printf_P

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */

#define LCD_ENABLE_PORT     PORTB
#define LCD_ENABLE_PIN      3
#define LCD_ENABLE_MASK     ( 1 << LCD_ENABLE_PIN )

// The following constant names were lifted from the avrlib lcd.h
// header file, however, I changed the definitions from bit numbers to bit masks
//
// HD44780 LCD controller command set
// writing:

#define LCD_CLR             0x01    // DB0: clear display
#define LCD_HOME            0x02    // DB1: return to home position

#define LCD_ENTRY_MODE      0x04    // DB2: set entry mode
#define LCD_ENTRY_INC       0x02    //   DB1: increment
#define LCD_ENTRY_SHIFT     0x01    //   DB2: shift

#define LCD_ON_CTRL         0x08    // DB3: turn lcd/cursor on
#define LCD_ON_DISPLAY      0x04    //   DB2: turn display on
#define LCD_ON_CURSOR       0x02    //   DB1: turn cursor on
#define LCD_ON_BLINK        0x01    //   DB0: blinking cursor

#define LCD_MOVE            0x10    // DB4: move cursor/display
#define LCD_MOVE_DISP       0x08    //   DB3: move display (0-> move cursor)
#define LCD_MOVE_RIGHT      0x04    //   DB2: move right (0-> left)

#define LCD_FUNCTION        0x20    // DB5: function set
#define LCD_FUNCTION_8BIT   0x10    //   DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES 0x08    //   DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS 0x04    //   DB2: 5x10 font (0->5x7 font)
#define LCD_FUNCTION_RESET  0x30    // See "Initializing by Instruction" section

#define LCD_CGRAM           0x40    // DB6: set CG RAM address
#define LCD_DDRAM           0x80    // DB7: set DD RAM address


// reading:
#define LCD_BUSY            7      // DB7: LCD is busy

/* ---- Private Variables ------------------------------------------------- */


/* ---- Private Function Prototypes --------------------------------------- */

void LCD_WriteCore( int rs, uint8_t cmd );

//***************************************************************************
/**
*   LCD_WriteCommand
*
*   Writes a "command" to the LCD.
*/

static inline void LCD_WriteCommand( uint8_t cmd )
{
    LCD_WriteCore( 0, cmd );

} // LCD_WriteCommand

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   LCD_BlinkCursor
*
*   Controls whether the cursor is blinking or not. This function also has 
*   the side effect of causing the cursor to be "shown".
*/

void LCD_BlinkCursor( int onOff )
{
    if ( onOff )
    {
        LCD_WriteCommand( LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR | LCD_ON_BLINK );
    }
    else
    {
        LCD_WriteCommand( LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR );
    }

} // LCD_BlinkCursor

//***************************************************************************
/**
*   LCD_Clear
*
*   Clears the LCD display and moves the cursor to the top left corner
*/

void LCD_Clear( void )
{
    LCD_WriteCommand( LCD_CLR );
    ms_spin( 5 );
    LCD_WriteCommand( LCD_HOME );
    ms_spin( 2 );

} // LCD_Clear

//***************************************************************************
/**
*   LCD_HideCursor
*
*   Causes the cursor to be hidden.
*/

void LCD_HideCursor( void )
{
    LCD_WriteCommand( LCD_ON_CTRL | LCD_ON_DISPLAY );

} // LCD_HideCursor

//***************************************************************************
/**
*   LCD_Init
*
*   Initializes the LCD for use.
*/

void LCD_Init( void )
{
    // Do a soft reset

    ms_spin( 20 );  // Allow time for the LCD to power up

    LCD_WriteCommand( LCD_FUNCTION_RESET );
    ms_spin( 5 );
    LCD_WriteCommand( LCD_FUNCTION_RESET );
    ms_spin( 1 );
    LCD_WriteCommand( LCD_FUNCTION_RESET );
    ms_spin( 1 );

    // Default is 8 bit operation

    LCD_WriteCommand( LCD_FUNCTION | LCD_FUNCTION_8BIT | LCD_FUNCTION_2LINES );
    LCD_DisplayOff();

    LCD_WriteCommand( LCD_ENTRY_MODE | LCD_ENTRY_INC );

    LCD_Clear();
    LCD_HideCursor();   // also turns on display

} // LCD_Init

//***************************************************************************
/**
*   LCD_DisplayOff
*
*   Turns off (i.e. blanks) the LCD.
*/

void LCD_DisplayOff( void )
{
    LCD_WriteCommand( LCD_ON_CTRL );

} // LCD_DisplayOff

//***************************************************************************
/**
*   LCD_Display
*
*   Turns on (i.e. unblanks) the LCD.
*/

void LCD_DisplayOn( void )
{
    LCD_WriteCommand( LCD_ON_CTRL | LCD_ON_DISPLAY );

} // LCD_DisplayOn

//***************************************************************************
/**
*   LCD_MoveTo
*
*   Moves the cursor position to the indicated position. x=0 is the first
*   column, y=0 is the first row.
*/

void LCD_MoveTo( unsigned x, unsigned y )
{
    uint8_t addr = ( x & 0x3f );

    addr |= 0x80;

    if ( y > 0 )
    {
        addr |= 0x40;
    }

    LCD_WriteCommand( addr );

} // LCD_MoveTo

//***************************************************************************
/**
*   LCD_PutChar
*
*   Writes the inidcated character to the LCD at the current cursor position
*   and advances the cursor by one position.
*/

void LCD_PutChar( char ch )
{
    LCD_WriteCore( 1, ch );

} // LCD_PutChar

//***************************************************************************
/**
*   LCD_PutStr
*
*   Writes the indicated string to the LCD at the current cursor position
*   and advances the cursor by the length of the string.
*/

void LCD_PutStr( const char *s )
{
    while ( *s != '\0' )
    {
        LCD_PutChar( *s++ );
    }

} // LCD_PutStr

//***************************************************************************
/**
*   LCD_Printf
*
*   Writes the formatted sting to the LCD at the current cursor position.
*/

static int lcdPrintfFunc( void *outParm, int ch )
{
    LCD_WriteCore( 1, ch );

    return 1;

} // lcdPrintfFunc

void LCD_Printf( const prog_char *fmt, ... )
{
    va_list args;

    va_start( args, fmt );

    Log( "LCD: " );
    vLog_P( fmt, args );

    vStrXPrintf_P( lcdPrintfFunc, 0, fmt, args );

    va_end( args );

} // LCD_Printf

//***************************************************************************
/**
*   LCD_ShowCurosr
*
*   Causes the cursor to be made visible.
*/

void LCD_ShowCursor( void )
{
    LCD_WriteCommand( LCD_ON_CTRL | LCD_ON_DISPLAY | LCD_ON_CURSOR );

} // LCD_ShowCursor

//***************************************************************************
/**
*   LCD_WriteCore
*
*   This is the core routine which writes commands or data to the LCD 
*   controller.
*/

#include "Log.h"
#include "UART.h"

void LCD_WriteCore( int rs, uint8_t cmd )
{
    uint16_t    outWord = 0;

    if ( rs )
    {
        outWord |= EXP_OUT_LCD_RS;
    }
    outWord &= ~EXP_OUT_LCD_WR;

    outWord |= ( (uint16_t)cmd << 8 );

    EXP_TransferWord( outWord, EXP_OUT_LCD_MASK );

    // The data is all setup for the LCD. Make sure it's stable for
    // >= 40 ns before E high

    asm( "nop" );

    LCD_ENABLE_PORT |= LCD_ENABLE_MASK;     // Turn on LCD Enable

    // Meed to wait 230ns before E low. (1 nop = 62.5ns @ 16MHz)

    asm( "nop" );
    asm( "nop" );
    asm( "nop" );
    asm( "nop" );

    LCD_ENABLE_PORT &= ~LCD_ENABLE_MASK;    // Turn off LCD Enable

    ms_spin( 1 );

} // LCD_WriteCore

//***************************************************************************

#if 0

int WaitForChar( void )
{
    while ( !UART0_IsCharAvailable() )
    {
        ;
    }

    return UART0_GetChar();
}

void SendBits( uint16_t bits )
{
    uint16_t    outWord = EXP_gLastXferOut & EXP_OUT_LED_MASK;

    outWord |= bits;

    EXP_TransferWord( outWord );
}

void LCD_Test( void )
{
    int testNum = 0;

    while ( 1 )
    {
        switch ( testNum )
        {
            case 0:
            {
                Log( "E=1" );
                PORTB |= ( 1 << PB3 ); // Turn on LCD Enable
                WaitForChar();
                PORTB &= ~( 1 << PB3 ); // Turn off LCD Enable
                Log( "  E=0\n" );
                break;
            }

            case 1:
            {
                Log( "RS=1" );
                SendBits( EXP_OUT_LCD_RS );
                WaitForChar();
                SendBits( 0 );
                Log( "  RS=0\n" );
                break;
            }

            case 2:
            {
                Log( "WR=1" );
                SendBits( EXP_OUT_LCD_WR );
                WaitForChar();
                SendBits( 0 );
                Log( "  WR=0\n" );
                break;
            }

            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            {
                uint16_t    bitNum = testNum - 3;

                Log( "DB%d=1", bitNum );
                SendBits( 1 << ( bitNum + 8 ));
                WaitForChar();
                SendBits( 0 );
                Log( "  DB%d=0\n", bitNum );
                break;
            }
        }
        testNum++;
        if ( testNum > 10 )
        {
            break;
        }
    }
}

#endif
