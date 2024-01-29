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
 *   @file   BioloidPacket.h
 *
 *   @brief  Contains the definitions for a bioloid packet.
 *
 ****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

#include "Config.h"
#include "Bioloid.h"

/**
 * @addtogroup bioloid
 * @{
 */

namespace Bioloid {

//! Class for defining a bioloid packet.
class Packet {
 public:
    Packet();
    Packet( void* data, uint8_t maxData);
    ~Packet();

    //------------------------------------------------------------------------
    // Accessor functions which allow portions of the packet to be retrieved.

    //! @brief Returns the ID contained within the packet.
    //! @return The ID contained in the packet.
    Bioloid::ID ID() { return m_id; }

    //! @brief Returns the length of the packet in bytes.
    //! @return the length of the packet in bytes.
    uint8_t Length() { return m_length; }

    //! @brief Returns the command contained within a request packet.
    //! @return the  command contained within a request packet.
    Bioloid::Command Command() { return as_Command(m_cmd); }

    //! @brief Returns the Error Code contained within a response packet.
    //! @return the Error Code contained within a response packet.
    Bioloid::Error ErrorCode() { return (Bioloid::Error)m_cmd; }

    //! @brief Returns the checksum of the packet.
    //! @return the checksum of the packet.
    uint8_t CheckSum() { return m_checksum; }

    Bioloid::Error ProcessChar(uint8_t ch);

 private:
    enum class State {
        IDLE,          ///< We're waiting for the beginning of the packet
        FF_1ST_RCVD,   ///< We've received the 1st 0xFF
        FF_2ND_RCVD,   ///< We've received the 2nd 0xFF
        ID_RCVD,       ///< We've received the ID
        LENGTH_RCVD,   ///< We've received the length
        COMMAND_RCVD,  ///< We've received the command
    };

    State m_state;
    uint8_t m_paramIdx;

    Bioloid::ID m_id;
    uint8_t m_length;
    uint8_t m_cmd;  // Error code for a status packet

    uint8_t* m_param;
    uint8_t m_maxParam;

    uint8_t m_checksum;
};

}  // namespace Bioloid

/** @} */
