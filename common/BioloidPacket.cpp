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
*   @file   BioloidPacket.cpp
*
*   @brief  This file implements the BioloidPacket class, which is used
*           to parse incoming byte streams into packets.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "BioloidPacket.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup bioloid
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

BioloidPacket::BioloidPacket()
    : m_state( BioloidPacket::STATE_IDLE ),
      m_param( NULL ),
      m_maxParam( 0 )
{
}

//***************************************************************************
/**
*   Constructor where storage for parameter data is specified.
*/

BioloidPacket::BioloidPacket( void *data, uint8_t maxData )
    : m_state( BioloidPacket::STATE_IDLE ),
      m_param( static_cast< uint8_t * >( data )),
      m_maxParam( maxData )
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

BioloidPacket::~BioloidPacket()
{
}

//***************************************************************************
/**
*   Runs a single character through the state machine. Once a packet
*   has been parsed successfully, the PacketReceived virtual method
*   is called.
*
*   virtual
*/

Bioloid::Error BioloidPacket::ProcessChar( uint8_t ch )
{
    State           nextState = m_state;
    Bioloid::Error  err = Bioloid::ERROR_NOT_DONE;

    switch ( nextState )
    {
        case STATE_IDLE:    // We're waiting for the beginning of the packet (0xFF)
        {
            if ( ch == 0xFF )
            {
                nextState = STATE_1ST_FF_RCVD;
            }
            break;
        }

        case STATE_1ST_FF_RCVD:  // We've received the 1st 0xFF
        {
            if ( ch == 0xFF )
            {
                nextState = STATE_2ND_FF_RCVD;
            }
            else
            {
                nextState = STATE_IDLE;
            }
            break;
        }

        case STATE_2ND_FF_RCVD:  // We've received the 2nd 0xFF, ch is the ID
        {
            if ( ch == 0xFF )
            {
                // 0xFF is invalid as an ID, so just stay in this state until we receive
                // a non-0xFF

                nextState = STATE_2ND_FF_RCVD;
                break;
            }
            m_id = ch;
            m_checksum = ch;
            nextState = STATE_ID_RCVD;
            break;
        }

        case STATE_ID_RCVD:      // We've received the ID, ch is the length
        {
            m_length = ch;
            m_checksum += ch;
            nextState = STATE_LENGTH_RCVD;
            break;
        }

        case STATE_LENGTH_RCVD:  // We've received the length, ch is the command
        {
            m_cmd = ch;
            m_checksum += ch;
            m_paramIdx = 0;
            nextState = STATE_COMMAND_RCVD;
            break;
        }

        // NOTE: In the future, we should decode the SYNC_WRITE
        //       packet so that we only need to keep the portion that
        //       belongs to our ID

        case STATE_COMMAND_RCVD: // We've received the command, ch is a param byte or checksum
        {
            if (( m_paramIdx + 2 ) >= m_length )
            {
                // ch is the Checksum

                m_checksum = ~m_checksum;

                if ( m_checksum == ch )
                {
                    if ( m_paramIdx <= m_maxParam )
                    {
                        err = Bioloid::ERROR_NONE;
                    }
                    else
                    {
                        err = Bioloid::ERROR_TOO_MUCH_DATA;
                    }
                }
                else
                {
                    // CRC failed

                    err = Bioloid::ERROR_CHECKSUM;
                    LogError( "Rcvd Checksum: 0x%02x Expecting: 0x%02x\n",
                              ch, m_checksum );
                }
                nextState = STATE_IDLE;
                break;
            }

            m_checksum += ch;
            if ( m_paramIdx < m_maxParam )
            {
                m_param[ m_paramIdx ] = ch;
            }
            m_paramIdx++;
            break;
        }
    }

    m_state = nextState;

    return err;
}

