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
 *  @file   EmuGadget.cpp
 *
 *  @brief  Class for implementing an emulated bioloid gadget.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "EmuControlTable.h"

#include "BioloidGadget.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

namespace Emu {

Gadget::Gadget(Bioloid::ID id, uint8_t numControlTableEntries, uint8_t numEEPROMEntries)
    : Bioloid::Gadget(id, numControlTableEntries, numEEPROMEntries) {}

// virtual
~Gadget::Gadget() {}

// virtual
void Gadget::InitializeControlTable() {}

// virtual
void Gadget::ReadEEPROMTable() {}

// virtual
void Gadget::ResetEEPROMValue() {}

// virtual
void Gadget::WriteEEPROMTable() {}

// virtual
void Gadget::delay_us(uint32_t usec) {}

// virtual
void Gadget::SendByte(uint8_t byte) {}

}  // namespace Emu
