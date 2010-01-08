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
*   @file   SerialBus.h
*
*   @brief  Implements a bioloid bus using posix serial. This typically
*           assumes that an FTDI USB-to-serial adapter is being used
*   to do the RS-485 management.
*
****************************************************************************/

#if !defined( NETBUS_H )
#define NETBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidBus.h"

#if defined( WIN32 )
#include <windows.h>
#endif

/**
 * @addtogroup bioloid
 * @{
 */

class NetBus : public BioloidBus
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    NetBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~NetBus();

    //------------------------------------------------------------------------
    // Sets the serial port which will be used for communications

    bool Open( const char *hostStr );

    //------------------------------------------------------------------------
    // Reads a byte.

    virtual bool ReadByte( uint8_t *ch );

    //------------------------------------------------------------------------
    // Reads a packet. Returns true if a packet was read successfully,
    // false if a timeout or error occurred.

    virtual bool ReadStatusPacket( BioloidPacket *pkt );

    //------------------------------------------------------------------------
    // Sends a byte. This will automatically accumulate the byte into 
    // the checksum

    virtual void SendByte( uint8_t data );

    //------------------------------------------------------------------------
    //  Send the checksum. Since the checksum byte is the last byte of the
    //  packet, this function is made virtual to allow bus drivers to
    //  buffer the packet bytes until the entire packet is ready to send.

    virtual void SendCheckSum();

    //------------------------------------------------------------------------
    // Sends the command header, which is common to all of the commands.
    // 2 is added to paramLen (to cover the length and cmd bytes). This
    // way the caller is only responsible for figuring out how many extra
    // parameter bytes are being sent.

    virtual void SendCmdHeader( Bioloid::ID_t id, uint8_t paramLen, Bioloid::Command cmd );

    //------------------------------------------------------------------------
    // Sets the debug mode

    void SetDebug( bool debug ) { m_debug = debug; }

private:

    //------------------------------------------------------------------------
    // Adds a byte to the buffer of data to send.

    void BufferByte( uint8_t data );

    //------------------------------------------------------------------------
    // Writes all of the buffered bytes to the serial port.

    void WriteBuffer();

    //------------------------------------------------------------------------

    bool                m_debug;

    bool                m_initialized;

    int                 m_socket;       ///< Socket handle
    int                 m_dataBytes;
    uint8_t             m_data[ 128 ];
    unsigned short      m_defaultPort;  ///< Default Port number (if none specified)
    struct sockaddr_in  m_sockAddr;     ///< Socket address for the remote end
    char                m_connectionInfo[ 100 ]; ///< Describes the hostname etc.
};

/** @} */

#endif /* NETBUS_H */

