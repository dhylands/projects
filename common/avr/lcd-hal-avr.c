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
*   @file   lcd-hal-avr.c
*
*   @brief  Provides an implementation for using the HD44780 LCD on an
*           AVR.
*
*   This particular implementation needs 6 I/O pins to connect to
*   the LCD. 4 data lines, a register select line, and an Enable line.
*
*   The R/W line on the LCD can be tied to ground.
*
*   The 4 data lines should be contiguous and wired to D4, D5, D6, and
*   D7 on the LCD, with the least significant bit on the AVR side 
*   going to D4 on the LCD.
*
*   See: http://homepage.hispeed.ch/peterfleury/avr-lcd44780.html
*   for further details.
*
*   This implementation allows for the following configuration parameters,
*   with the default shown.
*
*   CFG_LCD_DATA_PORT   PORTC   - Specifies which port the data pins are on
*   CFG_LCD_DATA_SHIFT  0       - Identifies the LSB of the 4 data bits
*   CFG_LCD_E_PORT      PORTC   - Identifies the port used for the enable line
*   CFG_LCD_E_PIN       6       - Identifies the pin used for the enable line
*   CFG_LCD_RS_PORT     PORTC   - Identifies the port used for the register select
*   CFG_LCD_RS_PIN      4       - Identifies the pin used for the register select
*   CFG_LCD_RW_PORT             - Identifies the port used for the R/W line
*   CFG_LCD_RW_PIN              - Identifies the pin used for the R/W line
*
*   Note: The RW PORT and PIN definitions are optional. If present, that pin
*         will be set to zero and not changed.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdarg.h>

#include "Config.h"

#include "lcd.h"
#include "lcd-hal.h"
#include "Delay.h"

#if !defined( CFG_LCD_SCATTERED_DATA )
#define CFG_LCD_SCATTERED_DATA  0
#endif

#if ( CFG_LCD_SCATTERED_DATA == 0 )
#   if !defined( CFG_LCD_DATA_PORT )
#       define  CFG_LCD_DATA_PORT   PORTC
#   endif
#   if !defined( CFG_LCD_DATA_SHIFT )
#       define  CFG_LCD_DATA_SHIFT  0
#   endif
#   define LCD_DATA_MASK   ( 0x0F << CFG_LCD_DATA_SHIFT )
#endif

#if !defined( CFG_LCD_E_PORT )
#   define  CFG_LCD_E_PORT      PORTC
#endif
#if !defined( CFG_LCD_E_PIN )
#   define  CFG_LCD_E_PIN       6
#endif
#if !defined( CFG_LCD_RS_PORT )
#   define  CFG_LCD_RS_PORT     PORTC
#endif
#if !defined( CFG_LCD_RS_PIN )
#   define  CFG_LCD_RS_PIN      4
#endif

#define LCD_E_MASK      ( 1    << CFG_LCD_E_PIN )
#define LCD_RS_MASK     ( 1    << CFG_LCD_RS_PIN )
#define LCD_RW_MASK     ( 1    << CFG_LCD_RW_PIN )

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

#define DDR( port ) (*(&(port) - 1))

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

static inline void LCD_HAL_E_High( void )
{
    CFG_LCD_E_PORT |= LCD_E_MASK;
}

static inline void LCD_HAL_E_Low( void )
{
    CFG_LCD_E_PORT &= ~LCD_E_MASK;
}

static inline void LCD_HAL_E_Pulse( void )
{
    // E needs to be high for 230 nsec. If we assume a 16 MHz clock, then
    // that would be 4 NOP's (1 NOP = 62.5 msec)

    LCD_HAL_E_High();
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    asm( " nop" );
    LCD_HAL_E_Low();
}

static inline void LCD_HAL_RS_High( void )
{
    CFG_LCD_RS_PORT |= LCD_RS_MASK;
}

static inline void LCD_HAL_RS_Low( void )
{
    CFG_LCD_RS_PORT &= ~LCD_RS_MASK;
}

static inline void LCD_HAL_RW_Low( void )
{
#if defined( CFG_LCD_RW_PORT )
    CFG_LCD_RW_PORT &= ~LCD_RW_MASK;
#endif
}

static inline void LCD_HAL_Data( uint8_t data )
{
#if CFG_LCD_SCATTERED_DATA

    if ( data & 1 )
        CFG_LCD_DATA_DB4_PORT |=  ( 1 << CFG_LCD_DATA_DB4_PIN );
    else
        CFG_LCD_DATA_DB4_PORT &= ~( 1 << CFG_LCD_DATA_DB4_PIN );

    if ( data & 2 )
        CFG_LCD_DATA_DB5_PORT |=  ( 1 << CFG_LCD_DATA_DB5_PIN );
    else
        CFG_LCD_DATA_DB5_PORT &= ~( 1 << CFG_LCD_DATA_DB5_PIN );

    if ( data & 4 )
        CFG_LCD_DATA_DB6_PORT |=  ( 1 << CFG_LCD_DATA_DB6_PIN );
    else
        CFG_LCD_DATA_DB6_PORT &= ~( 1 << CFG_LCD_DATA_DB6_PIN );

    if ( data & 8 )
        CFG_LCD_DATA_DB7_PORT |=  ( 1 << CFG_LCD_DATA_DB7_PIN );
    else
        CFG_LCD_DATA_DB7_PORT &= ~( 1 << CFG_LCD_DATA_DB7_PIN );

#else
    CFG_LCD_DATA_PORT &= ~LCD_DATA_MASK;
    CFG_LCD_DATA_PORT |= (( data & 0x0F ) << CFG_LCD_DATA_SHIFT );
#endif
}

//***************************************************************************
/**
*   LCD_HAL_Delay
*
*   Delays for the specified number of milliseconds
*/

void LCD_HAL_Delay( uint8_t msec )
{
    ms_spin( msec );

} // LCD_HAL_Delay

//***************************************************************************
/**
*   LCD_HAL_Init
*
*   Initializes the AVR for connecting to an LCD.
*
*   We currently assume 4-bit mode.
*/

void LCD_HAL_Init( void )
{
    // Configure the pins as outputs

#if CFG_LCD_SCATTERED_DATA
    DDR( CFG_LCD_DATA_DB4_PORT )    |= ( 1 << CFG_LCD_DATA_DB4_PIN );
    DDR( CFG_LCD_DATA_DB5_PORT )    |= ( 1 << CFG_LCD_DATA_DB5_PIN );
    DDR( CFG_LCD_DATA_DB6_PORT )    |= ( 1 << CFG_LCD_DATA_DB6_PIN );
    DDR( CFG_LCD_DATA_DB7_PORT )    |= ( 1 << CFG_LCD_DATA_DB7_PIN );
#else
    DDR( CFG_LCD_DATA_PORT ) |= LCD_DATA_MASK;
#endif
    DDR( CFG_LCD_RS_PORT )   |= LCD_RS_MASK;
    DDR( CFG_LCD_E_PORT )    |= LCD_E_MASK;
#if defined( CFG_LCD_RW_PORT )
    DDR( CFG_LCD_RW_PORT )   |= LCD_RW_MASK;
#endif

    LCD_HAL_RW_Low();
    LCD_HAL_E_Low();
    LCD_HAL_RS_Low();

    // Do a soft reset

    LCD_HAL_Delay( 20 );    // Allow LCD to powerup

    LCD_HAL_Data( LCD_FUNCTION_RESET >> 4 );
    LCD_HAL_E_Pulse();
    LCD_HAL_Delay( 5 ); // Need to delay for at least 4.1 msec
    LCD_HAL_E_Pulse();  // Sends Reset a 2nd time
    LCD_HAL_Delay( 1 );
    LCD_HAL_E_Pulse();  // and a 3rd time.
    LCD_HAL_Delay( 1 );

    // Put the LCD into 4-bit mode

    LCD_HAL_Data( LCD_FUNCTION >> 4 );
    LCD_HAL_E_Pulse();
    LCD_HAL_Delay( 1 );

    // Now that the LCD is in 4 bit mode, we can call LCD_HAL_Write

    if ( LCD_NumLines() == 1 )
    {
        LCD_HAL_Write( LCD_RS_CMD, LCD_FUNCTION );
    }
    else
    {
        LCD_HAL_Write( LCD_RS_CMD, LCD_FUNCTION | LCD_FUNCTION_2LINES );
    }
    

} // LCD_HAL_Init

//***************************************************************************
/**
*   LCD_HAL_Write
*
*   Writes a byte (command or data) to the LCD.
*/

void LCD_HAL_Write( uint8_t rs, uint8_t cmd )
{
    if ( rs )
    {
        LCD_HAL_RS_High();
    }
    else
    {
        LCD_HAL_RS_Low();
    }

    // Output high nibble

    LCD_HAL_Data( cmd >> 4 );
    LCD_HAL_E_Pulse();

    // Output low nibble

    LCD_HAL_Data( cmd & 0x0F );
    LCD_HAL_E_Pulse();

} // LCD_HAL_Write

