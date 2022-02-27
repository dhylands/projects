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
 *   To use:
 *
 *   This code is fairly generic and makes no assumptions about how switches
 *   map to your hardware.
 *
 *   This code works on the premise that CheckSwitches will be called on
 *   a periodic basis. It doesn't really matter what the period is, as long
 *   as its called regularly. If your main loop runs say 100 times per second
 *   then you can just call CheckSwitches each time the main loop is called.
 *
 *   If, like on Arduino, the main loop runs as often as possible, then you
 *   should probably keep track of when you last called CheckSwitches and call
 *   it after 5 or 10 msec have elapsed.
 *
 *   Generally speaking, it takes some number of milliseconds to debounce a
 *   switch. You can configure how many counts are needed. The total time
 *   to debounce will be the count times the period theat CheckSwitches is
 *   called.
 *
 *   For example: you call CheckSwitches every 10 msec, and it takes 23 msec
 *   to debounce your switches, you should set the count to be 3.
 *
 *   You should call SetRawSwitch to indicate the raw state of the switch.
 *   This could be everytime through your main loop, or perhaps on a pin
 *   change interrupt.
 *
 *   You should provide a function with the following prototype:
 *
 *   void SwitchEvent( SwitchNum_t switchNum, SwitchEvent_t event );
 *
 *   switchNum will be the number of the switch, and event will be one of
 *   SWITCH_EVENT_RELEASED or SWITCH_EVENT_PRESSED.
 *
 *   You should provide a header file called Config.h which contains the
 *   following definitions:
 *
 *   CFG_NUM_SWITCHES    - the total number of switches you wish to control.
 *                         This is an arbitrary number. Your switch numbers
 *                         are expected t be between 0 and (CFG_NUM_SWITCHES - 1)
 *
 *   CFG_SWITCH_BOUNCE_ON_COUNT  - a number which indicates how many times
 *                                 CheckSwitches needs to be called before
 *                                 the switch state is considered to be on.
 *
 *   CFG_SWITCH_BOUNCE_OFF_COUNT - a number which indicates how many times
 *                                 CheckSwitches needs to be called before
 *                                 the switch state is considered to be off.
 *
 ****************************************************************************/

#if !defined(SWITCH_H)
#define SWITCH_H /**< Include Guard                             */

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

#if !defined(CFG_NUM_SWITCHES)
#error CFG_NUM_SWITCHES not defined
#endif
#if !defined(CFG_SWITCH_BOUNCE_ON_COUNT)
#define CFG_SWITCH_BOUNCE_ON_COUNT 3
#endif
#if !defined(CFG_SWITCH_BOUNCE_OFF_COUNT)
#define CFG_SWITCH_BOUNCE_OFF_COUNT CFG_SWITCH_BOUNCE_ON_COUNT
#endif

#if defined(AVR)
typedef uint8_t SwitchEvent_t;
typedef uint8_t SwitchNum_t;
typedef uint8_t SwitchBool_t;
#else
typedef unsigned SwitchEvent_t;
typedef unsigned SwitchNum_t;
typedef unsigned SwitchBool_t;
#endif
typedef uint8_t SwitchCount_t;
typedef uint8_t SwitchState_t;

#define SWITCH_EVENT_RELEASED 0
#define SWITCH_EVENT_PRESSED 1

/* ---- Variable Externs ------------------------------------------------- */

#define NUM_SWITCH_BYTES ((CFG_NUM_SWITCHES + 7) / 8)

extern uint8_t gSwitchRaw[NUM_SWITCH_BYTES];
extern uint8_t gSwitchEnabled[NUM_SWITCH_BYTES];

#define SWITCH_RAW_BYTE(sw) (gSwitchRaw[(sw) / 8])
#define SWITCH_ENABLED_BYTE(sw) (gSwitchEnabled[(sw) / 8])
#define SWITCH_MASK(sw) (1 << (sw & 7))

/* ---- Inline Functions ------------------------------------------------- */

/***************************************************************************/
/**
 *   Checks to see if a switch is pressed. This is a raw check and doesn't
 *   factor debouncing into the equation
 */

static inline SwitchBool_t IsRawSwitchPressed(SwitchNum_t sw) {
    return (SWITCH_RAW_BYTE(sw) & SWITCH_MASK(sw)) != 0;

}  // IsRawSwitchPressed

/***************************************************************************/
/**
 *   Checks to see if a switch is enabled.
 */

static inline SwitchBool_t IsSwitchEnabled(SwitchNum_t sw) {
    return (SWITCH_ENABLED_BYTE(sw) & SWITCH_MASK(sw)) != 0;

}  // IsSwitchEnabled

/***************************************************************************/
/**
 *   Sets an individual switch as being set or cleared.
 */

static inline void SetRawSwitch(SwitchNum_t sw, SwitchBool_t isSet) {
    if (isSet) {
        SWITCH_RAW_BYTE(sw) |= SWITCH_MASK(sw);
    } else {
        SWITCH_RAW_BYTE(sw) &= ~SWITCH_MASK(sw);
    }

}  // SetRawSwitch

/***************************************************************************/
/**
 *   Enables a switch for processing.
 */

static inline void EnableSwitch(SwitchNum_t sw) {
    SWITCH_ENABLED_BYTE(sw) |= SWITCH_MASK(sw);

}  // EnableSwitch

/***************************************************************************/
/**
 *   Disables a switch for processing.
 */

static inline void DisableSwitch(SwitchNum_t sw) {
    SWITCH_ENABLED_BYTE(sw) &= ~SWITCH_MASK(sw);

}  // EnableSwitch

/* ---- Function Prototypes ---------------------------------------------- */

void CheckSwitches(void);

// The caller is expected to provide the SwitchEvent fnuction with the
// following prototype.

void SwitchEvent(SwitchNum_t switchNum, SwitchEvent_t event);

#endif  // SWITCH_H
