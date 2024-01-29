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
 *   @file   EmuGadget.h
 *
 *   @brief  Base class for implementing a bioloid gadget, which is a device
 *           on the bioloid bus.
 *
 *****************************************************************************/

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "BioloidGadget.h"

// ---- Include Files -------------------------------------------------------

namespace Emu {

//! Gadget used to emulate a bioloid gadget.
class Gadget : public Bioloid::Gadget {
 public:
    Gadget(Bioloid::ID id, uint8_t numControlTableEntries, uint8_t numEEPROMEntries)
        : Bioloid::Gadget(id, numControlTableEntries, numEEPROMEntries) {}

 protected:
    void delay_us(uint32_t usec) override;
    void SendByte(uint8_t byte) override;
};

}  // namespace Emu
