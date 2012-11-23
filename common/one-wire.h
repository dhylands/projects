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
*   @file   one-wire.h
*
*   @brief  Contains definitions for interfacing with a 1-wire bus.
*
****************************************************************************/

#if !defined( ONE_WIRE_H )
#define ONE_WIRE_H         /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include <stddef.h>
#include <stdint.h>

#if defined( AVR )
#   include <one-wire-hal-avr.h>
#else
#   error Unsupported CPU architecture
#endif

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void    OW_Init( void );
int     OW_Reset( void );
void    OW_WriteBit( uint8_t bit );
void    OW_WriteByte( uint8_t byte );
uint8_t OW_ReadBit( void );
uint8_t OW_ReadByte( void );
uint8_t OW_TouchByte( uint8_t byte );
void    OW_TouchBlock( void *data, size_t numBytes );


/** @} */

#endif /* ONE_WIRE_H */

