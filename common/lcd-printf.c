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
*   @file   lcd-printf.c
*
*   @brief  Provides the printf function interface. This is provided in a
*           separate file so that the user can choose not to use it,
*           or perhaps replace it with a different implementation.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdarg.h>

#include "lcd.h"
#include "Str.h"

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */

//***************************************************************************
/**
*   The lcdPrintFunc is called from the the vStrXPrintf function to print
*   a single character.
*/
static int lcdPrintfFunc( void *outParm, int ch )
{
    LCD_PutChar( ch );

    return 1;

} // lcdPrintfFunc

#if defined( AVR )

//***************************************************************************
/**
*   LCD_Printf_P
*
*   Writes the formatted sting to the LCD at the current cursor position.
*   The _P version indicates that the format string comes from program 
*   memory.
*
*   Use this function one of the following two ways:
*
*   LCD_Printf( "i = %d\n", i );
*
*   LCD_Printf_P( PSTR( "i = %d\n" ), i );
*/

void LCD_Printf_P( const prog_char *fmt, ... )
{
    va_list args;

    va_start( args, fmt );

    vStrXPrintf_P( lcdPrintfFunc, 0, fmt, args );

    va_end( args );

} // LCD_Printf_P

#else

//***************************************************************************
/**
*   LCD_Printf
*
*   Writes the formatted sting to the LCD at the current cursor position.
*/

void LCD_Printf( const char *fmt, ... )
{
    va_list args;

    va_start( args, fmt );

    vStrXPrintf( lcdPrintfFunc, 0, fmt, args );

    va_end( args );

} // LCD_Printf

#endif  // AVR

