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
*   @file   Switch.h
*
*   @brief  Provides switch debounc logic. The original algorithim for this
*           was posted by Craig Limber:
*           http://members.shaw.ca/climber/avrbuttons.html
*
****************************************************************************/

#if !defined( SWITCH_H )
#define SWITCH_H            /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>
#include "Config.h"

/* ---- Constants and Types ---------------------------------------------- */

// This file expects the following options to be defined in Config.h
//
//  CFG_NUM_SWITCHES            - Determines the total number of switches that 
//                                will be checked.
//                      
//  CFG_SWITCH_BOUNCE_ON_COUNT  - Determines the number of times that the 
//                                the swith needs to be seen on before it's
//                                considered to be really on.
//
//  CFG_SWITCH_BOUNCE_OFF_COUNT - Determines the number of times that the 
//                                the swith needs to be seen off before it's
//                                considered to be really off.
//
//  The caller is responsible for setting up the raw switch states and
//  calling CheckSwitches on some type of periodic basis - assumed to be once
//  every 10 msec, but this is pretty arbitrary.

#if !defined( CFG_NUM_SWITCHES )
#   error   CFG_NUM_SWITCHES not defined
#endif
#if !defined( CFG_SWITCH_BOUNCE_ON_COUNT )
#   define  CFG_SWITCH_BOUNCE_ON_COUNT  3
#endif
#if !defined( CFG_SWITCH_BOUNCE_OFF_COUNT )
#   define  CFG_SWITCH_BOUNCE_OFF_COUNT     CFG_SWITCH_BOUNCE_ON_COUNT
#endif

#if defined( AVR )
typedef uint8_t     SwitchEvent_t;
typedef uint8_t     SwitchNum_t;
typedef uint8_t     SwitchBool_t;
#else
typedef unsigned    SwitchEvent_t;
typedef unsigned    SwitchNum_t;
typedef unsigned    SwitchBool_t;
#endif
typedef uint8_t     SwitchCount_t;
typedef uint8_t     SwitchState_t;

#define SWITCH_EVENT_RELEASED   0
#define SWITCH_EVENT_PRESSED    1

/* ---- Variable Externs ------------------------------------------------- */

#define NUM_SWITCH_BYTES    (( CFG_NUM_SWITCHES + 7 ) / 8 )

extern  uint8_t gSwitchRaw[ NUM_SWITCH_BYTES ];
extern  uint8_t gSwitchEnabled[ NUM_SWITCH_BYTES ];

#define SWITCH_RAW_BYTE( sw )       (gSwitchRaw[ (sw) / 8 ])
#define SWITCH_ENABLED_BYTE( sw )   (gSwitchEnabled[ (sw) / 8 ])
#define SWITCH_MASK( sw )           ( 1 << ( sw & 7 ))

/* ---- Inline Functions ------------------------------------------------- */

/***************************************************************************/
/**
*   Checks to see if a switch is pressed. This is a raw check and doesn't
*   factor debouncing into the equation
*/

static inline SwitchBool_t IsRawSwitchPressed( SwitchNum_t sw )
{
    return ( SWITCH_RAW_BYTE( sw ) & SWITCH_MASK( sw )) != 0;

} // IsRawSwitchPressed

/***************************************************************************/
/**
*   Checks to see if a switch is enabled.
*/

static inline SwitchBool_t IsSwitchEnabled( SwitchNum_t sw )
{
    return ( SWITCH_ENABLED_BYTE( sw ) & SWITCH_MASK( sw )) != 0;

} // IsSwitchEnabled

/***************************************************************************/
/**
*   Sets an individual switch as being set or cleared.
*/

static inline void SetRawSwitch( SwitchNum_t sw, SwitchBool_t isSet )
{
    if ( isSet )
    {
        SWITCH_RAW_BYTE( sw ) |= SWITCH_MASK( sw );
    }
    else
    {
        SWITCH_RAW_BYTE( sw ) &= ~SWITCH_MASK( sw );
    }

} // SetRawSwitch

/***************************************************************************/
/**
*   Enables a switch for processing.
*/

static inline void EnableSwitch( SwitchNum_t sw )
{
    SWITCH_ENABLED_BYTE( sw ) |= SWITCH_MASK( sw );

} // EnableSwitch

/***************************************************************************/
/**
*   Disables a switch for processing.
*/

static inline void DisableSwitch( SwitchNum_t sw )
{
    SWITCH_ENABLED_BYTE( sw ) &= ~SWITCH_MASK( sw );

} // EnableSwitch

/* ---- Function Prototypes ---------------------------------------------- */

void CheckSwitches( void );

// The caller is expected to provide the SwitchEvent fnuction with the
// following prototype.

void SwitchEvent( SwitchNum_t switchNum, SwitchEvent_t event );
                                                 
#endif  // SWITCH_H
