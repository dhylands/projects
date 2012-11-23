/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   TcpComm.h
*
*   @brief  Implements the abstract communications device using a TCP/IP port.
*
****************************************************************************/
/**
*  @defgroup    TcpComm  TCP/IP Communications Device
*  @ingroup     CommDev
*
*   @brief  Implements the abstract communications device using a TCP/IP port.
*/

#if !defined( TCPCOMM_H )
#define TCPCOMM_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include "CommDev.h"

#if defined( WIN32_LEAN_AND_MEAN )
#   error Sockets requires WIN32_LEAN_AND_MEAN to be undefined
#endif
#if !defined( _WINDOWS_ )
#   include <windows.h>
#endif

/**
 * @addtogroup TcpComm
 * @{
 */

//---------------------------------------------------------------------------
/**
*  The TcpComm class implements the serial communications device.
*/

class TcpComm : public CommDev
{
public:

    /**
     *  Describes the type of socket that we're going to create.
     */

    enum Mode 
    {
        Client, ///< This is a client endpoint (default)
        Server  ///< This is a server endpoint
    };

    /**
     *  Default port to use if the default constructor is called.
     */

    enum { DefaultPort = 4444 };

    //------------------------------------------------------------------------
    // Default constructor
    
    TcpComm();
    
    //------------------------------------------------------------------------
    // Default constructor
    
    TcpComm( Mode mode, unsigned short portNum = DefaultPort );
    
    //------------------------------------------------------------------------
    // Destructor
    
    virtual ~TcpComm();
    
    //------------------------------------------------------------------------
    // Aborts a read, if one is pending.
    
    virtual void AbortRead();
    
    //------------------------------------------------------------------------
    // Close a previsouly opened device.
    
    virtual void Close();
    
    //------------------------------------------------------------------------
    // Returns connection information (for display purposes)

    virtual const char *ConnectionInfo() const;

    //------------------------------------------------------------------------
    // Open a communication device. This provides a generic interface,
    // but a derived class may choose to provide a more specialized version.
    
    virtual bool Open( const char *openStr );
    
    //------------------------------------------------------------------------
    // Read data from the communication device.
    
    virtual bool Read( void *buf, size_t bufSize, size_t *bytesRead );

    //------------------------------------------------------------------------
    // Write data to the communication device.

    virtual bool Write( const void *buf, size_t bufSize, size_t *bytesWritten );

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.
    
    TcpComm( const TcpComm & copy );
    TcpComm &operator =( const TcpComm &rhs );
    
    //------------------------------------------------------------------------

    /**
     *  Value which can be storeed in m_socket to indicate that a socket 
     *  handle is invalid.
     */

    enum { InvalidSocket = -1 };

    Mode                m_mode;         ///< Type of socket (Client or Server)
    unsigned short      m_defaultPort;  ///< Default Port number (if none specified)
    struct sockaddr_in  m_sockAddr;     ///< Socket address for the remote end
    struct sockaddr_in  m_remAddr;      ///< Address of remote end
    std::string         m_hostName;     ///< Host name/IP address that was passed in
    std::string         m_portStr;      ///< Port name/number
    int                 m_socket;       ///< Socket handle
	HANDLE				m_abortEvent;	///< Event that's signalled to abort a read
	HANDLE				m_readEvent;	///< Event that indicates when new data has arrived

    char                m_connectionInfo[ 100 ]; ///< Describes the hostname etc.

    static  bool        m_initialized;  ///< Has WSAStartup been called yet?
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // TCPCOMM_H

