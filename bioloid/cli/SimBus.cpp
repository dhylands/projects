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
*   @file   SimBus.cpp
*
*   @brief  This file implements the SimBus class, which is a simulated
*           bioloid bus, that just prints out the contents of the packets received.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "SimBus.h"

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

SimBus::SimBus()
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

SimBus::~SimBus()
{
}

//***************************************************************************
/**
*   Reads a byte.
*
*   virtual
*/

bool SimBus::ReadByte( uint8_t *ch )
{
    return false;
}

//***************************************************************************
/**
*   Sends a byte. This will automatically accumulate the byte into 
*   the checksum)
*
*   virtual
*/

void SimBus::SendByte( uint8_t data )
{
    m_checksum += data;

    m_packet.ProcessChar( data );
}

/** @} */

