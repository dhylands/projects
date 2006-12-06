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
*   @file   lcd.h
*
*   @brief  Provides an API for talking to character style LCDs. These
*           functions are designed around the HD44780 based LCD Controllers.
*
****************************************************************************/

#if !defined( LCD_H )
#define LCD_H           /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>

#if defined( AVR )
#   include <stdio.h>
#   include <avr/pgmspace.h>
#else
#   define  PSTR(str)   str
#endif

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void LCD_Init( uint8_t numLines, uint8_t numColumns );

void LCD_Clear( void );

void LCD_ShowCursor( void );
void LCD_HideCursor( void );

void LCD_BlinkCursor( int onOff );

void LCD_DisplayOff( void );
void LCD_DisplayOn( void );

void LCD_MoveTo( unsigned x, unsigned y );
void LCD_PutChar( char ch );

uint8_t LCD_NumCols( void );
uint8_t LCD_NumLines( void );

#if defined( AVR )

int  LCD_PutCharStdio( char ch, FILE *fs );

void LCD_PutStr_P( const prog_char *s );
void LCD_Printf_P( const prog_char *fmt, ... );

#define LCD_PutStr( str )           LCD_PutStr_P( PSTR( str ))
#define LCD_Printf( fmt, args... )  LCD_Printf_P( PSTR( fmt ), ## args )

#else

void LCD_PutStr( const char *s );
void LCD_Printf( const char *fmt, ... );

#define LCD_PutStr_P( str )             LCD_PutStr( str )
#define LCD_Printf_P( fmt, args... )    LCD_Printf( fmt, ## args )

#endif  // AVR

#endif // LCD_H

