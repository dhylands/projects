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

class BioloidGadget {
public:
    BioloidGadget();
    void ProcessBytes(uint8_t *buf, size_t numBytes);

private:
    BioloidPacket m_packet;
};
