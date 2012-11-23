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
*   @file    PacketProtocol.c 
*
*   @brief   This file contains the implementation of the packet protocol.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdlib.h>

#include "PKT_Packet.h"
#include "Crc8.h"
#include "Log.h"

/* ---- Public Variables -------------------------------------------------- */

PKT_TextCharFuncPtr         PKT_TextChar;
PKT_PacketReceivedFuncPtr   PKT_PacketReceived;
PKT_SendCharFuncPtr         PKT_SendChar;

/* ---- Private Constants and Types --------------------------------------- */

typedef enum
{
    Text,
    PacketLen,
    PacketData,
    PacketCrc,
    PacketEnd

} State;

typedef struct
{
    unsigned char   idx;
    unsigned char   crcFailures;

} PKT_Globals;

/* ---- Private Variables ------------------------------------------------- */

static  State       gState;

static  PKT_Packet  gPacket;
static  PKT_Globals gGlobals;

/* ---- Private Function Prototypes --------------------------------------- */

#if defined( __CYGWIN__ )
#   define  PKT_LOG_ENABLED 0
#else
#   define  PKT_LOG_ENABLED 0
#endif

#if PKT_LOG_ENABLED
#   define  PKT_LOG(x)       Log x
#   define  PKT_STATE_LOG(x) Log x
#else
#   define  PKT_LOG(x)
#   define  PKT_STATE_LOG(x)
#endif

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup PacketProtocol
 * @{
 */

/****************************************************************************/
/**
*   This function should be called for each character that is received on 
*   the serial port. It will run it through it's internal state machine
*   and call PKT_TextChar or PKT_PacketReceived as appropriate.
*
*   @param   ch (in)  Character that was received
*/

void PKT_ProcessChar( unsigned char ch, void *userData )
{
    switch ( gState )
    {
        case Text:
        {
            if ( ch == PKT_ETX )
            {
                PKT_STATE_LOG(( "PKT: Rcvd ETX\n" ));

                gState = PacketLen;
                return;
            }

            if ( PKT_TextChar == NULL )
            {
                PKT_LOG(( "PKT_ProcessChar: PKT_TextChar is NULL\n" ));
            }
            else
            {
                //PKT_STATE_LOG(( "PKT: Rcvd Text char: '%c'\n", ch ));

                PKT_TextChar( ch, userData );
            }
            break;
        }

        case PacketLen:
        {
            gPacket.len = ch;

            PKT_STATE_LOG(( "PKT: Rcvd Len: %d\n", ch ));

            gPacket.crc = Crc8( 0, ch );

            if ( gPacket.len > sizeof( gPacket.data ))
            {
                // Invalid packet.

                gState = PacketEnd;
            }
            else
            {
                // We have a length that looks valid - start collecting
                // data.
                gGlobals.idx = 0;

                if ( gPacket.len == 0 )
                {
                    gState = PacketCrc;
                }
                else
                {
                    gState = PacketData;
                }
            }
            break;
        }

        case PacketData:
        {
            PKT_STATE_LOG(( "PKT: Rcvd Data: 0x%02x\n", ch ));

            gPacket.data[ gGlobals.idx++ ] = ch;

            gPacket.crc = Crc8( gPacket.crc, ch );

            if ( gGlobals.idx >= gPacket.len )
            {
                gState = PacketCrc;
            }
            break;
        }

        case PacketCrc:
        {
            PKT_STATE_LOG(( "PKT: Rcvd Crc: 0x%02x\n", ch ));

            if ( gPacket.crc == ch )
            {
                PKT_STATE_LOG(( "PKT: Crc passed\n" ));

                // Packet received fine

                if ( PKT_PacketReceived == NULL )
                {
                    PKT_LOG(( "PKT_ProcessChar: PKT_PacketReceived is NULL\n" ));
                }
                else
                {
                    PKT_PacketReceived( &gPacket, userData );
                }
            }
            else
            {
                // CRC failed - we just log an error and ignore the packet

                PKT_LOG(( "PKT_ProcessChar: CRC failed\n" ));

                gGlobals.crcFailures++;
            }
            gState = PacketEnd;
            break;
        }

        case PacketEnd:
        {
            if ( ch == PKT_STX )
            {
                PKT_STATE_LOG(( "PKT: Rcvd STX\n" ));

                gState = Text;
            }
            else
            {
                PKT_STATE_LOG(( "PKT: Expecting STX, Rcvd 0x%02x\n", ch ));
            }
            break;
        }

        default:
        {
            PKT_STATE_LOG(( "PKT: Unrecognized state: %d\n", gState ));
            break;
        }
    }

} /* PKT_ProcessChar */

/****************************************************************************/
/**
*   Sends some data out as a packet.
*/

void PKT_SendPacket
(
    const PKT_Packet    *pkt,       /**< Pointer to packet to send,         */
    void                *userData   /**< Data to pass onto callback.        */
)
{
    unsigned char i;

    if ( pkt->len > sizeof( pkt->data ))
    {
        PKT_LOG(( "PKT_SendPacket: len %d is too big (max %d)\n", pkt->len, sizeof( pkt->data )));
        return;
    }

    if ( PKT_SendChar == NULL )
    {
        PKT_LOG(( "PKT_SendPacket: PKT_SendChar is NULL\n" ));
        return;
    }

    unsigned char   crc;

    crc = Crc8( 0, pkt->len );

    for ( i = 0; i < pkt->len; i++ ) 
    {
        crc = Crc8( crc, pkt->data[ i ]);
    }

#if PKT_LOG_ENABLED

    PKT_LOG(( "Sending %02x %02x ", PKT_ETX, pkt->len ));
    for ( i = 0; i < pkt->len; i++ ) 
    {
        PKT_LOG(( "%02x ", pkt->data[ i ], userData ));
    }
    PKT_LOG(( "%02x %02x\n", crc, PKT_STX ));

#endif

    PKT_SendChar( PKT_ETX, userData );
    PKT_SendChar( pkt->len, userData );

    for ( i = 0; i < pkt->len; i++ ) 
    {
        PKT_SendChar( pkt->data[ i ], userData );
    }

    PKT_SendChar( crc,     userData );
    PKT_SendChar( PKT_STX, userData );

} // PKT_SendPacket

/****************************************************************************/
/**
*   Sends a character string out as text data
*/

void PKT_SendText( const char *str, void *userData )
{
    char    ch;

    if ( PKT_SendChar == NULL )
    {
        PKT_LOG(( "PKT_SendPacket: PKT_SendChar is NULL\n" ));
        return;
    }

    while (( ch = *str++ ) != '\0' )
    {
        PKT_SendChar( (unsigned char)ch, userData );
    }

} // PKT_SendText


/** @} */

