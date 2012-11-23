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
*   @file   BioloidDevice.h
*
*   @brief  Contains definitions for the bioloid generic command interface.
*
****************************************************************************/

#if !defined( BIOLOIDDEVICE_H )
#define BIOLOIDDEVICE_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

#include "Bioloid.h"
#include "BioloidBus.h"

/**
 * @addtogroup bioloid
 * @{
 */

class BioloidDevice
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    BioloidDevice();

    //------------------------------------------------------------------------
    // Normal constructor

    BioloidDevice( BioloidBus *bus, Bioloid::ID_t id );

    //------------------------------------------------------------------------
    // Destructor

    virtual ~BioloidDevice();

    //------------------------------------------------------------------------
    // Returns the ID of this device.

    Bioloid::ID_t ID() const { return m_id; }

    //------------------------------------------------------------------------
    // Pings the device and waits for a status packet.

    Bioloid::Error Ping();

    //------------------------------------------------------------------------
    // Reads data from the devices control table.

    virtual Bioloid::Error Read( uint8_t offset, void *data, uint8_t numBytes );

    //------------------------------------------------------------------------
    // Reads an 8 bit data register

    Bioloid::Error Read( uint8_t offset, uint8_t *val );
    Bioloid::Error Read( uint8_t offset, uint16_t *val );

    //------------------------------------------------------------------------
    // Resets the control table to factory defaults.

    Bioloid::Error Reset();

    //------------------------------------------------------------------------
    // Sends a ping packet to this device

    void SendPing();

    //------------------------------------------------------------------------
    // Sends a request to read data from the devices control table

    void SendRead( uint8_t offset, uint8_t numBytes );

    //------------------------------------------------------------------------
    // Sends some data to write into the control table.

    void SendWrite( uint8_t offset, const void *data, uint8_t numBytes );

    //------------------------------------------------------------------------
    // Sends some data to write into the control table. The write into the
    // control table will be deferred until the ACTION command is sent.

    void SendDeferredWrite( uint8_t offset, const void *data, uint8_t numBytes );

    //------------------------------------------------------------------------
    // Sends a commands to reset the control table to factory defaults.

    void SendReset();

    //------------------------------------------------------------------------
    // Sets the Bus and ID

    void SetBusAndID( BioloidBus *bus, Bioloid::ID_t id )
    {
        m_bus = bus;
        m_id = id;
    }

    //------------------------------------------------------------------------
    // Writes some data into the control table, and returns the result.

    virtual Bioloid::Error Write( uint8_t offset, const void *data, uint8_t numBytes );

protected:

    //------------------------------------------------------------------------

    BioloidBus     *m_bus;
    Bioloid::ID_t   m_id;   // The ID of this device

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    BioloidDevice( const BioloidDevice & copy );
    BioloidDevice &operator =( const BioloidDevice &rhs );
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif /* BIOLOIDDEVICE_H */

