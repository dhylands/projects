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
*   @file   SerialComm.cpp
*
*   @brief  Implements the abstract communications device using a serial port.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Error.h"
#include "LogMessage.h"
#include "SerialComm.h"
#include "StrToken.h"
#include "Str.h"

/**
 * @addtogroup SerialComm
 * @{
 */

#if 0
#define	DBG(x)	LOG_DEBUG(x)
#else
#define	DBG(x)
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
*   Constructor
*/

SerialComm::SerialComm()
{
    m_serialHndl = INVALID_HANDLE_VALUE;
    memset( &m_readOverlapped, 0, sizeof( m_readOverlapped ));
    memset( &m_writeOverlapped, 0, sizeof( m_writeOverlapped ));

    memset( &m_connectionInfo, 0, sizeof( m_connectionInfo ));
}

//***************************************************************************
/**
*   Destructor
*
* virtual
*/

SerialComm::~SerialComm()
{
    Close();
}

//***************************************************************************
/**
*   Aborts a read, if one is pending.
*
* virtual
*/

void SerialComm::AbortRead()
{
    SetEvent( m_abortEvent );

} // AbortRead

//***************************************************************************
/**
*   Close a previsouly opened device.
*
* virtual 
*/

void SerialComm::Close()
{
    if ( m_serialHndl != INVALID_HANDLE_VALUE )
    {
        CloseHandle( m_serialHndl );
        m_serialHndl = INVALID_HANDLE_VALUE;
        m_name = "";
    }

    if ( m_readOverlapped.hEvent != NULL )
    {
        CloseHandle( m_readOverlapped.hEvent );
        m_readOverlapped.hEvent = NULL;
    }
    if ( m_writeOverlapped.hEvent != NULL )
    {
        CloseHandle( m_writeOverlapped.hEvent );
        m_writeOverlapped.hEvent = NULL;
    }
    if ( m_abortEvent != NULL )
    {
        CloseHandle( m_abortEvent );
        m_abortEvent = NULL;
    }
}

//***************************************************************************
/**
*   Returns connection information (for display purposes)
*
* virtual
*/

const char *SerialComm::ConnectionInfo() const
{
    return m_connectionInfo;
}

//***************************************************************************
/**
*   Opens a serial port. 
*
*   @returns    true if the port was opened successfully, false otherwise.
*
* virtual
*/

bool SerialComm::Open
(
    /// (in) Determines the serial port and parameters. The parameters are the same
    /// as what the MS-DOS MODE command supports.
    const char *openStr
)
{
    DWORD	err;
    char	errStr[ 200 ];

    // Extract the filename (terminated by a colon). The remaining parameters
    // are passed in to BuildCommDCB
    //
    //  COM1: baud=9600 parity=N data=8 stop=1

    char    token[ 20 ];

    StrTokenizer    tokenizer( openStr, token, sizeof( token ));

    char *comPort = tokenizer.NextToken( " " );

    if ( comPort == NULL )
    {
        LOG_ERROR( "No COM port specified" );
        return false;
    }

    const char *dcbParms = tokenizer.Remainder();
    m_serialHndl = CreateFile( comPort,                       // lpFileName
                               GENERIC_READ | GENERIC_WRITE,  // dwDesiredAccess
                               0,                             // dwShareMode
                               NULL,                          // lpSecurityAttributes
                               OPEN_EXISTING,                 // dwCreationDisposition
                               FILE_FLAG_OVERLAPPED,          // dwFlagsAndAttributes
                               NULL );                        // hTemplateFile
    if ( m_serialHndl == INVALID_HANDLE_VALUE )
    {
        err = GetLastError();

        LOG_ERROR( "Unable to open COM port '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

		return false;
    }
    m_name = comPort;

    DCB			    dcb;
    COMMTIMEOUTS    timeouts;

	memset( &dcb, 0, sizeof( dcb ));
	dcb.DCBlength = sizeof( dcb );

    memset( &timeouts, 0, sizeof( timeouts ));

    if ( !GetCommState( m_serialHndl, &dcb ))
    {
        err = GetLastError();

        LOG_ERROR( "Call to GetCommState failed for  '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
		return false;
    }

    if ( !GetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LOG_ERROR( "Call to GetCommTimeouts failed for  '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
		return false;
    }

    dcb.BaudRate = 19200;
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    dcb.fBinary      = 1;
    dcb.fParity      = 0;

    dcb.fOutX        = 0;
    dcb.fOutxCtsFlow = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fInX         = 0;

    dcb.fDtrControl  = DTR_CONTROL_ENABLE;
    dcb.fRtsControl  = RTS_CONTROL_ENABLE;
    dcb.fDsrSensitivity   = 0;
    dcb.fTXContinueOnXoff = 1;

	timeouts.ReadIntervalTimeout = 20;

    if ( !BuildCommDCBAndTimeouts( dcbParms, &dcb, &timeouts ))
    {
        err = GetLastError();

        LOG_ERROR( "Unable to parse COM parameters: '" << dcbParms << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
        return false;
    }

    char    parityChar;

    switch ( dcb.Parity )
    {
        case EVENPARITY:    parityChar = 'E';   break;
        case ODDPARITY:     parityChar = 'O';   break;
        case NOPARITY:      parityChar = 'N';   break;
        case MARKPARITY:    parityChar = 'M';   break;
        case SPACEPARITY:   parityChar = 'S';   break;
        default:            parityChar = '*';   break;
    }

    const char *stopStr;

    switch ( dcb.StopBits )
    {
        case ONESTOPBIT:    stopStr = "1";      break;
        case ONE5STOPBITS:  stopStr = "1.5";    break;
        case TWOSTOPBITS:   stopStr = "2";      break;
        default:            stopStr = "*";      break;
    }

    if ( !SetCommState( m_serialHndl, &dcb ))
    {
        err = GetLastError();

        LOG_ERROR( "Call to SetCommState failed for  '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
		return false;
    }

    if ( !SetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LOG_ERROR( "Call to SetCommTimeouts failed for  '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
		return false;
    }

    if ( !SetupComm( m_serialHndl, 32768,  32768 ))
    {
        err = GetLastError();

        LOG_ERROR( "Call to SetupComm failed for  '" << comPort << "'." );
        LOG_ERROR( GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
		return false;
    }

    StrPrintf( m_connectionInfo, sizeof( m_connectionInfo ), 
               "%d-%c-%d-%s", dcb.BaudRate, parityChar, dcb.ByteSize, stopStr );


    // Note that overlapped functions want a Manual-Reset event

    m_readOverlapped.hEvent = CreateEvent( NULL,    // lpEventAttributes
                                           TRUE,    // bManualReset
                                           FALSE,   // bInitialState
                                           NULL );  // lpName

    m_writeOverlapped.hEvent = CreateEvent( NULL,   // lpEventAttributes
                                            TRUE,   // bManualReset
                                            FALSE,  // bInitialState
                                            NULL ); // lpName

    m_abortEvent = CreateEvent( NULL,   // lpEventAttributes
                                FALSE,  // bManualReset
                                FALSE,  // bInitialState
                                NULL ); // lpName

#if 0
    COMMTIMEOUTS    ct;

    if ( GetCommTimeouts( m_serialHndl, &ct ))
    {
        LOG_INFO( "ReadIntervalTimeout         = " << ct.ReadIntervalTimeout );
        LOG_INFO( "ReadTotalTimeoutMultiplier  = " << ct.ReadTotalTimeoutMultiplier );
        LOG_INFO( "ReadTotalTimeoutConstant    = " << ct.ReadTotalTimeoutConstant );
        LOG_INFO( "WriteTotalTimeoutMultiplier = " << ct.WriteTotalTimeoutMultiplier );
        LOG_INFO( "WriteTotalTimeoutConstant   = " << ct.WriteTotalTimeoutConstant );
    }
#endif

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

bool SerialComm::Read
(
    void   *buf,        ///< (out) Place to store the read data.
    size_t  bufSize,    ///< (in)  Size of the data buffer.
    size_t *bytesRead   ///< (out) Number of bytes actually read.
)
{
    DWORD   err;
    char    errStr[ 200 ];
    
    *bytesRead = 0;

    // Use a while loop, since we don't always call WaitCommEvent, it's possible
    // for the event to be in a 'set' state and there not be any data in
    // the buffer. This basically just prevents us from returning no 
    // data to the caller.

	DBG( "Entered SerialComm::Read" );

    while ( *bytesRead == 0 )
    {
        // Wait for some data to arrive
    
        if ( !SetCommMask( m_serialHndl, EV_RXCHAR ))
        {
            err = GetLastError();
            LOG_ERROR( "SerialComm::Read call to SetCommMask failed: "
                    << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }
    
        // Check to see if there are any characters in the buffer already. If
        // there are, it's possible that they were missed by a previous
        // read (due to buffer full) and the event won't get triggered.
        //
        // Characters which arrive after the ReadFile call will set the event
        // but since ReadFile clears the event, characters which didn't fit
        // in the read buffer won't set the event.
    
        DWORD   commErrors;
        COMSTAT commStat;
    
        if ( !ClearCommError( m_serialHndl, &commErrors, &commStat ))
        {
    		err = GetLastError();
    
            LOG_ERROR( "SerialComm::Read call to ClearCommErros failed: "
                    << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }
    
        if ( commStat.cbInQue == 0 )
        {
    		DWORD	evtMask;
    
			DBG( "Calling WaitCommEvent" );
            if ( !WaitCommEvent( m_serialHndl, &evtMask, &m_readOverlapped ))
            {
                err = GetLastError();

                if ( err != ERROR_IO_PENDING )
                {
                    LOG_ERROR( "SerialComm::Read call to WaitCommEvent failed: "
                            << GetErrorStr( err, errStr, sizeof( errStr )));
                    return false;
                }
        
                // Wait for an event to occur
        
                DWORD   rc;
                HANDLE  waitHandle[ 2 ];
        
                waitHandle[ 0 ] = m_readOverlapped.hEvent;
                waitHandle[ 1 ] = m_abortEvent;

				DBG( "I/O Pending - waiting for event" );
				
                rc = WaitForMultipleObjects( 2, waitHandle, FALSE, INFINITE );
        
                if ( rc == ( WAIT_OBJECT_0 + 1 ))
                {
                    // We've been asked to quit. Cancel the I/O.

					DBG( "Abort detected" );
        
                    CancelIo( m_serialHndl );
                    return false;
                }
                if ( rc != WAIT_OBJECT_0 )
                {
                    err = GetLastError();
        
                    LOG_ERROR( "SerialComm::Read Wait for WaitCommEvent failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                    return false;
                }
        
                // Get the result of the overlapped operation
        
    			DWORD	dummy;
                if ( !GetOverlappedResult( m_serialHndl, &m_readOverlapped, &dummy, TRUE ))
                {
                    err = GetLastError();
        
                    LOG_ERROR( "SerialComm::Read GetOverlappedResult for WaitCommEvent failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                    return false;
                }
            }

            DBG( "WaitCommEvent finished: " << HEX( evtMask ));
        }
        
        // Start the Read. Since we've been told that there's data available
        // this should suceed.
    
        DWORD   bytesReadDword;

		DBG( "About to call ReadFile" );
    
        if ( !ReadFile( m_serialHndl, buf, bufSize, &bytesReadDword, &m_readOverlapped ))
        {
            err = GetLastError();
    
            if ( err != ERROR_IO_PENDING )
            {
                LOG_ERROR( "SerialComm::Read failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                return false;
            }
    
            // Wait for the read to finish, or to be aborted. Sometimes, the
            // EV_RXCHAR event is set even when there aren't any characters
            // in the buffer.
    
            DWORD   rc;
            HANDLE  waitHandle[ 2 ];

            waitHandle[ 0 ] = m_readOverlapped.hEvent;
            waitHandle[ 1 ] = m_abortEvent;
    
			DBG( "Waiting for read to complete" );

            rc = WaitForMultipleObjects( 2, waitHandle, FALSE, INFINITE );
    
            if ( rc == ( WAIT_OBJECT_0 + 1 ))
            {
                // We've been asked to quit. Cancel the I/O.

                DBG( "Abort detected" );

                CancelIo( m_serialHndl );
                return false;
            }
            if ( rc != WAIT_OBJECT_0 )
            {
                err = GetLastError();
    
                LOG_ERROR( "SerialComm::Read Wait failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                return false;
            }
    
            // Get the result of the overlapped operation
    
            if ( !GetOverlappedResult( m_serialHndl, &m_readOverlapped, &bytesReadDword, TRUE ))
            {
                err = GetLastError();
    
                LOG_ERROR( "SerialComm::Read GetOverlappedResult failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                return false;
            }

			DBG( "Read completed" );
        }
        *bytesRead = bytesReadDword;
    }
    //LOG_DEBUG( "SerialComm::Read returned " << *bytesRead << " bytes." );

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

bool SerialComm::Write
(
    const void *buf,            ///< (in)  Buffer containing data to write
    size_t      bytesToWrite,   ///< (in)  Number of bytes of data to write
    size_t     *bytesWritten    ///< (out) Number of bytes actually written
)
{
    DWORD   err;
    char    errStr[ 200 ];
    
    *bytesWritten = 0;

    // Start the write operation

    DWORD	bytesWrittenDword;

    if ( !WriteFile( m_serialHndl, buf, bytesToWrite, &bytesWrittenDword, &m_writeOverlapped ))
	{
        err = GetLastError();

        if ( err != ERROR_IO_PENDING )
        {
            LOG_ERROR( "SerialComm::Write failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        // Wait for the write to complete

        if ( WaitForSingleObject( m_writeOverlapped.hEvent, INFINITE ) != WAIT_OBJECT_0 )
        {
            err = GetLastError();

            LOG_ERROR( "SerialComm::Write Wait failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }

        // Get the result of the overlapped operation

        if ( !GetOverlappedResult( m_serialHndl, &m_writeOverlapped, &bytesWrittenDword, TRUE ))
        {
            err = GetLastError();

            LOG_ERROR( "SerialComm::Write GetOverlappedResult failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
            return false;
        }
	}                              

	*bytesWritten = bytesWrittenDword;

	return true;
}

/** @} */

