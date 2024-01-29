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
 *   @file   BioloidBus.cpp
 *
 *   @brief  This file implements the Bioloid::Bus class, which is an
 *           abstract base class for the hardware which actually talks
 *   to the bioloid bus. This hardware usually takes the form of a UART.
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "BioloidHost.h"

#include "Log.h"
#include "Bioloid.h"
#include "BioloidDevice.h"
#include "DumpMem.h"

// ---- Public Variables ----------------------------------------------------

namespace Bioloid {

bool Bus::m_log = false;

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup bioloid
 * @{
 */

//***************************************************************************
/**
 *   Constructor
 */

Host::Host(Bus* bus) : m_bus(bus) {}

//***************************************************************************
/**
 *   Destructor
 *
 *   virtual
 */

Host::~Host() {}

//***************************************************************************
/**
 *   Reads a packet. Returns true if a packet was read successfully,
 *   false if a timeout or error occurred.
 *
 *   virtual
 */

bool Host::ReadStatusPacket(Packet* pkt) {
    return m_bus->ReadPacket(pkt);
}

//***************************************************************************
/**
 *   Scans the bus, calling the passed callback for each device
 *   ID which responds.
 */

bool Host::Scan(bool (*devFound)(Bus* bus, Device* dev), ID startId, uint8_t numIds) {
    uint8_t id;
    uint8_t endId;
    Device scanDev;
    bool someDevFound = false;

    endId = as_uint8_t(startId) + numIds - 1;
    if (endId >= as_uint8_t(ID::BROADCAST)) {
        endId = as_uint8_t(ID::BROADCAST) - 1;
    }

    for (id = as_uint8_t(startId); id <= endId; id++) {
        scanDev.SetBusAndID(this->m_bus, as_ID(id));

        if (scanDev.Ping() == Bioloid::Error::NONE) {
            someDevFound = true;

            if (!devFound(this->m_bus, &scanDev)) {
                break;
            }
        }
    }

    return someDevFound;
}

//***************************************************************************
/**
 *   Broadcasts an action packet to all of the devices on the bus.
 *   This causes all of the devices to perform their deferred writes
 *   at the same time.
 */

void Host::SendAction() {
    BLD_BUS_LOG("Sending ACTION\n");

    SendCmdHeader(ID::BROADCAST, 0, Bioloid::Command::ACTION);
    this->m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Sends the command header, which is common to all of the commands.
 *   2 is added to paramLen (to cover the length and cmd bytes). This
 *   way the caller is only responsible for figuring out how many extra
 *   parameter bytes are being sent.
 *
 *   virtual
 */

void Host::SendCmdHeader(Bioloid::ID id, uint8_t paramLen, Bioloid::Command cmd) {
    // May need to call a function on m_bus to reset m_datBytes

    m_dataBytes = 0;

    this->m_bus->SendByte(0xff);
    this->m_bus->SendByte(0xff);

    // May need a function to reset checksum

    // Maybe SendCmdHeader and SendRspHeader are really the same

    m_checksum = 0;

    this->m_bus->SendByte(as_uint8_t(id));
    this->m_bus->SendByte(paramLen + 2);
    this->m_bus->SendByte(static_cast<uint8_t>(cmd));
}

}  // namespace Bioloid

/** @} */
