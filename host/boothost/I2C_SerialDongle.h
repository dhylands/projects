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
*   @file   I2C_SerialDongle.h
*
*   @brief  This file describes the interface to the I2C_SerialDongle class.
*
****************************************************************************/

#if !defined( I2C_SERIALDONGLE_H )
#define I2C_SERIALDONGLE_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#if !defined( I2C_ADAPTER_H )
#   include "I2C_Adapter.h"
#endif

#if !defined( PKT_PACKET_H )
#   include "PKT_Packet.h"
#endif

#if !defined( SERIALPORT_H )
#   include "SerialPort.h"
#endif

#if !defined( PACKETDEF_H )
#   include "PacketDef.h"
#endif

#if !defined( I2C_H )
#   include "i2c.h"
#endif

#include <semaphore.h>

/**
 * @addtogroup i2c
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The I2C_SerialDongle class implements an i2c device interface which 
*   is accessed via the serial port.
*
*   The protocol used by the serial dongle is to use the framing provided
*   by PacketProtocol.h, and to send down a packet type (read/write/call, etc)
*   followed by the data that would be sent out the i2c device.
*
*   See the I2C_Adapter.h for details of data sent back and forth on the i2c
*   bus. Note that the Start/RepeatedS tart/Stop don't correspond to bytes
*   that are sent.
*/

class I2C_SerialDongle : public I2C_Adapter
{
public:

    //------------------------------------------------------------------------
    //  Default constructor

    I2C_SerialDongle();

    //------------------------------------------------------------------------
    //  Destructor

    virtual ~I2C_SerialDongle();

    //------------------------------------------------------------------------
    // Function we expect to be called when a packet is received over the
    // serial port. Note that it is expected that this function will be
    // called from a different thread than the one which calls Read or Call.

    void PacketReceived( PKT_Packet *pkt );

    //------------------------------------------------------------------------
    // Retrieves the i2c address associated with the serial dongle

    virtual bool GetI2CAddress( I2C_Addr_t *masterAddr );

    //------------------------------------------------------------------------
    //  Does a Write/Read (aka Process Call) with an i2c slave

    virtual bool Call( I2C_Addr_t slaveAddr, uint8_t cmd, const Buffer *writeBuf, Buffer *readBuf );

    //------------------------------------------------------------------------
    //  Reads data from an i2c slave

    virtual bool Read( I2C_Addr_t slaveAddr, uint8_t cmd, Buffer *buf );

    //------------------------------------------------------------------------
    //  Writes data to an i2c slave.

    virtual bool Write( I2C_Addr_t slaveAddr, uint8_t cmd, const Buffer *buf );

    bool            m_debugDongle;      ///< High level debugging
    bool            m_debugDongleData;  ///< Print packet contents

private:


    //------------------------------------------------------------------------
    //  The copy constructor and assignment operator are not need for this
    //  class so we declare them private and don't provide an implementation.

    I2C_SerialDongle( const I2C_SerialDongle & copy );
    I2C_SerialDongle &operator =( const I2C_SerialDongle &rhs );

    //------------------------------------------------------------------------
    // Waits for the desired type of response to be received

    bool SendPacketWaitForResponse( const PKT_Packet *pkt, PacketType responseType, uint8_t partialCrc, Buffer *buf );

    //------------------------------------------------------------------------

    struct Response
    {
        PacketType  m_responseType; ///< Type of packet we're waiting for
        uint8_t     m_partialCrc;   ///< CRC so far on the request side
        Buffer     *m_buf;          ///< Buffer to store response in
        I2C_Error_t m_errorCode;    ///< Error while processing command

        sem_t       m_semaphore;    ///< Semaphore to block on until response is ready
    };
    Response        m_response;

    I2C_Addr_t      m_dongleAddr;   ///< i2c address of the serial dongle
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // I2C_SERIALDONGLE_H

