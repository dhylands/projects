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
*   @brief  This file implements the BioloidBus class, which is an 
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

bool    BioloidBus::m_log   = false;

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

BioloidBus::BioloidBus()
    : m_showPackets( false ),
      m_dataBytes( 0 )
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

BioloidBus::~BioloidBus()
{
}

//***************************************************************************
/**
*   Adds a byte to the buffer of data to send.
*/

void BioloidBus::BufferByte( uint8_t data )
{
    m_data[ m_dataBytes++ ] = data;

    if ( m_dataBytes >= sizeof( m_data ))
    {
        WriteBuffer();
    }
}

//***************************************************************************
/**
*   Reads a packet. Returns true if a packet was read successfully,
*   false if a timeout or error occurred.
*
*   virtual
*/

bool BioloidBus::ReadStatusPacket( BioloidPacket *pkt )
{
    Bioloid::Error err;

    m_dataBytes = 0;

    do
    {
        uint8_t         ch;

        if ( !ReadByte( &ch ))
        {
            //LogError( "BioloidBus::ReadStatusPacket: ReadByte returned false\n");
            return false;
        }

        if ( m_dataBytes < sizeof( m_data ))
        {
            m_data[ m_dataBytes++ ] = ch;
        }
        err = pkt->ProcessChar( ch );

    } while ( err == Bioloid::ERROR_NOT_DONE );

    if ( err == Bioloid::ERROR_NONE )
    {
        if ( m_showPackets )
        {
            if ( m_dataBytes > 0 )
            {
                DumpMem( "R", 0, m_data, m_dataBytes );
            }
        }
    }
    else
    {
        Log( "BioloidBus::ReadStatusPacket err = %d\n", err );
    }

    return err == Bioloid::ERROR_NONE;
}

//***************************************************************************
/**
*   Scans the bus, calling the passed callback for each device
*   ID which responds.
*/

bool BioloidBus::Scan( bool (*devFound)( BioloidBus *bus, BioloidDevice *dev ), uint8_t startId, uint8_t numIds )
{
    Bioloid::ID_t   id;
    Bioloid::ID_t   endId;
    BioloidDevice   scanDev;
    bool            someDevFound = false;

    endId = startId + numIds - 1;
    if ( endId >= Bioloid::BROADCAST_ID )
    {
        endId = Bioloid::BROADCAST_ID - 1;
    }

    for ( id = startId; id <= endId; id++ )
    {
        scanDev.SetBusAndID( this, id );

        if ( scanDev.Ping() == Bioloid::ERROR_NONE )
        {
            someDevFound = true;

            if ( !devFound( this, &scanDev ))
            {
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

void BioloidBus::SendAction()
{
    BLD_BUS_LOG( "Sending ACTION\n" );

    SendCmdHeader( Bioloid::BROADCAST_ID, 0, Bioloid::CMD_ACTION );
    SendCheckSum();
}

//***************************************************************************
/**
*   Sends a byte. This will automatically accumulate the byte into 
*   the checksum)
*
*   virtual
*/

void BioloidBus::SendByte( uint8_t data )
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

void BioloidBus::SendCheckSum()
{
    SendByte( ~m_checksum );

    WriteBuffer();
}

//***************************************************************************
/**
*   Sends 'len' bytes, returning the sum of the bytes for easy accumulation
*   into the checksum) 
*/

void BioloidBus::SendData( uint8_t len, const void *voidData )
{
    const uint8_t *data = static_cast< const uint8_t *>( voidData );

    while ( len-- > 0 )
    {
        SendByte( *data++ );
    }
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

void BioloidBus::SendCmdHeader
(
    Bioloid::ID_t       id, 
    uint8_t             paramLen,
    Bioloid::Command    cmd
)
{
    m_dataBytes = 0;

    SendByte( 0xff );
    SendByte( 0xff );

    m_checksum = 0;

    SendByte( id );
    SendByte( paramLen + 2 );
    SendByte( cmd );
}

//***************************************************************************
/**
*   Writes all of the buffered bytes to the serial port.
*/

void BioloidBus::WriteBuffer()
{
    if ( m_showPackets )
    {
        DumpMem( "W", 0, m_data, m_dataBytes );
    }

    WriteBufferedData( m_data, m_dataBytes );

    m_dataBytes = 0;
}

/** @} */

