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
*   @file   SerialBus.h
*
*   @brief  Implements a bioloid bus using posix serial. This typically
*           assumes that an FTDI USB-to-serial adapter is being used
*   to do the RS-485 management.
*
****************************************************************************/

#if !defined( NETBUS_H )
#define NETBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidBus.h"

#if defined( WIN32 )
#include <windows.h>
#else
typedef int SOCKET;
#include <netinet/in.h>
#endif

/**
 * @addtogroup bioloid
 * @{
 */

class NetBus : public BioloidBus
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    NetBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~NetBus();

    //------------------------------------------------------------------------
    // Sets the serial port which will be used for communications

    bool Open( const char *hostStr );

    //------------------------------------------------------------------------
    // Reads a byte.

    virtual bool ReadByte( uint8_t *ch );

private:

    //------------------------------------------------------------------------
    // Core portion of the WriteBuffer routine, which writes the data
    // out the serial port.

    virtual void WriteBufferedData( void *data, size_t numBytes );

    //------------------------------------------------------------------------

    bool                m_initialized;

    SOCKET              m_socket;       ///< Socket handle
    size_t              m_dataBytes;
    uint8_t             m_data[ 128 ];
    unsigned short      m_defaultPort;  ///< Default Port number (if none specified)
    struct sockaddr_in  m_sockAddr;     ///< Socket address for the remote end
    char                m_connectionInfo[ 100 ]; ///< Describes the hostname etc.
};

/** @} */

#endif /* NETBUS_H */

