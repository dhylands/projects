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
 *   @file   BioloidDevice.cpp
 *
 *   @brief  This file implements the BioloidDevice class, which is an
 *           abstract base class for devices on the bioloid bus.
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "BioloidDevice.h"

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup bioloid
 * @{
 */

namespace Bioloid {

//***************************************************************************
/**
 *   Default Constructor
 */

Device::Device() : m_bus(NULL), m_id(ID::INVALID) {}

//***************************************************************************
/**
 *   Normal Constructor
 */

Device::Device(Bus* bus, Bioloid::ID id) : m_bus(bus), m_id(id) {}

//***************************************************************************
/**
 *   Destructor
 *
 *   virtual
 */

Device::~Device() {}

//***************************************************************************
/**
 *   Pings the device and waits for a status packet.
 */

Error Device::Ping() {
    Packet pkt;

    SendPing();
    if (!m_bus->ReadStatusPacket(&pkt)) {
        return Error::TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
 *   Reads data from the device's control table
 */

Error Device::Read(uint8_t offset, void* data, uint8_t numBytes) {
    Packet pkt(data, numBytes);

    SendRead(offset, numBytes);
    if (!m_bus->ReadStatusPacket(&pkt)) {
        return Error::TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
 *   Reads an 8 bit register.
 */

Error Device::Read(uint8_t offset, uint8_t* val) {
    return Read(offset, val, sizeof(*val));
}

//***************************************************************************
/**
 *   Reads an 16 bit register.
 */

Error Device::Read(uint8_t offset, uint16_t* val) {
    Error rc;

    uint8_t byteVal[2];

    if ((rc = Read(offset, byteVal, sizeof(byteVal))) == Error::NONE) {
        *val = (uint16_t)byteVal[0] | ((uint16_t)byteVal[1] << 8);
    }
    return rc;
}

//***************************************************************************
/**
 *   Reset the control table to factory defaults.
 */

Error Device::Reset() {
    Packet pkt;

    SendReset();

    if (m_id == ID::BROADCAST) {
        return Error::NONE;
    }

    if (!m_bus->ReadStatusPacket(&pkt)) {
        return Error::TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
 *   Sends a ping packet to this device
 */

void Device::SendPing() {
    BLD_BUS_LOG("Sending PING to ID %d\n", m_id);

    m_bus->SendCmdHeader(m_id, 0, Command::PING);
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Sends a request to read data from the devices control table
 */

void Device::SendRead(uint8_t offset, uint8_t numBytes) {
    BLD_BUS_LOG("Sending READ to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes);

    m_bus->SendCmdHeader(m_id, 2, Command::READ);
    m_bus->SendByte(offset);
    m_bus->SendByte(numBytes);
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Sends some data to write into the control table.
 */

void Device::SendWrite(uint8_t offset, const void* data, uint8_t numBytes) {
    BLD_BUS_LOG("Sending WRITE to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes);

    m_bus->SendCmdHeader(m_id, numBytes + 1, Command::WRITE);
    m_bus->SendByte(offset);
    m_bus->SendData(numBytes, data);
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Sends some data to write into the control table. The write into the
 *   control table will be deferred until the ACTION command is sent.
 */

void Device::SendDeferredWrite(uint8_t offset, const void* data, uint8_t numBytes) {
    BLD_BUS_LOG("Sending REG_WRITE to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes);

    m_bus->SendCmdHeader(m_id, numBytes + 1, Command::REG_WRITE);
    m_bus->SendByte(offset);
    m_bus->SendData(numBytes, data);
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Sends a commands to reset the control table to factory defaults.
 */

void Device::SendReset() {
    BLD_BUS_LOG("Sending RESET to ID %d\n", m_id);

    m_bus->SendCmdHeader(m_id, 0, Command::RESET);
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
 *   Writes some data into the control table, and returns the result.
 */

Error Device::Write(uint8_t offset, const void* data, uint8_t numBytes) {
    Packet pkt;

    SendWrite(offset, data, numBytes);

    if (m_id == ID::BROADCAST) {
        return Error::NONE;
    }

    if (!m_bus->ReadStatusPacket(&pkt)) {
        return Error::TIMEOUT;
    }

    return pkt.ErrorCode();
}

}  // namespace Bioloid

/** @} */
