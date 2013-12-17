/****************************************************************************
*
*   Copyright (c) 2009 Dave Hylands     <dhylands@gmail.com>
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
*   @file   led.h
*
*   @brief  Contains an abstraction for turning LEDs on and off.
*
*   This header file takes advantage of the C-preprocessor to
*   perform it's magic.
*
*   For a named LED, the following macro definitions need to exist
*   in your project's Config.h file.
*
*   CFG_LED_name_PORT   - Identifies the port that the LED can be found on
*                         For the AVR, this should be something like PORTC
*
*   CFG_LED_name_BIT    - Identifies the bit number of the LED within the
*                         port. For the AVR, this should be a number between
*                         0 and 7.
*
*   CFG_LED_name_ON     - Determines if the LED is on by setting the
*                         appropriate pin to a 0 or 1.
*
*   One the above defines have been made, each LED should be initialized
*   using:
*
*       LED_Init( name );
*
*   You can then turn the LED on by using:
*
*       LED_On( name );
*
*   and turn the LED off by using:
*
*       LED_Off( name );
*
*   or toggle the LED by using:
*
*       LED_Toggle( name );
*
****************************************************************************/

#if !defined( LED_H )
#define LED_H         /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include "Config.h"

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

// For all of the AVRs that I've looked at, the address of the DDR
// register for a given port is always 1 less than the address of the
// PORT register.

#define LED_DDR(port)   ( *(&(port) - 1) )
#define LED_MASK(bit)   ( 1 << (bit))

#define LED_Init( name ) \
    do \
    { \
        LED_DDR( CFG_LED_ ## name ## _PORT ) |= LED_MASK( CFG_LED_ ## name ## _BIT ); \
    } while (0)

// If CFG_LED_name_ON == 1 and value == 1, then we want to set the bit
// If CFG_LED_name_ON == 1 and value == 0, then we want to clear the bit
// If CFG_LED_name_ON == 0 and value == 1, then we want to clear the bit
// If CFG_LED_name_ON == 0 and value == 0, then we want to set the bit

#define LED_Set( name, value ) \
    do \
    { \
        if ( CFG_LED_ ## name ## _ON == (value) ) \
        { \
            CFG_LED_ ## name ## _PORT |= LED_MASK( CFG_LED_ ## name ## _BIT ); \
        } \
        else \
        { \
            CFG_LED_ ## name ## _PORT &= ~LED_MASK( CFG_LED_ ## name ## _BIT ); \
        } \
    } while ( 0 )

#define LED_On( name )  LED_Set( name, 1 )
#define LED_Off( name ) LED_Set( name, 0 )

#define LED_Toggle( name ) \
    do \
    { \
        CFG_LED_ ## name ## _PORT ^= LED_MASK( CFG_LED_ ## name ## _BIT ); \
    } while ( 0 )

/** @} */

#endif /* LED_H */

