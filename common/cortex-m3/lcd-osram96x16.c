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
*   @file   lcd-osram96x16.c
*
*   @brief  Implements the lcd.h API uing the OSRAM 96x16 display that comes
*           with the Luminary L3S811 eval board.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdarg.h>
#include <stdio.h>

#include "lcd.h"

#include "hw_types.h"
#include "osram96x16.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

static  uint8_t gNumLines;
static  uint8_t gNumCols;
static  uint8_t gX;
static  uint8_t gY;


/* ---- Private Function Prototypes --------------------------------------- */

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
    (void)onOff;

} // LCD_BlinkCursor

//***************************************************************************
/**
*   LCD_Clear
*
*   Clears the LCD display and moves the cursor to the top left corner
*/

void LCD_Clear( void )
{
    OSRAMClear();

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
    OSRAMDisplayOff();

} // LCD_DisplayOff

//***************************************************************************
/**
*   LCD_Display
*
*   Turns on (i.e. unblanks) the LCD.
*/

void LCD_DisplayOn( void )
{
    OSRAMDisplayOn();

} // LCD_DisplayOn

//***************************************************************************
/**
*   LCD_HideCursor
*
*   Causes the cursor to be hidden.
*/

void LCD_HideCursor( void )
{

} // LCD_HideCursor

//***************************************************************************
/**
*   LCD_Init
*
*   Initializes the LCD for use.
*/

void LCD_Init( uint8_t numLines, uint8_t numCols )
{
    OSRAMInit( false );

    gNumLines = numLines;
    gNumCols  = numCols;

    LCD_Clear();
    LCD_DisplayOn();

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
    char    chStr[ 2 ];

    if (( gX >= gNumCols ) || ( ch == '\n' ))
    {
        gX = 0;
        gY++;

        if ( gY >= gNumLines )
        {
            gY = 0;
        }
    }
    if ( ch != '\n' )
    {
        chStr[ 0 ] = ch;
        chStr[ 1 ] = 0;

        OSRAMStringDraw( chStr, gX * 6, gY );

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

void LCD_PutStr( const char *s )
{
    char    ch;

    while (( ch = *s++ ) != '\0' )
    {
        LCD_PutChar( ch );
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

} // LCD_ShowCursor

