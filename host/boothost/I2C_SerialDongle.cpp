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
*   @file   I2C_SerialDongle.cpp
*
*   @brief  This file implemented the I2C_SerialDongle device.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "I2C_SerialDongle.h"
#include "PKT_Packet.h"
#include "DumpMem.h"
#include "Crc8.h"
#include "Log.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

struct
{
    I2C_Error_t     errCode;
    const char     *errStr;
} gErr[] =
{
    { I2C_ERROR_NONE,               "No Error" },
    { I2C_ERROR_ADDR_NACK,          "No Response" },
    { I2C_ERROR_DATA_NACK,          "NAK rcvd during data transfer" },
    { I2C_ERROR_ARBITRATION_LOST,   "Arbitration Lost" },
    { I2C_ERROR_BAD_LEN,            "Bad Length" },
    { I2C_ERROR_BAD_CRC,            "CRC failed" },
    { I2C_ERROR_BUS_ERROR,          "BUS Error" },
};

// ---- Private Function Prototypes -----------------------------------------

#define LOG( fmt, args... )     do { if ( m_debugDongle ) { Log( fmt, ## args ); }} while (0)

const char *ErrorStr( I2C_Error_t errCode );

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup i2c
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

I2C_SerialDongle::I2C_SerialDongle()
    : m_debugDongle( false ),
      m_debugDongleData( false )
{
    // We assume that the caller has installed callbacks with the 
    // PacketProtocol module.

    memset( &m_response, 0, sizeof( m_response ));

    if ( sem_init( &m_response.m_semaphore, 0, 0 ) != 0 )
    {
        LogError( "I2C_SerialDongle: call to sem_init failed: %s (%d)\n", strerror( errno ), errno );
        m_response.m_semaphore = 0;
    }

    m_dongleAddr = 0;
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

I2C_SerialDongle::~I2C_SerialDongle()
{
    if ( m_response.m_semaphore > 0 )
    {
        sem_destroy( &m_response.m_semaphore );
    }
}

//***************************************************************************
/**
*   Retrieves the I2C address associated with the serial dongle.
*
*   virtual
*/

bool I2C_SerialDongle::GetI2CAddress( I2C_Addr_t *masterAddr )
{
    LOG( "GetI2CAddress\n" );

    // If we've already fetched it from the device, just return the cached
    // resule.

    if ( m_dongleAddr != 0 )
    {
        LOG( "  returning 0x%02x (cached)\n", m_dongleAddr );

        *masterAddr = m_dongleAddr;
        return true;
    }

    PKT_Packet  pkt;

    pkt.data[ 0 ] = GetI2CAddrPacket;
    pkt.len = 1;

    Buffer response;

    response.dataLen = 1;
    response.data = masterAddr;

    // Since this is a synchronous call, we need to wait for the data to 
    // arrive.

    if ( SendPacketWaitForResponse( &pkt, GetI2CAddrReplyPacket, 0, &response ))
    {
        m_dongleAddr = *masterAddr;

        LOG( "  returning 0x%02x\n", m_dongleAddr );
        return true;
    }

    return false;
    
} // GetI2CAddress

//***************************************************************************
/**
*   Does a Write/Read (aka Process Call) with an i2c slave
*
*   virtual
*/

bool I2C_SerialDongle::Call
(
    I2C_Addr_t    slaveAddr,    ///< I2C Address of the slave device
    uint8_t       cmd,          ///< Command (type of call being performed)
    const Buffer *writeBuf,     ///< Buffer of data to send down to the device
    Buffer        *readBuf      ///< Buffer to place results in
)
{
    LOG( "Call SlaveAddr:0x%02x Cmd:0x%02x\n", slaveAddr, cmd );

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

    PKT_Packet  pkt;

    uint8_t len = writeBuf->dataLen;
    if ( len > ( sizeof( pkt.data ) - 5 ))
    {
        LogError( "I2C_SerialDongle::Call len (%d) is too big for packet (%d)\n", len, sizeof( pkt.data ));

        len = sizeof( pkt.data ) - 5;
    }

    uint8_t    *pd = &pkt.data[ 0 ];
    uint8_t     crc = 0;

    *pd++ = CallPacket;     // Not sent on i2c bus, therefore not included in crc

    *pd = ( slaveAddr << 1 ) | SlaveWrite;  crc = Crc8( crc, *pd );    pd++;
    *pd = cmd;                              crc = Crc8( crc, *pd );    pd++;
    *pd = len;                              crc = Crc8( crc, *pd );    pd++;

    for ( int i = 0; i < len; i++ ) 
    {
        *pd = writeBuf->data[ i ];
        crc = Crc8( crc, *pd );
        pd++;
    }
    *pd = ( slaveAddr << 1 ) | SlaveRead;   crc = Crc8( crc, *pd );    pd++;

    pkt.len = pd - &pkt.data[ 0 ];

    // Since this is a synchronous call, we need to wait for the data to 
    // arrive.

    return SendPacketWaitForResponse( &pkt, CallReplyPacket, crc, readBuf );

} // Call

//***************************************************************************
/**
*   Reads data from an i2c slave.
*
*   virtual
*/

bool I2C_SerialDongle::Read
(
    I2C_Addr_t  slaveAddr,  ///< I2C Address of the slave device
    uint8_t     cmd,        ///< Command (type of read being performed)
    Buffer     *buf         ///< Buffer to place results in
)
{
    LOG( "Read SlaveAddr:0x%02x Cmd:0x%02x\n", slaveAddr, cmd );

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

    PKT_Packet  pkt;

    uint8_t    *pd = &pkt.data[ 0 ];
    uint8_t     crc = 0;

    *pd++ = ReadPacket;     // Not send on i2c bus, therefore not included in crc

    *pd = ( slaveAddr << 1 ) | SlaveWrite;  crc = Crc8( crc, *pd );    pd++;
    *pd = cmd;                              crc = Crc8( crc, *pd );    pd++;
    *pd = ( slaveAddr << 1 ) | SlaveRead;   crc = Crc8( crc, *pd );    pd++;

    pkt.len = pd - &pkt.data[ 0 ];

    // Since this is a synchronous call, we need to wait for the data to 
    // arrive.

    return SendPacketWaitForResponse( &pkt, ReadReplyPacket, crc, buf );

} // Read

//***************************************************************************
/**
*   Writes data to an i2c slave.
*
*   virtual
*/

bool I2C_SerialDongle::Write
(
    I2C_Addr_t    slaveAddr,    ///< I2C Address of the slave device
    uint8_t       cmd,          ///< Command (type of write being performed)
    const Buffer *buf           ///< Buffer of data to send down to the device
)
{
    LOG( "Write SlaveAddr:0x%02x Cmd:0x%02x\n", slaveAddr, cmd );

    //  On the i2c bus, the following will take place (M=Master, S=Slave):
    //
    //  M->S:   Start
    //  M->S:   slave-addr w/ W
    //  M->S:   command
    //  M->S:   len
    //  M->S:   data (first byte should be address of master)
    //  M->S:   CRC
    //  M->S:   Stop

    PKT_Packet  pkt;

    uint8_t len = buf->dataLen;
    if ( len > ( sizeof( pkt.data ) - 4 ))
    {
        LogError( "I2C_SerialDongle::Write len (%d) is too big for packet (%d)\n", len, sizeof( pkt.data ));

        len = sizeof( pkt.data ) - 4;
    }

    uint8_t    *pd = &pkt.data[ 0 ];
    uint8_t     crc = 0;

    *pd++ = WritePacket;     // Not send on i2c bus, therefore not included in crc

    *pd = ( slaveAddr << 1 ) | SlaveWrite;  crc = Crc8( crc, *pd );    pd++;
    *pd = cmd;                              crc = Crc8( crc, *pd );    pd++;
    *pd = len;                              crc = Crc8( crc, *pd );    pd++;

    for ( int i = 0; i < len; i++ ) 
    {
        *pd = buf->data[ i ];
        crc = Crc8( crc, *pd );
        pd++;
    }
    *pd++ = crc;

    pkt.len = pd - &pkt.data[ 0 ];

    return SendPacketWaitForResponse( &pkt, WriteReplyPacket, 0, NULL );

} // Write

//***************************************************************************
/**
*   Function we expect to be called when a packet is received over the
*   serial port. Note that it is expected that this function will be
*   called from a different thread than the one which calls Read or Call.
*/

void I2C_SerialDongle::PacketReceived( PKT_Packet *packet )
{
    uint8_t    *pd = &packet->data[ 0 ];

    PacketType  packetType = (PacketType)*pd++;

    uint8_t crc = 0;
    uint8_t len = 0;

    if ( m_debugDongleData )
    {
        DumpMem( "PktRcvd", 0, packet->data, packet->len );
    }

    if ( packetType == ErrorPacket )
    {
        m_response.m_errorCode = *pd;

        LogError( "Rcvd Error packet, error code:%d '%s'\n", m_response.m_errorCode, ErrorStr( m_response.m_errorCode ));
    }
    else
    if ( packetType == m_response.m_responseType )
    {
        // This is the type of packet that the caller is waiting for.

        crc = m_response.m_partialCrc;

        if ( packet->len > 1 )
        {
            // Check the CRC to see if it's valid
        
            len = *pd;    crc = Crc8( crc, *pd ); pd++;
        
            // Sanity check the length. The packet length should equal the i2c len
            // plus 3 (1 for packet type, 1 for len, 1 for CRC)
        
            if (( len + 3 ) != packet->len )
            {
                LogError( "I2C_SerialDongle::PacketReceived Inconsistent packet length. Serial packetLen = %d, i2c packetLen = %d (expecting %d)\n",
                          packet->len, len, packet->len - 3 );
                m_response.m_errorCode = I2C_ERROR_BAD_LEN;
                packetType = ErrorPacket;
            }
            else
            {
                for ( int i = 0; i < len; i++ ) 
                {
                    crc = Crc8( crc, pd[ i ]);
                }
                if ( crc != pd[ len ] )
                {
                    LogError( "I2C_SerialDongle::PacketReceived CRC failure. Found 0x%02x, expecting 0x%02x\n", pd[ len ], crc );
                    m_response.m_errorCode = I2C_ERROR_BAD_CRC;
                    packetType = ErrorPacket;
                }
            }

            // We've received a good packet, and we're waiting for this type
            // of packet. Wakeup the reader
    
            if ( len < 1 )
            {                                   
                LogError( "I2C_SerialDongle::PacketReceived packet len too small (%d). Needs to be at least 1 to store address of sender\n", len );
                len = 1;
            }
    
            if ( len > m_response.m_buf->dataLen )
            {
                LogError( "I2C_SerialDongle::PacketReceived packet buffer too small. bufferSize = %d, received %d\n", m_response.m_buf->dataLen, len );
    
                len = m_response.m_buf->dataLen;
            }

            m_response.m_buf->dataLen = len;
            if (( len > 0 ) && ( m_response.m_buf != NULL ))
            {
                memcpy( &m_response.m_buf->data[ 0 ], &pd[ 0 ], len );
            }
        }
    }

    if (( packetType == m_response.m_responseType ) || ( packetType == ErrorPacket ))
    {
        // Setting m_responseType to NoPacket signals that we're no longer
        // waiting for a packet.

        m_response.m_responseType = NoPacket;

        sem_post( &m_response.m_semaphore );
        return;
    }

    // Call the registered callback to report the received packet

    if ( m_callback != NULL )
    {
        m_callback->PacketReceived( m_response.m_buf );
    }
    else
    {
        LogError( "m_callback == NULL\n" );
        LogError( "packetType:%d m_response.m_responseType:%d\n", packetType, m_response.m_responseType );
        LogError( "packet len: %d\n", packet->len );
        DumpMem( "  pkt", 0, packet->data, packet->len );
    }

} // PacketReceived

//***************************************************************************
/**
*   Sends a packet and waits for a respomse. We need to do things in the
*   correct order to prevent a race condition.
*/
bool I2C_SerialDongle::SendPacketWaitForResponse
(
    const PKT_Packet   *pkt,
    PacketType          responseType, 
    uint8_t             partialCrc, 
    Buffer             *buf
)
{
    // Currently we only handle one outstanding "wait" at a time.

    if ( m_response.m_responseType != NoPacket )
    {
        LogError( "m_response.m_responseType:%d\n", m_response.m_responseType );
    }
    assert( m_response.m_responseType == NoPacket );

    // Setup the response before sending the packet. Since we have a different
    // thread processing the received packets, we could receive the packet
    // really quickly

    m_response.m_responseType = responseType;
    m_response.m_partialCrc = partialCrc;
    m_response.m_buf = buf;
    m_response.m_errorCode = I2C_ERROR_NONE;

    // Send the packet

    if ( m_debugDongleData )
    {
        DumpMem( "SendPkt", 0, pkt->data, pkt->len );
    }

    PKT_SendPacket( pkt, this );

    // Wait for the response

    struct timespec waitTime;

    waitTime.tv_sec = time( NULL ) + 3;    // We shouldn't have to wait this long for a response
    waitTime.tv_nsec = 0;

    if ( sem_timedwait( &m_response.m_semaphore, &waitTime ) != 0 )
    {
        if ( errno == ETIMEDOUT )
        {
            LogError( "I2C_Dongle::WaitForResponse timeout waiting for response\n" );
        }
        else
        {
            LogError( "I2C_Dongle::WaitForResponse: error waiting for semaphore: %s (%d)\n", strerror( errno ), errno );
        }

        m_response.m_responseType = NoPacket;
        return false;
    }

    if ( m_response.m_errorCode != I2C_ERROR_NONE )
    {
        LogError( "I2C_Dongle::WaitForResponse error %d detected\n", m_response.m_errorCode );
        return false;
    }

    return true;

} // WaitForResponse


//***************************************************************************
/**
*   Translate an error code into a printable string
*/

const char *ErrorStr( I2C_Error_t errCode )
{
    int i;

    for ( i = 0; i < ( sizeof( gErr ) / sizeof( gErr[ 1 ])); i++ ) 
    {
        if ( errCode == gErr[ i ].errCode )
        {
            return gErr[ i ].errStr;
        }
    }

    return "*** Unknown ***";

} // ErrorStr


/** @} */

