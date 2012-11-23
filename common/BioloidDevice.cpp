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

//***************************************************************************
/**
*   Default Constructor
*/

BioloidDevice::BioloidDevice()
    : m_bus( NULL ), m_id( Bioloid::INVALID_ID )
{
}

//***************************************************************************
/**
*   Normal Constructor
*/

BioloidDevice::BioloidDevice( BioloidBus *bus, Bioloid::ID_t id )
    : m_bus( bus ), m_id( id )
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

BioloidDevice::~BioloidDevice()
{
}

//***************************************************************************
/**
*   Pings the device and waits for a status packet.
*/

Bioloid::Error BioloidDevice::Ping()
{
    BioloidPacket   pkt;

    SendPing();
    if ( !m_bus->ReadStatusPacket( &pkt ))
    {
        return Bioloid::ERROR_TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
*   Reads data from the device's control table
*/

Bioloid::Error BioloidDevice::Read( uint8_t offset, void *data, uint8_t numBytes )
{
    BioloidPacket   pkt( data, numBytes );

    SendRead( offset, numBytes );
    if ( !m_bus->ReadStatusPacket( &pkt ))
    {
        return Bioloid::ERROR_TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
*   Reads an 8 bit register.
*/

Bioloid::Error BioloidDevice::Read( uint8_t offset, uint8_t *val )
{
    return Read( offset, val, sizeof( *val ));
}

//***************************************************************************
/**
*   Reads an 16 bit register.
*/

Bioloid::Error BioloidDevice::Read( uint8_t offset, uint16_t *val )
{
    Bioloid::Error  rc;

    uint8_t byteVal[2];

    if (( rc = Read( offset, byteVal, sizeof( byteVal ))) == Bioloid::ERROR_NONE )
    {
        *val = (uint16_t)byteVal[0] | ((uint16_t)byteVal[1] << 8 );
    }
    return rc;
}

//***************************************************************************
/**
*   Reset the control table to factory defaults.
*/

Bioloid::Error BioloidDevice::Reset()
{
    BioloidPacket   pkt;

    SendReset();

    if ( m_id == Bioloid::BROADCAST_ID )
    {
        return Bioloid::ERROR_NONE;
    }

    if ( !m_bus->ReadStatusPacket( &pkt ))
    {
        return Bioloid::ERROR_TIMEOUT;
    }

    return pkt.ErrorCode();
}

//***************************************************************************
/**
*   Sends a ping packet to this device
*/

void BioloidDevice::SendPing()
{
    BLD_BUS_LOG( "Sending PING to ID %d\n", m_id );

    m_bus->SendCmdHeader( m_id, 0, Bioloid::CMD_PING );
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
*   Sends a request to read data from the devices control table
*/

void BioloidDevice::SendRead( uint8_t offset, uint8_t numBytes )
{
    BLD_BUS_LOG( "Sending READ to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes );

    m_bus->SendCmdHeader( m_id, 2, Bioloid::CMD_READ );
    m_bus->SendByte( offset );
    m_bus->SendByte( numBytes );
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
*   Sends some data to write into the control table.
*/

void BioloidDevice::SendWrite( uint8_t offset, const void *data, uint8_t numBytes )
{
    BLD_BUS_LOG( "Sending WRITE to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes );

    m_bus->SendCmdHeader( m_id, numBytes + 1, Bioloid::CMD_WRITE );
    m_bus->SendByte( offset );
    m_bus->SendData( numBytes, data );
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
*   Sends some data to write into the control table. The write into the 
*   control table will be deferred until the ACTION command is sent.    
*/

void BioloidDevice::SendDeferredWrite( uint8_t offset, const void *data, uint8_t numBytes )
{
    BLD_BUS_LOG( "Sending REG_WRITE to ID %d, offset:%d numBytes:%d\n", m_id, offset, numBytes );

    m_bus->SendCmdHeader( m_id, numBytes + 1, Bioloid::CMD_REG_WRITE );
    m_bus->SendByte( offset );
    m_bus->SendData( numBytes, data );
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
*   Sends a commands to reset the control table to factory defaults.
*/

void BioloidDevice::SendReset()
{
    BLD_BUS_LOG( "Sending RESET to ID %d\n", m_id );

    m_bus->SendCmdHeader( m_id, 0, Bioloid::CMD_RESET );
    m_bus->SendCheckSum();
}

//***************************************************************************
/**
*   Writes some data into the control table, and returns the result.
*/

Bioloid::Error BioloidDevice::Write( uint8_t offset, const void *data, uint8_t numBytes )
{
    BioloidPacket   pkt;

    SendWrite( offset, data, numBytes );

    if ( m_id == Bioloid::BROADCAST_ID )
    {
        return Bioloid::ERROR_NONE;
    }

    if ( !m_bus->ReadStatusPacket( &pkt ))
    {
        return Bioloid::ERROR_TIMEOUT;
    }

    return pkt.ErrorCode();
}


/** @} */

