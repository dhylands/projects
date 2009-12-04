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
*   @file   BioloidBus.h
*
*   @brief  Contains the definitions for a bioloid bus. Essentially
*           there is a bus for each UART with bioloid devices attached.
*
****************************************************************************/

#if !defined( BIOLOIDBUS_H )
#define BIOLOIDBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdint.h>
#include "Bioloid.h"

/**
 * @addtogroup bioloid
 * @{
 */

#define BLD_BUS_LOG( fmt, args... ) do { if ( BioloidBus::m_log ) { Log( fmt, ##args ); }} while (0)

class BioloidBus
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    BioloidBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~BioloidBus();

    //------------------------------------------------------------------------
    // Broadcasts an action packet to all of the devices on the bus.
    // This causes all of the devices to perform their deferred writes
    // at the same time.

    void SendAction();

    //------------------------------------------------------------------------
    // Sends a byte. This will automatically accumulate the byte into 
    // the checksum

    virtual void SendByte( uint8_t data ) = 0;

    //------------------------------------------------------------------------
    // Sends Sends the accumulated checksum.

    void SendCheckSum() { SendByte( ~m_checksum ); }

    //------------------------------------------------------------------------
    // Sends 'len' bytes

    void SendData( uint8_t len, const uint8_t *data );

    //------------------------------------------------------------------------
    // Sends the command header, which is common to all of the commands.
    // 2 is added to paramLen (to cover the length and cmd bytes). This
    // way the caller is only responsible for figuring out how many extra
    // parameter bytes are being sent.

    void SendCmdHeader( Bioloid::ID_t id, uint8_t paramLen, Bioloid::Command cmd );

    static  bool    m_log;

protected:

    uint8_t     m_checksum;

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    BioloidBus( const BioloidBus & copy );
    BioloidBus &operator =( const BioloidBus &rhs );
};

/** @} */

#endif /* BIOLOIDBUS_H */

