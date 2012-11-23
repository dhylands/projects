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
*   @file   Switch.c
*
*   @brief  Provides switch debounc logic. The original algorithim for this
*           was posted by Craig Limber:
*           http://members.shaw.ca/climber/avrbuttons.html
*
*   This particular implementation is for the SRS Workshop Bot expansion 
*   board.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdint.h>

#include "Switch.h"

/* ---- Public Variables -------------------------------------------------- */

uint8_t gSwitchRaw[ NUM_SWITCH_BYTES ];
uint8_t gSwitchEnabled[ NUM_SWITCH_BYTES ];
                                                         
/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

// Stores the raw value retrieved for the above switches

#define SWITCH_STATE_OFF            0
#define SWITCH_STATE_BOUNCING_ON    1
#define SWITCH_STATE_ON             2
#define SWITCH_STATE_BOUNCING_OFF   3

static  SwitchCount_t   gSwitchBounceCount[ CFG_NUM_SWITCHES ];
static  SwitchState_t   gSwitchState[ CFG_NUM_SWITCHES ];

/***************************************************************************/
/**
*   Sets the state for a switch
*/

static inline void SetSwitchState( SwitchNum_t sw, SwitchState_t state )
{
    gSwitchState[ sw ] = state;

} // SetSwitchState

/***************************************************************************/
/**
*   Clears the switch bounce counter
*/

static inline void ClearSwitchBounceCount( SwitchNum_t sw )
{
    gSwitchBounceCount[ sw ] = 0;

} // ClearSwitchBounceCounter

/***************************************************************************/
/**
*   Increments the switch bounce counter
*/

static inline void IncrementSwitchBounceCount( SwitchNum_t sw )
{
    gSwitchBounceCount[ sw ]++;

} // IncrementSwitchBounceCounter

/***************************************************************************/
/**
*   Retrieves the bounce counter for a particular switch
*/

static inline SwitchCount_t SwitchBounceCount( SwitchNum_t sw )
{
    return gSwitchBounceCount[ sw ];

} // IncrementSwitchBounceCounter



/***************************************************************************/
/**
*   Checks to see if keypress events have occurred.
*/

void CheckSwitches( void )
{
    uint8_t     sw;

    for ( sw = 0; sw < CFG_NUM_SWITCHES; sw++ ) 
    {
        if ( IsSwitchEnabled( sw ))
        {
            switch ( gSwitchState[ sw ])
            {
                case SWITCH_STATE_OFF:
                {
                    if ( IsRawSwitchPressed( sw ))
                    {
                        SetSwitchState( sw, SWITCH_STATE_BOUNCING_ON );
                        ClearSwitchBounceCount( sw );
                    }
                    break;
                }
    
                case SWITCH_STATE_BOUNCING_ON:
                {
                    if ( SwitchBounceCount(  sw ) >= CFG_SWITCH_BOUNCE_ON_COUNT )
                    {
                        if ( IsRawSwitchPressed( sw ) )
                        {
                            SetSwitchState( sw, SWITCH_STATE_ON );
                            SwitchEvent( sw, SWITCH_EVENT_PRESSED );
                        }
                        else
                        {
                            SetSwitchState( sw, SWITCH_STATE_OFF );
                        }
                    }
                    else
                    {
                        IncrementSwitchBounceCount( sw );
                    }
                    break;
                }
    
                case SWITCH_STATE_ON:
                {
                    if ( !IsRawSwitchPressed( sw ))
                    {
                        SetSwitchState( sw, SWITCH_STATE_BOUNCING_OFF );
                        ClearSwitchBounceCount( sw );
                    }
                    break;
                }
    
                case SWITCH_STATE_BOUNCING_OFF:
                {
                    if ( SwitchBounceCount( sw ) >= CFG_SWITCH_BOUNCE_OFF_COUNT )
                    {
                        if ( IsRawSwitchPressed( sw ) )
                        {
                            SetSwitchState( sw, SWITCH_STATE_ON );
                        }
                        else
                        {
                            SetSwitchState( sw, SWITCH_STATE_OFF );
                            SwitchEvent( sw, SWITCH_EVENT_RELEASED );
                        }
                    }
                    else
                    {
                        IncrementSwitchBounceCount( sw );
                    }
                    break;
                }
            }
        }
        else
        {
            SetSwitchState( sw, SWITCH_STATE_OFF );
        }
    }

} // CheckSwitches

