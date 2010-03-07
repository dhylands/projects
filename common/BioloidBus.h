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

#if !defined( BIOLOIDBUS_H )
#define BIOLOIDBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdint.h>
#include "Bioloid.h"
#include "BioloidPacket.h"

class BioloidDevice;

/**
 * @addtogroup bioloid
 * @{
 */

#define BLD_BUS_LOG( fmt, args... ) do { if ( BioloidBus::m_log ) { Log( fmt, ##args ); }} while (0)

class BioloidBus
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    BioloidBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~BioloidBus();

    //------------------------------------------------------------------------
    // Scans the bus, calling the passed callback for each device
    // ID which responds.

    bool Scan( bool (*devFound)( BioloidBus *bus, BioloidDevice *dev ), uint8_t startId, uint8_t numIds );

    //------------------------------------------------------------------------
    // Broadcasts an action packet to all of the devices on the bus.
    // This causes all of the devices to perform their deferred writes
    // at the same time.

    void SendAction();

    //------------------------------------------------------------------------
    // Sends a byte. This will automatically accumulate the byte into 
    // the checksum

    virtual void SendByte( uint8_t data );

    //------------------------------------------------------------------------
    // Reads a byte. This function returns true if a character was read, and
    // returns false if no character is received within the designated
    // timeout.
    // 
    // The max Return Delay time is 254 x 2 usec = 508 usec (the default
    // is 500 usec). This represents the minimum time between receiving a
    // packet and sending the response.

    virtual bool ReadByte( uint8_t *ch ) = 0;

    //------------------------------------------------------------------------
    // Send the checksum. Since the checksum byte is the last byte of the
    // packet, this function is made virtual to allow bus drivers to
    // buffer the packet bytes until the entire packet is ready to send.

    virtual void SendCheckSum();

    //------------------------------------------------------------------------
    // Sends 'len' bytes

    void SendData( uint8_t len, const void *data );

    //------------------------------------------------------------------------
    // Sends the command header, which is common to all of the commands.
    // 2 is added to paramLen (to cover the length and cmd bytes). This
    // way the caller is only responsible for figuring out how many extra
    // parameter bytes are being sent.

    virtual void SendCmdHeader( Bioloid::ID_t id, uint8_t paramLen, Bioloid::Command cmd );

    //------------------------------------------------------------------------
    // Reads a packet. Returns true if a packet was read successfully,
    // false if a timeout or error occurred.

    virtual bool ReadStatusPacket( BioloidPacket *pkt );

    //------------------------------------------------------------------------
    // Sets debug option which causes packet data to be printed.

    void SetShowPackets( bool showPackets )
    {
        m_showPackets = showPackets;
    }

    static  bool    m_log;

protected:

    //------------------------------------------------------------------------
    // Adds a byte to the buffer of data to send.

    void BufferByte( uint8_t data );

    //------------------------------------------------------------------------
    // Writes all of the buffered bytes to the serial port.

    void WriteBuffer();

    //------------------------------------------------------------------------
    // Core portion of WriteBuffer which writes the data

    virtual void WriteBufferedData( void *data, size_t numBytes ) = 0;

    //------------------------------------------------------------------------

    uint8_t     m_checksum;
    bool        m_showPackets;

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    BioloidBus( const BioloidBus & copy );
    BioloidBus &operator =( const BioloidBus &rhs );

    //------------------------------------------------------------------------

    size_t      m_dataBytes;
    uint8_t     m_data[ 128 ];
};

/** @} */

#endif /* BIOLOIDBUS_H */

