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

#if 1
#   define AccumCrc( inst, ch )    ((inst)->m_pkt.m_crc += (ch))
#else
#   define AccumCrc( inst, ch ) \
    do { \
        (inst)->m_pkt.m_crc += (ch); \
        BLD_LogPacket( inst, "Accum 0x%02x into crc: 0x%02x\n", ch, (inst)->m_pkt.m_crc ); \
    } while (0)
#endif

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
            else
            {
                nextState = BLD_STATE_IDLE;
            }
            break;
        }

        case BLD_STATE_2ND_FF_RCVD:  // We've received the 2nd 0xFF, ch is the ID
        {
            if ( ch == 0xFF )
            {
                // 0xFF is invalid as an ID, so just stay in this state until we receive
                // a non-0xFF
                
                nextState = BLD_STATE_2ND_FF_RCVD;
                break;
            }
            inst->m_pkt.m_id = ch;
            AccumCrc( inst, ch );
            nextState = BLD_STATE_ID_RCVD;
            break;
        }

        case BLD_STATE_ID_RCVD:      // We've received the ID, ch is the length
        {
            inst->m_pkt.m_length = ch;
            AccumCrc( inst, ch );
            nextState = BLD_STATE_LENGTH_RCVD;
            break;
        }

        case BLD_STATE_LENGTH_RCVD:  // We've received the length, ch is the command
        {
            inst->m_pkt.m_cmd = ch;
            AccumCrc( inst, ch );
            inst->m_paramIdx = 0;
#if CFG_BLD_OPTIMIZE_SYNC_WRITE_PARSING

            if ( ch == BLD_CMD_SYNC_WRITE )
            {
                nextState = BLD_STATE_SYNC_WRITE_CMD_RCVD;
            }
            else
#endif
            {
                nextState = BLD_STATE_COMMAND_RCVD;
            }
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

                    err = BLD_ERROR_CHECKSUM;
                    LogError( "Rcvd Checksum: 0x%02x Expecting: 0x%02x\n",
                              ch, inst->m_pkt.m_crc );
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
                AccumCrc( inst, ch );
                inst->m_pkt.m_param[ inst->m_paramIdx ] = ch;
            }
            inst->m_paramIdx++;
            break;
        }

#if CFG_BLD_OPTIMIZE_SYNC_WRITE_PARSING
        // Format of the SYNC WRITE packet:
        // 
        // ID:          0xFE (Broadcast)
        // Length:        xx (L+1) * N + 4 where L=len for each ID, N = # of ids
        // Instruction: 0x83
        // Param 1:       AA Starting address where the data is written
        // Param 2:       LL Length of data for each ID (L)
        // 
        // The following repeats for  each ID:
        // ID:            xx ID of the first actuator
        // data 1..L      xx L bytes of data

        case BLD_STATE_SYNC_WRITE_CMD_RCVD:  // We've received the command, ch is a param byte or checksum
        {
            // Save Param1 and Param2 (address and length)

            inst->m_pkt.m_param[ inst->m_paramIdx ] = ch;
            inst->m_paramIdx++;
            AccumCrc( inst, ch );
            if ( inst->m_paramIdx >= 2 )
            {
                inst->m_syncWriteLen = inst->m_pkt.m_param[1];
                nextState = BLD_STATE_SYNC_WRITE_LEN_RCVD;
            }
            break;
        }

        case BLD_STATE_SYNC_WRITE_LEN_RCVD:
        {
            if (( inst->m_paramIdx + 2 ) >= inst->m_pkt.m_length )
            {
                BLD_Error_t err = BLD_ERROR_NONE;

                // ch is the Checksum

                inst->m_pkt.m_crc = ~inst->m_pkt.m_crc;

                if ( inst->m_pkt.m_crc != ch )
                {
                    // CRC failed

                    err = BLD_ERROR_CHECKSUM;
                    LogError( "Rcvd Checksum: 0x%02x Expecting: 0x%02x\n",
                              ch, inst->m_pkt.m_crc );
                }

                if ( inst->m_pktRcvd != NULL )
                {
                    inst->m_pktRcvd( inst, &inst->m_pkt, err );
                }
                nextState = BLD_STATE_IDLE;
                break;
            }

            AccumCrc( inst, ch );
            inst->m_syncWriteIdx = 0;

            if ( ch == inst->m_id )
            {
                // The ID matches our ID, store the data

                inst->m_pkt.m_param[ inst->m_paramIdx ] = ch;
                inst->m_paramIdx++;

                nextState = BLD_STATE_SYNC_WRITE_STORE_DATA;
            }
            else
            {
                // The ID doesn't match our ID, skip the data

                inst->m_pkt.m_length--;
                nextState = BLD_STATE_SYNC_WRITE_SKIP_DATA;
            }
            break;
        }

        case BLD_STATE_SYNC_WRITE_STORE_DATA:
        {
            if ( inst->m_paramIdx < sizeof( inst->m_pkt.m_param ))
            {
                inst->m_pkt.m_param[ inst->m_paramIdx ] = ch;
                inst->m_paramIdx++;
            }

            AccumCrc( inst, ch );
            inst->m_syncWriteIdx++;
            if ( inst->m_syncWriteIdx >= inst->m_syncWriteLen )
            {
                // We've reached the end of the data for this 
                // ID, move on to the next one

                nextState = BLD_STATE_SYNC_WRITE_LEN_RCVD;
            }
            break;
        }
        case BLD_STATE_SYNC_WRITE_SKIP_DATA:
        {
            // We're not going to store this character,
            // so we need to subtract one from the length

            inst->m_pkt.m_length--;
            AccumCrc( inst, ch );
            inst->m_syncWriteIdx++;
            if ( inst->m_syncWriteIdx >= inst->m_syncWriteLen )
            {
                nextState = BLD_STATE_SYNC_WRITE_LEN_RCVD;
            }
            break;
        }
#endif // CFG_BLD_OPTIMIZE_SYNC_WRITE_PARSING
    }

    inst->m_state = nextState;

} // BLD_ProcessChar

//***************************************************************************
/**
*   Sends out a status packet.
*
*   The format of a status packet looks like the following:
*
*       0xff 0xff ID Len Error [Param1 ... ParamN] ChkSum
*/

void BLD_SendStatus
(
    BLD_Instance_t *inst,       ///< Instance data 
    uint8_t         errCode,    ///< Error bitmask to send
    const void     *param,      ///< Pointer to parameter data to send. May be NULL if paramLen is zero
    uint8_t         paramLen    ///< Number of bytes of parameter data to send
)
{
    const uint8_t  *data;
    uint8_t         checksum;
    uint8_t         paramIdx;
    BLD_SendChar    sendChar = inst->m_sendChar;

    BLD_LogPacket( inst, "Sending STATUS errCode: %d\n", errCode );

    // Send out the preamble

    sendChar( 0xff );
    sendChar( 0xff );
    sendChar( inst->m_id );
    checksum = inst->m_id;

    // The length sent in the packet is the number of parameter bytes + 2

    sendChar( paramLen + 2 );
    checksum += ( paramLen + 2 );

    sendChar( errCode );
    checksum += errCode;

    data = param;
    for ( paramIdx = 0; paramIdx < paramLen; paramIdx++ )
    {
        checksum += *data;
        sendChar( *data++ );
    }

    // Finally send out the checksum

    sendChar( checksum );

} // BLD_SendStatus

/** @} */

