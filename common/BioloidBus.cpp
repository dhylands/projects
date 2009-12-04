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
*   Sends 'len' bytes, returning the sum of the bytes for easy accumulation
*   into the checksum) 
*/

void BioloidBus::SendData( uint8_t len, const uint8_t *data )
{
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
*/

void BioloidBus::SendCmdHeader
(
    Bioloid::ID_t       id, 
    uint8_t             paramLen,
    Bioloid::Command    cmd
)
{
    SendByte( 0xff );
    SendByte( 0xff );

    m_checksum = 0;

    SendByte( id );
    SendByte( paramLen + 2 );
    SendByte( cmd );
}

/** @} */

