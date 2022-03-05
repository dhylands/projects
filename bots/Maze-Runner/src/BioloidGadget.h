/****************************************************************************
 *
 *   Copyright (c) 2022 Dave Hylands     <dhylands@gmail.com>
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
 *   @file   BioloidGadget
 *
 *   @brief  Base class for implementing a bioloid gadget, which is a device
 *           on the bioloid bus.
 *
 *****************************************************************************/

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "BioloidPacket.h"

// ---- Include Files -------------------------------------------------------

namespace Bioloid {

class Gadget {
 public:
    // These are pre-defined table entries that shouldn't be used  for other purposes.
    enum class Control : uint8_t {
        MODEL_NUM_L = 0x00,
        MODEL_NUM_H = 0x01,
        FIRMWARE_VERSION = 0x02,
        ID = 0x03,
        BAUD_RATE = 0x04,
        RETURN_DELAY_TIME = 0x05,
        STATUS_RETURN_LEVEL = 0x10,

        LED = 0x19
    };
    static inline uint8_t as_uint8_t(Control val) { return static_cast<uint8_t>(val); }

    enum class StatusReturnLevel : uint8_t {
        NONE = 0,       // Don't respond
        READ_DATA = 1,  // respond only to READ_DATA
        ALL = 2,        // Respond to everything
    };
    static inline uint8_t as_uint8_t(StatusReturnLevel val) { return static_cast<uint8_t>(val); }

    Gadget(Bioloid::ID id, uint8_t numControlTableEntries, uint8_t numEEPROMEntries);
    virtual ~Gadget();

    virtual void InitializeControlTable();

    void ProcessBytes(uint8_t* buf, size_t numBytes);

 protected:
    virtual void ReadEEPROMTable();
    virtual void ResetEEPROMValue();
    virtual void WriteEEPROMTable();

    virtual void ProcessPacket();

    void SendResponse(Error errorCode, bool forceResponse);

 private:
    Bioloid::ID m_id;
    uint8_t m_numControlTableEntries;
    uint8_t m_numEEPROMEntries;
    uint8_t* m_controlTable;

    Packet m_packet;
};

}  // namespace Bioloid
