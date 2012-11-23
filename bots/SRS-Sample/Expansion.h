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
*   @file   Expansion.h
*
*   @brief  Contains definitions for the Expansion Board
*
****************************************************************************/

#if !defined( EXPANSION_H )
#define EXPANSION_H           /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <inttypes.h>

/* ---- Constants and Types ---------------------------------------------- */

// We treat the expansion board as a 16 bit output and 16 bit input register.
// Each time we want to read or write anything, we transfer all 16 bits
// in both directions.

#define EXP_OUT_LCD_RS      0x0001  // RS - Register Select
#define EXP_OUT_LCD_WR      0x0002  // WR - Active low write

#define EXP_OUT_LED_MASK    0x00fc

#define EXP_OUT_LED6        0x0004
#define EXP_OUT_LED3        0x0008
#define EXP_OUT_LED5        0x0010
#define EXP_OUT_LED2        0x0020
#define EXP_OUT_LED4        0x0040
#define EXP_OUT_LED1        0x0080
#define EXP_OUT_LCD_DATA    0xff00

#define EXP_OUT_LCD_MASK    ( EXP_OUT_LCD_DATA | EXP_OUT_LCD_RS | EXP_OUT_LCD_WR )

#define EXP_IN_RESERVED     0x000f
#define EXP_IN_S4_1         0x0010
#define EXP_IN_S4_2         0x0020
#define EXP_IN_S4_3         0x0040
#define EXP_IN_S4_4         0x0080
#define EXP_IN_S2           0x0100
#define EXP_IN_S1           0x0200
#define EXP_IN_JOY_DOWN     0x0400
#define EXP_IN_JOY_UP       0x0800
#define EXP_IN_JOY_SELECT   0x1000
#define EXP_IN_JOY_RIGHT    0x2000
#define EXP_IN_JOY_LEFT     0x4000
#define EXP_IN_LCD7         0x8000

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

uint16_t EXP_TransferWord( uint16_t outVal, uint16_t outMask );

#endif // EXPANSION_H

