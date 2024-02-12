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
 *   @file   BioloidBus.h
 *
 *   @brief  Contains the definitions for a bioloid bus. Essentially
 *           there is a bus for each UART with bioloid devices attached.
 *
 ****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

#include "Bioloid.h"

/**
 * @addtogroup bioloid
 * @{
 */

namespace Bioloid {

class Packet {
 public:
    //------------------------------------------------------------------------
    // Default constructor

    Packet();

    //------------------------------------------------------------------------
    // Constructor where the storage for parameter data is specified.

    Packet(void* data, uint8_t maxData);

    //------------------------------------------------------------------------
    // Destructor

    ~Packet();

    //------------------------------------------------------------------------
    // Accessor functions which allow portions of the packet to be retrieved.

    Bioloid::ID ID() { return m_id; }
    uint8_t Length() { return m_length; }
    Bioloid::Command Command() { return static_cast<Bioloid::Command>(m_cmd); }
    Bioloid::Error ErrorCode() { return static_cast<Bioloid::Error>(m_cmd); }
    uint8_t CheckSum() { return m_checksum; }

    //------------------------------------------------------------------------
    // Runs a single character through the state machine.
    //
    // ProcessChar returns Bioloid::ERROR_NOT_DONE if the packet is incomplete.
    // If the packet was parsed successfully, then Bioloid::ERROR_NONE is
    // returned. If an error is detected that Bioloid::ERROR_CHECKSUM is
    // returned.

    Bioloid::Error ProcessChar(uint8_t ch);

    size_t Data(size_t maxLen, uint8_t* data);

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
