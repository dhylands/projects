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
*   @file   SerialBus.cpp
*
*   @brief  This file implements the SerialBus class, which talks to the
*           bioloid bus using a serial port, typically through an FTDI
*           USB-to-serial adapter.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "DumpMem.h"
#include "SerialBus.h"
#include "Error.h"

// ---- Public Variables ----------------------------------------------------
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

SerialBus::SerialBus()
    : m_serialPort( NULL ),
      m_debug( false ),
      m_dataBytes( 0 )
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

SerialBus::~SerialBus()
{
}

//***************************************************************************
/**
*   Adds a byte to the buffer of data to send.
*/

void SerialBus::BufferByte( uint8_t data )
{
    m_data[ m_dataBytes++ ] = data;

    if ( m_dataBytes >= sizeof( m_data ))
    {
        WriteBuffer();
    }
}

//***************************************************************************
/**
*   Writes all of the buffered bytes to the serial port.
*/

void SerialBus::WriteBuffer()
{
    size_t  bytesWritten;

    if ( m_debug )
    {
        DumpMem( "W", 0, m_data, m_dataBytes );
    }

    if (( bytesWritten = m_serialPort->Write( m_data, m_dataBytes )) != m_dataBytes )
    {
        LogError( "Error writing %d bytes to serial port", m_dataBytes );
    }

    m_dataBytes = 0;
}

//***************************************************************************
/**
*   Reads a byte.
*
*   virtual
*/

bool SerialBus::ReadByte( uint8_t *ch )
{
    bool rc;

    rc = m_serialPort->Read( ch, 1 ) == 1;

    if ( rc )
    {
        if ( m_dataBytes < sizeof( m_data ))
        {
            m_data[ m_dataBytes++ ] = *ch;
        }
    }

    return rc;
}

//***************************************************************************
/**
*   Reads a packet. Returns true if a packet was read successfully,
*   false if a timeout or error occurred.
*
*   virtual
*/

bool SerialBus::ReadStatusPacket( BioloidPacket *pkt )
{
    bool    rc;

    m_dataBytes = 0;

    rc = BioloidBus::ReadStatusPacket( pkt );

    if ( m_debug )
    {
        if ( m_dataBytes > 0 )
        {
            DumpMem( "R", 0, m_data, m_dataBytes );
        }
#if 0
        if ( !rc )
        {
            LogError( "Packet Error\n" );
        }
#endif
    }

    return rc;
}

//***************************************************************************
/**
*   Sends a byte. This will automatically accumulate the byte into 
*   the checksum)
*
*   virtual
*/

void SerialBus::SendByte( uint8_t data )
{
    m_checksum += data;

    BufferByte( data );
}

//***************************************************************************
/**
*   Send the checksum. Since the checksum byte is the last byte of the
*   packet, this function is made virtual to allow bus drivers to
*   buffer the packet bytes until the entire packet is ready to send.
*
*   virtual
*/

void SerialBus::SendCheckSum()
{
    SendByte( ~m_checksum );

    WriteBuffer();
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

void SerialBus::SendCmdHeader
(
    Bioloid::ID_t       id, 
    uint8_t             paramLen,
    Bioloid::Command    cmd
)
{
    m_dataBytes = 0;

    BioloidBus::SendCmdHeader( id, paramLen, cmd );
}

//***************************************************************************
/**
*   Sets the serial port that will be used for talking with the bioloid
*   devices.
*/

void SerialBus::SetSerialPort( SerialPort *serPort )
{
    m_serialPort = serPort;

    // 15 gives intermittent failures on my Dell laptop. 50 seems
    // to work reliably

    m_serialPort->SetTimeout( 50 );
}

/** @} */

