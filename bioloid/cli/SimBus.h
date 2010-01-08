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

#if !defined( SIMBUS_H )
#define SIMBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidBus.h"
#include "SimPacket.h"

/**
 * @addtogroup bioloid
 * @{
 */

class SimBus : public BioloidBus
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    SimBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~SimBus();

    //------------------------------------------------------------------------
    // Reads a byte.

    virtual bool ReadByte( uint8_t *ch );

    //------------------------------------------------------------------------
    // Sends a byte. This will automatically accumulate the byte into 
    // the checksum

    virtual void SendByte( uint8_t data );

private:

    SimPacket   m_packet;

};

/** @} */

#endif /* SIMBUS_H */

