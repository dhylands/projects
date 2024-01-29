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
 *   @file   BioloidGadget.h
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

//! Base class to implement a bioloid gadget.
class Gadget {
 public:
    Gadget(Bioloid::ID id, uint8_t numControlTableEntries);
    virtual ~Gadget();

    virtual void InitializeControlTable();

    void ProcessBytes(uint8_t* buf, size_t numBytes);

 protected:
    virtual void ReadEEPROMTable();
    virtual void ResetEEPROMValue();
    virtual void WriteEEPROMTable();

    virtual void ProcessPacket();

    virtual void delay_us(uint32_t usec) = 0;
    virtual void SendByte(uint8_t byte) = 0;

    void SendResponse(Error errorCode, bool forceResponse);

 private:
    Bioloid::ID m_id;
    uint8_t m_numControlTableEntries;
    uint8_t m_numEEPROMEntries;
    uint8_t* m_controlTable;

    Packet m_packet;
};

}  // namespace Bioloid
