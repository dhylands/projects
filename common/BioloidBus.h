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
#include "BioloidPacket.h"

namespace Bioloid {

class Device;

/**
 * @addtogroup bioloid
 * @{
 */

#define BLD_BUS_LOG(fmt, args...) \
    do {                          \
        if (Bus::m_log) {         \
            Log(fmt, ##args);     \
        }                         \
    } while (0)

class Bus {
 public:
    //------------------------------------------------------------------------
    // Default constructor

    Bus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~Bus();

    //------------------------------------------------------------------------
    // Sends a byte. This will automatically accumulate the byte into
    // the checksum

    virtual void SendByte(uint8_t data);

    //------------------------------------------------------------------------
    // Reads a byte. This function returns true if a character was read, and
    // returns false if no character is received within the designated
    // timeout.
    //
    // The max Return Delay time is 254 x 2 usec = 508 usec (the default
    // is 500 usec). This represents the minimum time between receiving a
    // packet and sending the response.

    virtual bool ReadByte(uint8_t* ch) = 0;


    //------------------------------------------------------------------------
    // Reads a packet. This function returns true
    bool ReadPacket(Packet* pkt);

    //------------------------------------------------------------------------
    // Send the checksum. Since the checksum byte is the last byte of the
    // packet, this function is made virtual to allow bus drivers to
    // buffer the packet bytes until the entire packet is ready to send.

    virtual void SendCheckSum();

    //------------------------------------------------------------------------
    // Sends 'len' bytes

    void SendData(uint8_t len, const void* data);

    //------------------------------------------------------------------------
    // Sets debug option which causes packet data to be printed.

    void SetShowPackets(bool showPackets) { m_showPackets = showPackets; }

    static bool m_log;

 protected:
    //------------------------------------------------------------------------
    // Adds a byte to the buffer of data to send.

    void BufferByte(uint8_t data);

    //------------------------------------------------------------------------
    // Writes all of the buffered bytes to the serial port.

    void WriteBuffer();

    //------------------------------------------------------------------------
    // Core portion of WriteBuffer which writes the data

    virtual void WriteBufferedData(void* data, size_t numBytes) = 0;

    //------------------------------------------------------------------------

    uint8_t m_checksum;
    bool m_showPackets;
    size_t m_dataBytes;
    uint8_t m_data[128];

 private:
    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    Bus(const Bus& copy);
    Bus& operator=(const Bus& rhs);
};
}  // namespace Bioloid
/** @} */
