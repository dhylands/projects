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

#include "Log.h"
#include "Bioloid.h"
#include "BioloidBus.h"
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

Bus::Bus() : m_showPackets(false), m_dataBytes(0) {}

//***************************************************************************
/**
 *   Destructor
 *
 *   virtual
 */

Bus::~Bus() {}

//***************************************************************************
/**
 *   Adds a byte to the buffer of data to send.
 */

void Bus::BufferByte(uint8_t data) {
    m_data[m_dataBytes++] = data;

    if (m_dataBytes >= sizeof(m_data)) {
        WriteBuffer();
    }
}

//***************************************************************************
/**
 *   Reads a packet. Returns true if a packet was read successfully,
 *   false if a timeout or error occurred. This function will block
 *   until a packet is received.
 *
 *   virtual
 */
bool Bus::ReadPacket(Packet* pkt) {
    Bioloid::Error err;

    m_dataBytes = 0;

    do {
        uint8_t ch;

        if (!ReadByte(&ch)) {
            // LogError( "Bus::ReadStatusPacket: ReadByte returned false\n");
            return false;
        }

        if (m_dataBytes < sizeof(m_data)) {
            m_data[m_dataBytes++] = ch;
        }
        err = pkt->ProcessChar(ch);
    } while (err == Bioloid::Error::NOT_DONE);

    if (err == Bioloid::Error::NONE) {
        if (m_showPackets) {
            if (m_dataBytes > 0) {
                DumpMem("R", 0, m_data, m_dataBytes);
            }
        }
    } else {
        Log("Bus::ReadPacket err = %d\n", err);
    }

    return err == Bioloid::Error::NONE;
}

//***************************************************************************
/**
 *   Send the checksum. Since the checksum byte is the last byte of the
 *   packet, this function is made virtual to allow bus drivers to
 *   buffer the packet bytes until the entire packet is ready to send.
 *
 *   virtual
 */

void Bus::SendCheckSum() {
    SendByte(~m_checksum);

    WriteBuffer();
}

//***************************************************************************
/**
 *   Sends 'len' bytes, returning the sum of the bytes for easy accumulation
 *   into the checksum)
 */

void Bus::SendData(uint8_t len, const void* voidData) {
    const uint8_t* data = static_cast<const uint8_t*>(voidData);

    while (len-- > 0) {
        SendByte(*data++);
    }
}

//***************************************************************************
/**
 *   Writes all of the buffered bytes to the serial port.
 */

void Bus::WriteBuffer() {
    if (m_showPackets) {
        DumpMem("W", 0, m_data, m_dataBytes);
    }

    WriteBufferedData(m_data, m_dataBytes);

    m_dataBytes = 0;
}

}  // namespace Bioloid

/** @} */
