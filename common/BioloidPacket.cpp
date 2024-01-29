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

//! @addtogroup bioloid
//! @{

namespace Bioloid {

//***************************************************************************
/**
 *   Constructor
 */
Packet::Packet() : m_state(Packet::State::IDLE), m_param(NULL), m_maxParam(0) {}

//***************************************************************************
/**
 *   Constructor where storage for parameter data is specified.
 */
Packet::Packet(
    void* data,      //!< [out] Place to store the packet bytes.
    uint8_t maxData  //!< [in] Size of the packet buffer.
)
    : m_state(Packet::State::IDLE), m_param(static_cast<uint8_t*>(data)), m_maxParam(maxData) {}

//***************************************************************************
/**
 *   Destructor
 */
Packet::~Packet() {}

//***************************************************************************
/**
 *  @brief  Runs a single character through the state machine.
 *  @details Once a packet has been parsed successfully, the PacketReceived()
 *           callback is called.
 *  @return Bioloid::Error::NONE if a packet was parsed successfully.
 *  @return Bioloid::Error::ERROR_NOT_DONE if more data is needed.
 *  @return Bioloid::Error::TOO_MUCH_DATA is the packet doesn't fit in the buffer.
 *  @return Bioloid::Error::CHECKSUM if a checksum error was parsed.
 */
Bioloid::Error Packet::ProcessChar(
    uint8_t ch  //!< [in] Character to parse,
) {
    State nextState = m_state;
    Bioloid::Error err = Bioloid::Error::NOT_DONE;

    switch (nextState) {
        case State::IDLE:  // We're waiting for the beginning of the packet (0xFF)
        {
            if (ch == 0xFF) {
                nextState = State::FF_1ST_RCVD;
            }
            break;
        }

        case State::FF_1ST_RCVD:  // We've received the 1st 0xFF
        {
            if (ch == 0xFF) {
                nextState = State::FF_2ND_RCVD;
            } else {
                nextState = State::IDLE;
            }
            break;
        }

        case State::FF_2ND_RCVD:  // We've received the 2nd 0xFF, ch is the ID
        {
            if (ch == 0xFF) {
                // 0xFF is invalid as an ID, so just stay in this state until we receive
                // a non-0xFF

                nextState = State::FF_2ND_RCVD;
                break;
            }
            m_id = Bioloid::as_ID(ch);
            m_checksum = ch;
            nextState = State::ID_RCVD;
            break;
        }

        case State::ID_RCVD:  // We've received the ID, ch is the length
        {
            m_length = ch;
            m_checksum += ch;
            nextState = State::LENGTH_RCVD;
            break;
        }

        case State::LENGTH_RCVD:  // We've received the length, ch is the command
        {
            m_cmd = ch;
            m_checksum += ch;
            m_paramIdx = 0;
            nextState = State::COMMAND_RCVD;
            break;
        }

            // NOTE: In the future, we should decode the SYNC_WRITE
            //       packet so that we only need to keep the portion that
            //       belongs to our ID

        case State::COMMAND_RCVD:  // We've received the command, ch is a param byte or checksum
        {
            if ((m_paramIdx + 2) >= m_length) {
                // ch is the Checksum

                m_checksum = ~m_checksum;

                if (m_checksum == ch) {
                    if (m_paramIdx <= m_maxParam) {
                        err = Bioloid::Error::NONE;
                    } else {
                        err = Bioloid::Error::TOO_MUCH_DATA;
                    }
                } else {
                    // CRC failed

                    err = Bioloid::Error::CHECKSUM;
                    LogError("Rcvd Checksum: 0x%02x Expecting: 0x%02x\n", ch, m_checksum);
                }
                nextState = State::IDLE;
                break;
            }

            m_checksum += ch;
            if (m_paramIdx < m_maxParam) {
                m_param[m_paramIdx] = ch;
            }
            m_paramIdx++;
            break;
        }
    }

    m_state = nextState;

    return err;
}

}  // namespace Bioloid

//! @}
