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
*   @file   bioloid-dump.c 
*
*   @brief  Contains constants and strings which are useful for dumping
*           bioloid packets.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdint.h>

#include "bioloid-dump.h"
#include "Log.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Dumps the command portion of a packet.
*/

void BLD_DumpCmdPacket( BLD_Packet_t *pkt )
{
    uint8_t     i;

    switch ( pkt->m_cmd )
    {
        case BLD_CMD_PING:
        {
            Log( "ID:0x%02x Cmd: Ping\n", pkt->m_id );
            break;
        }

        case BLD_CMD_READ:
        {
            Log( "ID:0x%02x Cmd: Read @ 0x%02x Len:%d\n", pkt->m_id, pkt->m_param[ 0 ], pkt->m_param[ 1 ]);
            break;
        }

        case BLD_CMD_WRITE:
        {
            Log( "ID:0x%02x Cmd: Write @ 0x%02x [", pkt->m_id, pkt->m_param[ 0 ] );
            for ( i = 1; i <= pkt->m_length - 3; i++ ) 
            {
                Log( " 0x%02x", pkt->m_param[ i ] );
            }
            Log( " ]\n" );
            break;
        }

        case BLD_CMD_REG_WRITE:
        {
            Log( "ID:0x%02x Cmd: RegWrite @ 0x%02x [", pkt->m_id, pkt->m_param[ 0 ] );
            for ( i = 1; i <= pkt->m_length - 3; i++ ) 
            {
                Log( " 0x%02x", pkt->m_param[ i ] );
            }
            Log( " ]\n" );
            break;
        }

        case BLD_CMD_ACTION:
        {
            Log( "ID:0x%02x Cmd: Action\n", pkt->m_id );
            break;
        }

        case BLD_CMD_RESET:
        {
            Log( "ID:0x%02x Cmd: Reset\n", pkt->m_id );
            break;
        }

        case BLD_CMD_SYNC_WRITE:
        {
            uint8_t j;
            uint8_t actuatorLen  = pkt->m_param[1];
            uint8_t numActuators = ( pkt->m_length - 4 ) / ( actuatorLen + 1 );
            uint8_t paramIdx = 2;

            Log( "ID:0x%02x Cmd: SyncWrite @ 0x%02x Len:%d\n", pkt->m_id, pkt->m_param[0], actuatorLen );

            for ( j = 0; j < numActuators; j++ ) 
            {
                Log( "   ID:0x%02x [", pkt->m_param[ paramIdx++ ] );

                for ( i = 0; i < actuatorLen; i++ ) 
                {
                    Log( " 0x%02x", pkt->m_param[ paramIdx++ ] );
                }
                Log( " ]\n" );
            }
            break;
        }

        default:
        {
            Log( "ID:0x%02x Cmd: 0x%02x *** Unknown ***\n", pkt->m_id, pkt->m_cmd );
            break;
        }
    }
    
} // BLD_DumpCmdPacket

//***************************************************************************
/**
*   Dumps the response portion of a packet.
*/

void BLD_DumpRspPacket( BLD_Packet_t *pkt )
{
    uint8_t i;

    Log( "ID:0x%02x Err:0x%02x", pkt->m_id, pkt->m_cmd );

    if ( pkt->m_length > 2 )
    {
        Log( " [" );
        for ( i = 0; i < ( pkt->m_length - 2 ); i++ ) 
        {
            Log( " 0x%02x", pkt->m_param[ i ] );
        }
        Log( " ]" );
    }
    Log( "\n" );

} // BLD_DumpRspPacket
