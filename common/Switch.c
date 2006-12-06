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

#include <avr/io.h>

#include "Switch.h"
#include "Expansion.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

// Stores the raw value retrieved for the above switches

static  uint16_t    gRawSwitch;

#define SWITCH_STATE_OFF            0
#define SWITCH_STATE_BOUNCING_ON    1
#define SWITCH_STATE_ON             2
#define SWITCH_STATE_BOUNCING_OFF   3

#define SWITCH_BOUNCE_ON_COUNT      3   // 10 msec/count or 100ths of a sec
#define SWITCH_BOUNCE_OFF_COUNT     1   // 10 msec/count or 100ths of a sec

static  uint8_t gSwitchBounceCount[ NUM_SWITCHES ];
static  uint8_t gSwitchState[ NUM_SWITCHES ];

/***************************************************************************/
/**
*   Checks to see if a button is pressed. This is a raw check and doesn't
*   factor debouncing into the equation
*/

static inline uint8_t IsRawSwitchPressed( uint8_t switchNum )
{
    // All of the buttons have pullups and closing the button connects to
    // ground

    return ( gRawSwitch & ( 1 << switchNum )) == 0;

} // IsRawSwitchPressed

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

static inline void ClearSwitchBounceCounter( SwitchNum_t sw )
{
    gSwitchBounceCounter[ sw ] = 0;

} // ClearSwitchBounceCounter

/***************************************************************************/
/**
*   Increments the switch bounce counter
*/

static inline void IncrementSwitchBounceCounter( SwitchNum_t sw )
{
    gSwitchBounceCounter[ sw ]++;

} // IncrementSwitchBounceCounter

/***************************************************************************/
/**
*   Checks to see if keypress events have occurred.
*/

void CheckSwitches( void )
{
    uint8_t     sw;

    // Retrieve most of the switches from the expansion card.

    gRawSwitch = EXP_TransferWord( 0, 0 ) & 0x7FF0;

    if ( PIND & ( 1 << 6 ))
    {
        // Switch S3 comes from Port D.6

        gRawSwitch |= ( 1 << SWITCH_S3 );
    }

    for ( sw = 4; sw < NUM_SWITCHES; sw++ ) 
    {
        switch ( gSwitchState[ sw ])
        {
            case SWITCH_STATE_OFF:
            {
                if ( IsRawSwitchPressed( sw ))
                {
                    gSwitchState[ sw ] = SWITCH_STATE_BOUNCING_ON;
                    ClearSwitchBounceCounter();
                }
                break;
            }

            case SWITCH_STATE_BOUNCING_ON:
            {
                if ( gSwitchBounceCount[ sw ] >= SWITCH_BOUNCE_ON_COUNT )
                {
                    if ( IsRawSwitchPressed( sw ) )
                    {
                        gSwitchState[ sw ] = SWITCH_STATE_ON;
                        SwitchTurnedOn( sw );
                    }
                    else
                    {
                        gSwitchState[ sw ] = SWITCH_STATE_OFF;
                    }
                }
                else
                {
                    gSwitchBounceCount[ sw ]++;
                }
                break;
            }

            case SWITCH_STATE_ON:
            {
                if ( !IsRawSwitchPressed( sw ))
                {
                    gSwitchState[ sw ] = SWITCH_STATE_BOUNCING_OFF;
                    ClearSwitchBounceCounter();
                }
                break;
            }

            case SWITCH_STATE_BOUNCING_OFF:
            {
                if ( gSwitchBounceCount[ sw ] >= SWITCH_BOUNCE_OFF_COUNT )
                {
                    if ( IsRawSwitchPressed( sw ) )
                    {
                        gSwitchState[ sw ] = SWITCH_STATE_ON;
                    }
                    else
                    {
                        gSwitchState[ sw ] = SWITCH_STATE_OFF;
                        SwitchTurnedOff( sw );
                    }
                }
                else
                {
                    gSwitchBounceCount[ sw ]++;
                }
                break;
            }
        }
    }
}


