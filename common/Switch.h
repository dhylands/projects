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
*   @brief  Provides an API for talking to character style LCDs. These
*           functions are designed around the HD44780 based LCD Controllers.
*
****************************************************************************/

#if !defined( SWITCH_H )
#define SWITCH_H            /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include "Config.h"

/* ---- Constants and Types ---------------------------------------------- */

// This file expects the following options to be defined in Config.h
//
//  CFG_NUM_SWITCHES
//

#if !defind( CFG_NUM_SWITCHES )
#   error   CFG_NUM_SWITCHES not defined
#endif

typedef uint8_t SwitchEvent_t;
typedef uint8_t SwitchNum_t;

#define SWITCH_EVENT_OFF    0
#define SWITCH_EVENT_ON     1

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void CheckSwitches( void );
void SwitchEvent( SwitchNum_t switchNum, SwitchEvent_t event );
                                                 
#endif  // SWITCH_H
