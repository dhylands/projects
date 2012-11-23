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
*   @file   SimBus.h
*
*   @brief  Implements a simulated bioloid bus which is useful for some
*           types of debugging.
*
*   The simulated bus prints out all of the packets that would be
*   sent over the bus.
*
****************************************************************************/

#if !defined( SIMPACKET_H )
#define SIMPACKET_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidPacket.h"

/**
 * @addtogroup bioloid
 * @{
 */

class SimPacket : public BioloidPacket
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    SimPacket();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~SimPacket();

    //------------------------------------------------------------------------
    // Called whenever a character is received on the bus

    virtual void ProcessChar( uint8_t ch );

    //------------------------------------------------------------------------
    // Called when a packet has been parsed.

    virtual void PacketReceived( Bioloid::Error err );
};

/** @} */

#endif /* SIMPACKET_H */

