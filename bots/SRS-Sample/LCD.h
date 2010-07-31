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
*   @file   LCD.h
*
*   @brief  Describes the interface to the LCD.
*
****************************************************************************/

#if !defined( LCD_H )
#define LCD_H           /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <inttypes.h>

#include <avr/pgmspace.h>

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void LCD_Init( void );

void LCD_Clear( void );

void LCD_ShowCursor( void );
void LCD_HideCursor( void );

void LCD_BlinkCursor( int onOff );

void LCD_DisplayOff( void );
void LCD_DisplayOn( void );

void LCD_MoveTo( unsigned x, unsigned y );
void LCD_PutChar( char ch );
void LCD_PutStr( const char *s );

void LCD_Printf_P( const prog_char *fmt, ... );

#define LCD_Printf( fmt, args... )  LCD_Printf_P( PSTR( fmt ), ## args )

void LCD_Test( void );

#endif // LCD_H

