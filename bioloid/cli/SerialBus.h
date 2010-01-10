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

#if !defined( SERIALBUS_H )
#define SERIALBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidBus.h"
#include "SerialPort.h"

/**
 * @addtogroup bioloid
 * @{
 */

class SerialBus : public BioloidBus
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    SerialBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~SerialBus();

    //------------------------------------------------------------------------
    // Sets the serial port which will be used for communications

    void SetSerialPort( SerialPort *serPort );

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

    SerialPort *m_serialPort;

    bool        m_debug;

    int         m_fd;
    size_t      m_dataBytes;
    uint8_t     m_data[ 128 ];

};

/** @} */

#endif /* SERIALBUS_H */

