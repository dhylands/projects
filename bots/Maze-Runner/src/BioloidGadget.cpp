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
 *  @file   BioloidGadget.cpp
 *
 *  @brief  Base class for implementing a bioloid gadget.
 *
 *  @details   This class takes care of managing the control table. Each
 *              time a control table entry is changed, the ControlTableModified
 *  virtual function is called.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "BioloidGadget.h"

#include "Bioloid.h"

#include "Log.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

BioloidGadget::BioloidGadget() {
}

void BioloidGadget::ProcessBytes(uint8_t *buf, size_t numBytes) {
    for (size_t i = 0; i < numBytes; i++) {
        Bioloid::Error err = this->m_packet.ProcessChar(buf[i]);
        if (err == Bioloid::Error::NOT_DONE) {
            continue;
        }
        if (err != Bioloid::Error::NONE) {
            LogError("Error parsing a packet: %d\n", err);
            continue;
        }
        Log("Parsed a packet for ID: %u\n", this->m_packet.ID());
    }
}

