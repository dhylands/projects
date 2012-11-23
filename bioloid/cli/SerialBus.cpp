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
    : m_serialPort( NULL )
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
*   Reads a byte.
*
*   virtual
*/

bool SerialBus::ReadByte( uint8_t *ch )
{
    return m_serialPort->Read( ch, 1 ) == 1;
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

//***************************************************************************
/**
*   Writes all of the buffered bytes to the serial port.
*
*   virtual
*/

void SerialBus::WriteBufferedData( void *data, size_t numBytes )
{
    size_t  bytesWritten;

    if (( bytesWritten = m_serialPort->Write( data, numBytes )) != numBytes )
    {
        LogError( "Error writing %d bytes to serial port", numBytes );
    }
}

/** @} */

