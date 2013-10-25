/****************************************************************************
*
*   Copyright (c) 2005 - 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   I2C_Adapter.h
*
*   @brief  This file describes the interface to the I2C_Adapter class.
*
****************************************************************************/
/**
*   @defgroup   i2c   i2c abstract base class.
*
*   @brief      Interface to the i2c device.
*
****************************************************************************/

#if !defined( I2C_ADAPTER_H )
#define I2C_ADAPTER_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <string.h>
#include <inttypes.h>

#include "i2c.h"
#include "State.h"

/**
 * @addtogroup i2c
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The I2C_Adapter class forms an abstract base class, used to communicate with
*   i2c devices. A derived class does the real work. This way we can have
*   a common interface for say talking directly with i2c (on the gumstix) or
*   through an i2c serial dongle (oin the PC).
*/

class I2C_Adapter
{
public:

    struct Buffer
    {
        uint8_t     dataLen;
        uint8_t    *data;

        Buffer() : data( NULL ), dataLen( 0 ) {}
        Buffer( void *inData, uint8_t inLen ) : data( (uint8_t *)inData ), dataLen( inLen ) {}

        void Clear()
        {
            memset( data, 0, dataLen );
        }
    };

    class Callback
    {
    public:
        virtual ~Callback();
        virtual void PacketReceived( Buffer *buf ) = 0;
    };

    //------------------------------------------------------------------------
    // The ReadWrite flag is or'd together with the slave address.

    enum SlaveReadWrite
    {
        SlaveWrite   = 0x00,
        SlaveRead    = 0x01,
    };

    //------------------------------------------------------------------------
    //  Default constructor

    I2C_Adapter();

    //------------------------------------------------------------------------
    //  Destructor

    virtual ~I2C_Adapter();

    //------------------------------------------------------------------------
    // Sets the callback which is called when a packet is received

    void SetCallback( Callback *callback );

    //------------------------------------------------------------------------
    // Retrieves the i2c address associated with the adapter

    virtual bool GetI2CAddress( I2C_Addr_t *masterAddr ) = 0;

    //------------------------------------------------------------------------
    //  Reads data from an i2c slave.
    //
    //  On the i2c bus, the following will take place (M=Master, S=Slave):
    //
    //  M->S:   Start
    //  M->S:   slave-addr w/ W
    //  M->S:   command
    //  M->S:   Repeated Start
    //  M->S:   slave-addr w/ R
    //  S->M:   len
    //  S->M:   data (first byte should be address of slave)
    //  S->M:   CRC
    //  M->S:   Stop

    virtual bool Read( I2C_Addr_t slaveAddr, uint8_t cmd, Buffer *buf ) = 0;

    //------------------------------------------------------------------------
    //  Writes data to an i2c slave.
    //
    //  On the i2c bus, the following will take place (M=Master, S=Slave):
    //
    //  M->S:   Start
    //  M->S:   slave-addr w/ W
    //  M->S:   command
    //  M->S:   len
    //  M->S:   data (first byte should be address of master)
    //  M->S:   CRC
    //  M->S:   Stop

    virtual bool Write( I2C_Addr_t slaveAddr, uint8_t cmd, const Buffer *buf )= 0;

    //------------------------------------------------------------------------
    //  Does a Write/Read (aka Process Call) with an i2c slave
    //
    //  On the i2c bus, the following will take place (M=Master, S=Slave):
    //
    //  M->S:   Start
    //  M->S:   slave-addr w/ W
    //  M->S:   command
    //  M->S:   len
    //  M->S:   data (first byte should be address of master)
    //  M->S:   Repeated Start
    //  M->S:   slave-addr w/ R
    //  S->M:   len
    //  S->M:   data (first byte should be address of slave)
    //  S->M:   CRC
    //  M->S:   Stop

    virtual bool Call( I2C_Addr_t slaveAddr, uint8_t cmd, const Buffer *writeBuf, Buffer *readBuf ) = 0;

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    I2C_Adapter( const I2C_Adapter & copy );
    I2C_Adapter &operator =( const I2C_Adapter &rhs );

    //------------------------------------------------------------------------

protected:

    Callback   *m_callback;         ///< Fucntion called when the read data arrives.
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // I2C_ADAPTER_H

