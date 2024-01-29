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
#include "BioloidBus.h"
#include "BioloidPacket.h"

namespace Bioloid {

class Device;

/**
 * @addtogroup bioloid
 * @{
 */

class Host {
 public:
    //------------------------------------------------------------------------
    // Default constructor

    Host(Bus* bus);

    //------------------------------------------------------------------------
    // Destructor

    virtual ~Host();

    //------------------------------------------------------------------------
    // Scans the bus, calling the passed callback for each device
    // ID which responds.

    bool Scan(bool (*devFound)(Bus* bus, Device* dev), Bioloid::ID startId, uint8_t numIds);

    //------------------------------------------------------------------------
    // Broadcasts an action packet to all of the devices on the bus.
    // This causes all of the devices to perform their deferred writes
    // at the same time.

    void SendAction();

    //------------------------------------------------------------------------
    // Sends the command header, which is common to all of the commands.
    // 2 is added to paramLen (to cover the length and cmd bytes). This
    // way the caller is only responsible for figuring out how many extra
    // parameter bytes are being sent.

    virtual void SendCmdHeader(Bioloid::ID id, uint8_t paramLen, Bioloid::Command cmd);

    //------------------------------------------------------------------------
    // Reads a packet. Returns true if a packet was read successfully,
    // false if a timeout or error occurred.

    virtual bool ReadStatusPacket(Packet* pkt);

 private:
    Bus* m_bus;

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    Host(const Host& copy);
    Host& operator=(const Host& rhs);
};
}  // namespace Bioloid
/** @} */
