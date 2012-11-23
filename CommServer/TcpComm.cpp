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
*   @file   TcpComm.cpp
*
*   @brief  Implements the abstract communications device using a TCP/IP port.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <WinSock2.h>

#include "Error.h"
#include "LogMessage.h"
#include "TcpComm.h"
#include "Str.h"
#include "StrToken.h"

/**
 * @addtogroup TcpComm
 * @{
 */

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

bool TcpComm::m_initialized = false;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
*   Constructor
*/

TcpComm::TcpComm()
    : m_mode( TcpComm::Client ),
      m_socket( TcpComm::InvalidSocket ),
      m_defaultPort( TcpComm::DefaultPort )
{
    memset( &m_sockAddr, 0, sizeof( m_sockAddr ));
    memset( &m_connectionInfo, 0, sizeof( m_connectionInfo ));
}

//***************************************************************************
/**
*   Constructor
*/

TcpComm::TcpComm( Mode mode, unsigned short defaultPort )
    : m_mode( mode ),
      m_socket( TcpComm::InvalidSocket ),
      m_defaultPort( defaultPort )
{
    memset( &m_sockAddr, 0, sizeof( m_sockAddr ));
    memset( &m_connectionInfo, 0, sizeof( m_connectionInfo ));

    m_abortEvent = NULL;
    m_readEvent = NULL;
}

//***************************************************************************
/**
*   Destructor
*
* virtual
*/

TcpComm::~TcpComm()
{
    Close();
}

//***************************************************************************
/**
*   Aborts a read, if one is pending.
*
* virtual
*/

void TcpComm::AbortRead()
{
    SetEvent( m_abortEvent );

} // AbortRead

//***************************************************************************
/**
*   Close a previsouly opened device.
*
* virtual 
*/

void TcpComm::Close()
{
    if ( m_socket != InvalidSocket )
    {
        closesocket( m_socket );
        m_socket = InvalidSocket;
        memset( &m_sockAddr, 0, sizeof( m_sockAddr ));
        m_hostName = "";
        m_portStr = "";
        m_name = "";
    }

    if ( m_abortEvent != NULL )
    {
        CloseHandle( m_abortEvent );
        m_abortEvent = NULL;
    }
    if ( m_readEvent != NULL )
    {
        CloseHandle( m_readEvent );
        m_readEvent = NULL;
    }
}

//***************************************************************************
/**
*   Returns connection information (for display purposes)
*
* virtual
*/

const char *TcpComm::ConnectionInfo() const
{
    return m_connectionInfo;
}

//***************************************************************************
/**
*   Opens a TCP/IP socket. 
*
*   @returns    true if the port was opened successfully, false otherwise.
*
* virtual
*/

bool TcpComm::Open
(
    /** Specifies the host and port. The host can be a hostname or a dotted
        IP address. The port (optional) is specified with a colon and may be
        numeric of the name of a service.                                   */
    const char *openStr
)
{
    DWORD   err;
    char    errStr[ 200 ];

    // Initialize 

    if ( !m_initialized )
    {
        WSADATA  wsaData;

        if (( err = WSAStartup( 0x0202, &wsaData )) != 0  )
        {
            LOG_ERROR( "Error initializing Windows Sockets: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }
        m_initialized = true;
    }

    char   *endPtr;

    // Extract the hostname and portname.

    char    token[ 200 ];

    char *hostName = NULL;

    // NextToken parses skips leading delimiters

    if ( *openStr == ':' )
    {
        hostName = "";
        openStr++;
    }

    // If it's all digits, then assume it's a port number

    if ( strspn( openStr, "0123456789" ) == strlen( openStr ))
    {
        hostName = "";
    }

    StrTokenizer    tokenizer( openStr, token, sizeof( token ));

    if ( hostName == NULL )
    {
        hostName = tokenizer.NextToken( ":" );
    }

    if ( hostName == NULL )
    {
        if ( m_mode == TcpComm::Server )
        {
            hostName = "";
        }
        else
        {
            LOG_ERROR( "No hostname specified" );
            return false;
        }
    }

    if ( *hostName == '\0' )
    {
        m_sockAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
    }
    else
    {
        // Check to see if the servername is only made up of numbers and dots.

        if ( strspn( hostName, ".0123456789" ) == strlen( hostName ))
        {
            int   seg;
            char *segStr  = hostName;
            char *endPtr;

            // The hostname is only made up of numbers and dots. We'll parse
            // it ourselves.
            //
            // We expect it to be of the form a.b.c.d, where a, b, c, and d are
            // all positive integers in the range 0 thru 255.

            for ( seg = 0; seg < 4; seg++ ) 
            {
                long  longByte = strtol( segStr, &endPtr, 10 );

                if (( longByte < 0 ) || ( longByte > 255 ))
                {
                    LOG_ERROR( "Expecting a positive number between 0 and 255, found: '" << segStr << "'" );
                    return false;
                }
                if (( endPtr == segStr )
                ||  (( *endPtr == '.' ) && ( seg == 3 ))
                ||  (( *endPtr != '.' ) && ( seg <  3 ))) 
                {
                    LOG_ERROR( "Expecting IP address of the form a.b.c.d, found: '" << hostName << "'" );
                    return false;
                }

                m_sockAddr.sin_addr.S_un.S_addr <<= 8; 
				m_sockAddr.sin_addr.S_un.S_addr += (unsigned char)longByte;

                segStr = endPtr + 1;
            }
			m_sockAddr.sin_addr.S_un.S_addr = htonl( m_sockAddr.sin_addr.S_un.S_addr );
        }
        else
        {
            // The server name was specified symbolically, see if we can translate it

            struct hostent      *hostent;

            hostent = gethostbyname( hostName );
            if ( hostent == NULL )
            {
                LOG_ERROR( "Unable to translate hostname '" << hostName << "' into an IP address." );
                return false;
            }
            m_sockAddr.sin_addr.S_un.S_addr = *((long *)hostent->h_addr_list[ 0 ]);
        }
    }

    // Now parse the port (if present)

    const char *portStr = tokenizer.Remainder();

    if ( *portStr == '\0' )
    {
        // No port specified

        m_sockAddr.sin_port = htons( m_defaultPort );
    }
    else
    {
		long	longPort;

        if ( strspn( portStr, "0123456789" ) == strlen( portStr ))
        {
            // The port string is all numeric

            longPort = strtol( portStr, &endPtr, 0 );
            if ( *endPtr != '\0' )
            {
                LOG_ERROR( "Expecting a positive numeric port specifier; Found '" << portStr << "'." );
                return false;
            }
        }
        else
        {
            struct servent *servent;

            servent = getservbyname( portStr, "tcp" );
            if ( servent == NULL )
            {
                LOG_ERROR( "Unable to translate service '" << portStr << "' into a port number." );
                return false;
            }
            longPort = ntohs( servent->s_port );
        }

        if (( longPort <= 0 ) || ( longPort > 65535 ))
        {
            LOG_ERROR( "Expecting port to be between 1 and 65535; Found: " << longPort );
            return false;
        }
        m_sockAddr.sin_port = htons( (short)longPort );
    }
    m_sockAddr.sin_family = AF_INET;

    // We now have an IP address and port number

    // Open a socket

    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( m_socket == INVALID_SOCKET )
    {
        err = WSAGetLastError();
        LOG_ERROR( "Error calling socket: " << GetErrorStr( err, errStr, sizeof( errStr )));
        return false;
    }

    if ( m_mode == Server )
    {
        // Bind the address to the socket

        if ( bind( m_socket, (struct sockaddr *)&m_sockAddr, sizeof( m_sockAddr )) != 0 )
        {
            err = WSAGetLastError();
            LOG_ERROR( "Error calling bind: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        // Wait for an incoming connection

        LOG_INFO( "Waiting for incoming TCP/IP connection on port " << ntohs( m_sockAddr.sin_port ) << " ..." );

        int listenSocket = m_socket;

        if ( listen( listenSocket, 1 ) != 0 )
        {
            err = WSAGetLastError();
            LOG_ERROR( "Error calling listen: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        int addrLen = sizeof( m_remAddr );

        memset( &m_remAddr, 0, sizeof( m_remAddr ));

        m_socket = accept( listenSocket, (struct sockaddr *)&m_remAddr, &addrLen );
        closesocket( listenSocket );

        if ( m_socket == INVALID_SOCKET )
        {
            err = WSAGetLastError();
            LOG_ERROR( "Error calling accept: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        StrPrintf( m_connectionInfo, sizeof( m_connectionInfo ),
                   "%d.%d.%d.%d:%d",
                   m_remAddr.sin_addr.S_un.S_un_b.s_b1,
                   m_remAddr.sin_addr.S_un.S_un_b.s_b2,
                   m_remAddr.sin_addr.S_un.S_un_b.s_b3,
                   m_remAddr.sin_addr.S_un.S_un_b.s_b4,
                   ntohs( m_remAddr.sin_port ));
        
        LOG_INFO( "Connection received from " << m_connectionInfo
               << " on port " << ntohs( m_sockAddr.sin_port ));
    }
    else
    {
        // Connect to the server

        if ( connect( m_socket, (struct sockaddr *)&m_sockAddr, sizeof( m_sockAddr )) != 0 )
        {
            err = WSAGetLastError();
            LOG_ERROR( "Error calling connect: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        StrPrintf( m_connectionInfo, sizeof( m_connectionInfo ),
                   "%s (%d.%d.%d.%d:%d)", openStr,
                   m_sockAddr.sin_addr.S_un.S_un_b.s_b1,
                   m_sockAddr.sin_addr.S_un.S_un_b.s_b2,
                   m_sockAddr.sin_addr.S_un.S_un_b.s_b3,
                   m_sockAddr.sin_addr.S_un.S_un_b.s_b4,
                   ntohs( m_sockAddr.sin_port ));

        LOG_INFO( "Connected to " << m_connectionInfo );
    }

    // Create an event which will be signalled when new data arrives

    m_abortEvent = CreateEvent( NULL,   // lpEventAttributes
                                FALSE,  // bManualReset
                                FALSE,  // bInitialState
                                NULL ); // lpName
    
    m_readEvent  = CreateEvent( NULL,   // lpEventAttributes
                                FALSE,  // bManualReset
                                FALSE,  // bInitialState
                                NULL ); // lpName

    // Set things up so the event gets signalled when new data arrives

    if ( WSAEventSelect( m_socket, m_readEvent, FD_READ | FD_CLOSE ) != 0 )
    {
        err = WSAGetLastError();
        LOG_ERROR( "Error calling WSAEventSelect: " << GetErrorStr( err, errStr, sizeof( errStr )));
        return false;
    }

    m_name = openStr;

    return true;
}

//***************************************************************************
/**
*   Read data from the communication device.
*
*   @returns    true if the read was successful, flse otherwise.
*
* virtual 
*/

bool TcpComm::Read
(
    void   *buf,        ///< (out) Place to store the read data.
    size_t  bufSize,    ///< (in)  Size of the data buffer.
    size_t *bytesRead   ///< (out) Number of bytes actually read.
)
{
    *bytesRead = 0;

    // Wait for data to arrive

    HANDLE  waitHandle[ 2 ];

    waitHandle[ 0 ] = m_readEvent;
    waitHandle[ 1 ] = m_abortEvent;

    DWORD rc = WaitForMultipleObjects( 2, waitHandle, FALSE, INFINITE );

    if ( rc == ( WAIT_OBJECT_0 + 1 ))
    {
        // We've been asked to quit.

        return false;
    }
    if (( rc != WAIT_OBJECT_0 ) && ( rc != WAIT_TIMEOUT ))
    {
        DWORD	err = GetLastError();
        char	errStr[ 200 ];	

        LOG_ERROR( "TcpComm::Read Wait failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
        return false;
    }

    // Go and read the data. recv will return zero bytes if the remote
    // end closes the connection.

    int bytesRecv = recv( m_socket, (char *)buf, bufSize, 0 );

    if ( bytesRecv == SOCKET_ERROR )
    {
        DWORD	err = GetLastError();
        char	errStr[ 200 ];	

        if ( err == WSAECONNRESET )
        {
            // The remote end closed the connection forcibly. Treat it like 
            // a normal close

            *bytesRead = 0;
            return true;
        }

        LOG_ERROR( "TcpComm::Read recv failed (" << err << "): " << GetErrorStr( err, errStr, sizeof( errStr )));
        return false;
    }

    *bytesRead = bytesRecv;

	return true;
}

//***************************************************************************
/**
*   Write data to the communication device.
*
*   @returns    true if the write was successful, flse otherwise.
*
* virtual 
*/

bool TcpComm::Write
(
    const void *buf,            ///< (in)  Buffer containing data to write
    size_t      bytesToWrite,   ///< (in)  Number of bytes of data to write
    size_t     *bytesWritten    ///< (out) Number of bytes actually written
)
{
    *bytesWritten = 0;

    int bytesSent = send( m_socket, (const char *)buf, bytesToWrite, 0 );
    if ( bytesSent == SOCKET_ERROR )
    {
        return false;
    }
    *bytesWritten = bytesSent;

	return true;
}

/** @} */

