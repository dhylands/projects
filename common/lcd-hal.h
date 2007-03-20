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
*   @file   lcd-hal.h
*
*   @brief  Describes the API for talking to the LCD HAL layer.
*
****************************************************************************/

#if !defined( LCD_HAL_H )
#define LCD_HAL_H               /**< Include Guard                         */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>

/* ---- Constants and Types ---------------------------------------------- */

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

#define LCD_RS_CMD          0
#define LCD_RS_DATA         1

// reading:

#define LCD_BUSY            0x80    // DB7: LCD is busy

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void LCD_HAL_Init( void );

void LCD_HAL_Delay( uint8_t msec );
void LCD_HAL_Write( uint8_t rs, uint8_t cmd );

#endif // LCD_HAL_H


