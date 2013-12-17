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
*   @file   one-wire-avr.h
*
*   @brief  Contains definitions for using the one-wire protocol on
*           the AVR.
*
****************************************************************************/

#if !defined( ONE_WIRE_AVR_H )
#define ONE_WIRE_AVR_H         /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include "Config.h"

#include <Delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#if defined( __cplusplus )
extern "C"
{
#endif

//---------------------------------------------------------------------------
//
//  To use this header file the following #defines should be made in Config.h
//
//  CFG_ONE_WIRE_PORT   - The port containing the one-wire DQ line (i.e. PORTC)
//  CFG_ONE_WIRE_PIN    - The pin number of the one-wire DQ-line (i.e. 3)
//
//---------------------------------------------------------------------------

/* ---- Constants and Types ---------------------------------------------- */

#if !defined( CFG_ONE_WIRE_PORT )
#error  CFG_ONE_WIRE_PORT is not defined
#endif

#if !defined( CFG_ONE_WIRE_PIN )
#error  CFG_ONE_WIRE_PIN is not defined
#endif

// We rely on the fact that the PIN, DDR, and PORT registers are
// always in exactly that order.

#define DDR( port ) (*(&(port) - 1))
#define PIN( port ) (*(&(port) - 2))

#define DQ_MASK ( 1 << CFG_ONE_WIRE_PIN )

typedef uint8_t OW_HAL_AtomicFlags_t;

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

//***************************************************************************
/**
*   The timing on one-wire is fairly precise, so we need to disable
*   interrupts in order to ensure that the operations are timely.
*/
static inline OW_HAL_AtomicFlags_t OW_HAL_AtomicStart( void )
{
    uint8_t sreg = SREG;
    cli();

    return sreg;
}

//***************************************************************************
/**
*   The timing on one-wire is fairly precise, so we need to disable
*   interrupts in order to ensure that the operations are timely.
*/
static inline void OW_HAL_AtomicEnd( OW_HAL_AtomicFlags_t flags )
{
    SREG = flags;
}

//***************************************************************************
/**
*   Initializes the port and pin for use with one-wire
*/
static inline void OW_HAL_Init( void )
{
    uint8_t sreg = SREG;
    cli();

    // Start things off with the pin being an input

    DDR( CFG_ONE_WIRE_PORT ) &= ~DQ_MASK;

    // Enable the internal pullup. This allows one-wire devices to be
    // used with no external pullup

    CFG_ONE_WIRE_PORT |= DQ_MASK;

    SREG = sreg;
}

//***************************************************************************
/**
*   Deleys for the specified number of microseconds.
*/

static inline void OW_HAL_DelayUSecs
(
    uint16_t usecs  ///< Number of microseconds to delay
)
{
    us_spin( usecs );
}

//***************************************************************************
/**
*   Drive the DQ line low.
*/

static inline void OW_HAL_DriveDQLow( void )
{
    CFG_ONE_WIRE_PORT &= ~DQ_MASK;
    DDR( CFG_ONE_WIRE_PORT ) |= DQ_MASK;
}

//***************************************************************************
/**
*   Releases the DQ lines, which will allow the external pullup
*   resistor to pull thel line high.
*/

static inline void OW_HAL_ReleaseDQ( void )
{
    CFG_ONE_WIRE_PORT |= DQ_MASK;
    DDR( CFG_ONE_WIRE_PORT ) &= ~DQ_MASK;
}

//***************************************************************************
/**
*   Reads the current value on the DQ line.
*/

static inline uint8_t OW_HAL_ReadDQ( void )
{
    return ( PIN( CFG_ONE_WIRE_PORT ) & DQ_MASK ) != 0;
}

#if defined( __cplusplus )
}
#endif

/** @} */

#endif /* ONE_WIRE_AVR_H */

