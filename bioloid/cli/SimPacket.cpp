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
*   @file   SimPacket.cpp
*
*   @brief  This file implements the SimPacket class. The PacketReceived
*           method will be called whenever a packet is sent and it will 
*   print the contents of the packet.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "SimPacket.h"

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

SimPacket::SimPacket()
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

SimPacket::~SimPacket()
{
}

//***************************************************************************
/**
*   Called to run a character through the packet processor. We log the
*   character.
*
*   virtual
*/

void SimPacket::ProcessChar( uint8_t ch )
{
    Log( "%02x ", ch );

    BioloidPacket::ProcessChar( ch );
}

//***************************************************************************
/**
*   Called when a packet is parsed.
*
*   virtual
*/

void SimPacket::PacketReceived( Bioloid::Error err )
{
    if ( err == Bioloid::ERROR_NONE )
    {
        Log( "good\n" );
    }
    else
    {
        Log( "*** ERROR ***\n" );
    }
}

/** @} */

