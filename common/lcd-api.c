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
/**
*
*   @file   lcd.c
*
*   @brief  Provides an API for talking to character style LCDs. These
*           functions are designed around the HD44780 based LCD Controllers.
*
****************************************************************************/
/**
*
*   This particular file doesn't know how to interface with the hardware.
*
*   It expects the following routines to be defined:
*
*       LCD_HAL_Write           - Writes a command or data byte to the hardware
*       LCD_HAL_Delay           - Delays for a specified number of milliseconds
*
*   Currently, we don't support reading of the LCD or busy bit, instead
*   just waiting for the maximum amount of time for each command.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdarg.h>
#include <stdio.h>

#include "lcd.h"
#include "lcd-hal.h"

#if defined( AVR )
#else
#   define pgm_read_byte( addr )  *addr
#endif

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

static  uint8_t gNumLines;
static  uint8_t gNumCols;
static  uint8_t gX;
static  uint8_t gY;


/* ---- Private Function Prototypes --------------------------------------- */

//***************************************************************************
/**
*   LCD_WriteCommand
*
*   Writes a "command" to the LCD.
*/

static inline void LCD_WriteCommand( uint8_t cmd )
{
    LCD_HAL_Write( LCD_RS_CMD, cmd );

    if ( cmd <= 3 )
    {
        // The Home and Clear commands require a worst case delay of 4.1 msec

        LCD_HAL_Delay( 5 );
    }
    else
    {
        // All of the rest of the commands require 160 usecs

        LCD_HAL_Delay( 1 );
    }

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
    LCD_WriteCommand( LCD_HOME );

    gX = 0;
    gY = 0;

} // LCD_Clear

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

void LCD_Init( uint8_t numLines, uint8_t numCols )
{
    if ( numLines > 4 )
    {
        numLines = 4;
    }
    gNumLines = numLines;

    if ( numCols > 40 )
    {
        numCols = 40;
    }
    gNumCols = numCols;

    // Let the low level HAL do most of the initialization

    LCD_HAL_Init();

    LCD_DisplayOff();

    LCD_Clear();

    LCD_WriteCommand( LCD_ENTRY_MODE | LCD_ENTRY_INC );

    LCD_HideCursor();   // also turns on display

} // LCD_Init

//***************************************************************************
/**
*   LCD_MoveTo
*
*   Moves the cursor position to the indicated position. x=0 is the first
*   column, y=0 is the first row.
*/

void LCD_MoveTo( unsigned x, unsigned y )
{
    uint8_t addr = x & 0x3f;

    if ( y & 1 )
    {
        addr += 0x40;   // Lines 1 & 3 add 0x40
    }
    if ( y & 2 )
    {
        addr += 0x14;   // Lines 2 & 3 add 0x14
    }

    LCD_WriteCommand( LCD_DDRAM | addr );

    gX = x;
    gY = y;

} // LCD_MoveTo

//***************************************************************************
/**
*   LCD_NumCols
*
*   Returns the number of columns passed into LCD_Init.
*/

uint8_t LCD_NumCols( void )
{
    return gNumCols;

} // LCD_NumCols

//***************************************************************************
/**
*   LCD_NumLines
*
*   Returns the number of rows passed into LCD_Init.
*/

uint8_t LCD_NumLines( void )
{
    return gNumLines;

} // LCD_NumLines

//***************************************************************************
/**
*   LCD_PutChar
*
*   Writes the inidcated character to the LCD at the current cursor position
*   and advances the cursor by one position.
*/

void LCD_PutChar( char ch )
{
    if (( gX >= gNumCols ) || ( ch == '\n' ))
    {
        gX = 0;
        gY++;

        if ( gY >= gNumLines )
        {
            gY = 0;
        }

        LCD_MoveTo( gX, gY );
    }
    if ( ch != '\n' )
    {
        LCD_HAL_Write( LCD_RS_DATA, ch );
        LCD_HAL_Delay( 1 );

        gX++;
    }

} // LCD_PutChar

//***************************************************************************
/**
*   LCD_PutStr
*
*   Writes the indicated string to the LCD at the current cursor position
*   and advances the cursor by the length of the string.
*/

void LCD_PutStr_P( const prog_char *s )
{
    char    ch;

    while (( ch = pgm_read_byte( s )) != '\0' )
    {
        LCD_PutChar( ch );

        s++;
    }

} // LCD_PutStr

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

