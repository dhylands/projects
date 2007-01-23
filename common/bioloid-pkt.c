/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   bioloid.c 
*
*   @brief  Implements a parser for detecting and parsing packets sent to
*           the bioloid servos (like the AX-12)
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "bioloid-pkt.h"
#include "Log.h"

#include <string.h>

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup bioloid
 * @{
 */

//***************************************************************************
/**
*   Initializes a BLD_State_t structure.
*/

void BLD_Init( BLD_Instance_t *inst )
{
    memset( inst, 0, sizeof( *inst ));

} // BLD_Init

//***************************************************************************
/**
*   Runs the packet parsing state machine. When a packet is recognized, 
*   the PacketReceived callback will be called.
*/

void BLD_ProcessChar( BLD_Instance_t *inst, uint8_t ch )
{
    BLD_State_t nextState = inst->m_state;

    switch ( nextState )
    {
        case BLD_STATE_IDLE:    // We're waiting for the beginning of the packet (0xFF)
        {
            if ( ch == 0xFF )
            {
                nextState = BLD_STATE_1ST_FF_RCVD;
            }
            break;
        }

        case BLD_STATE_1ST_FF_RCVD:  // We've received the 1st 0xFF
        {
            if ( ch == 0xFF )
            {
                nextState = BLD_STATE_2ND_FF_RCVD;
            }
            break;
        }

        case BLD_STATE_2ND_FF_RCVD:  // We've received the 2nd 0xFF, ch is the ID
        {
            inst->m_id = ch;
            inst->m_pkt.m_crc = ch;
            nextState = BLD_STATE_ID_RCVD;
            break;
        }

        case BLD_STATE_ID_RCVD:      // We've received the ID, ch is the length
        {
            inst->m_pkt.m_length = ch;
            inst->m_pkt.m_crc += ch;
            nextState = BLD_STATE_LENGTH_RCVD;
            break;
        }

        case BLD_STATE_LENGTH_RCVD:  // We've received the length, ch is the command
        {
            inst->m_pkt.m_cmd = ch;
            inst->m_pkt.m_crc += ch;
            inst->m_paramIdx = 0;
            nextState = BLD_STATE_COMMAND_RCVD;
            break;
        }

        case BLD_STATE_COMMAND_RCVD: // We've received the command, ch is a param byte or checksum
        {
            if (( inst->m_paramIdx + 2 ) >= inst->m_pkt.m_length )
            {
                BLD_Error_t err = BLD_ERROR_NONE;

                // ch is the Checksum

                inst->m_pkt.m_crc = ~inst->m_pkt.m_crc;

                if ( inst->m_pkt.m_crc != ch )
                {
                    // CRC failed

                    LogError( "CRC failed: calculated 0x%02x, read: 0x%02x\n", inst->m_pkt.m_crc, ch );

                    err = BLD_ERROR_CHECKSUM;
                }

                if ( inst->m_pktRcvd != NULL )
                {
                    inst->m_pktRcvd( inst, &inst->m_pkt, err );
                }
                nextState = BLD_STATE_IDLE;
                break;
            }

            if ( inst->m_paramIdx < sizeof( inst->m_pkt.m_param ))
            {
                inst->m_pkt.m_crc += ch;
                inst->m_pkt.m_param[ inst->m_paramIdx ] = ch;
            }
            inst->m_paramIdx++;
            break;
        }
    }

    inst->m_state = nextState;

} // BLD_ProcessChar


/** @} */

