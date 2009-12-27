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
*   @file   SerialPort.cpp
*
*   @brief  This file implements the SerialPort class using the posix
*           API, which is supported by cygwin and linux.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"
#include "SerialPort.h"
#include "Str.h"
#include "Error.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup SerialPort
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

SerialPort::SerialPort()
    : m_serialHndl( INVALID_HANDLE_VALUE )
{
}

//***************************************************************************
/**
*   Destructor
*/

SerialPort::~SerialPort()
{
    Close();
}

//***************************************************************************
/**
*   Closes a previously opened serial port
*/

void SerialPort::Close()
{
    if ( m_serialHndl != INVALID_HANDLE_VALUE )
    {
        CloseHandle( m_serialHndl );

        m_serialHndl = INVALID_HANDLE_VALUE;
    }

} // Close

//***************************************************************************
/**
*   Opens a serial port.
*/

bool SerialPort::Open( const char *devName, const char *param )
{
    DWORD   err;
    char    errStr[ 200 ];
    char    dcbParam[ 40 ];

    LogVerbose( "Port: '%s' Param: %s\n", devName, param );

    m_serialHndl = CreateFile( devName,                       // lpFileName
                               GENERIC_READ | GENERIC_WRITE,  // dwDesiredAccess
                               0,                             // dwShareMode
                               NULL,                          // lpSecurityAttributes
                               OPEN_EXISTING,                 // dwCreationDisposition
                               0,                             // dwFlagsAndAttributes
                               NULL );                        // hTemplateFile

    if ( m_serialHndl == INVALID_HANDLE_VALUE )
    {
        err = GetLastError();

        LogError( "Unable to open COM port '%s'.\n", devName );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        return false;
    }

    DCB			    dcb;
    COMMTIMEOUTS    timeouts;

    memset( &dcb, 0, sizeof( dcb ));
    dcb.DCBlength = sizeof( dcb );

    memset( &timeouts, 0, sizeof( timeouts ));

    if ( !GetCommState( m_serialHndl, &dcb ))
    {
        err = GetLastError();

        LogError( "Call to GetCommState failed for  '%s'.\n", devName );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
        return false;
    }

    if ( !GetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LogError( "Call to GetCommTimeouts failed for  '%s'.\n", devName );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

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

    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;

    //  COM1: baud=9600 parity=N data=8 stop=1

    snprintf( dcbParam, sizeof( dcbParam ), "baud=%s", param );

    if ( !BuildCommDCBAndTimeouts( dcbParam, &dcb, &timeouts ))
    {
        err = GetLastError();

        LogError( "Unable to parse COM parameters: '%s'.\n", param );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

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

        LogError( "Call to SetCommState failed for  '%s'.\n", devName );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
        return false;
    }

    if ( !SetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LogError( "Call to SetCommTimeouts failed for  '%s'.\n", devName );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        Close();
        return false;
    }

    return true;

} // Open

//***************************************************************************
/**
*   Reads data from the serial port.
*/

size_t SerialPort::Read( void *buf, size_t bufSize )
{
    DWORD   bytesRead;
    DWORD   err;
    char    errStr[ 200 ];

    if ( m_serialHndl == INVALID_HANDLE_VALUE )
    {
        return 0;
    }
    GetLastError();
    SetLastError( 0 );

    if ( !ReadFile( m_serialHndl, buf, bufSize, &bytesRead, NULL ))
    {
        err = GetLastError();

        LogError( "SerialPort::Read failed: %s\n", GetErrorStr( err, errStr, sizeof( errStr )));

        return 0;
    }

#if 0
    if ( bytesRead == 0 )
    {
        err = GetLastError();
        Log( "************* Read returning 0 %s", GetErrorStr( err, errStr, sizeof( errStr)) );
    }
#endif

    return bytesRead;

} // Read

//***************************************************************************
/**
*   Sets the timeout to use when waiting for data. 0 = infinite
*   The timeout is specified in milliseconds.
*/
bool SerialPort::SetTimeout( unsigned timeout )
{
    DWORD           err;
    char            errStr[ 200 ];
    COMMTIMEOUTS    timeouts;

    if ( m_serialHndl == INVALID_HANDLE_VALUE )
    {
        return false;
    }

    if ( !GetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LogError( "Call to GetCommTimeouts failed.\n" );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        return false;
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutMultiplier = timeout;

    if ( !SetCommTimeouts( m_serialHndl, &timeouts ))
    {
        err = GetLastError();

        LogError( "Call to SetCommTimeouts failed.\n" );
        LogError( "%s", GetErrorStr( err, errStr, sizeof( errStr )));

        return false;
    }

    return true;

} // SetTimeout

//***************************************************************************
/**
*   Writes data to the serial port.
*/

size_t SerialPort::Write( const void *buf, size_t bytesToWrite )
{
    DWORD   bytesWritten;
    DWORD   err;
    char    errStr[ 200 ];

    if ( !WriteFile( m_serialHndl, buf, bytesToWrite, &bytesWritten, NULL ))
    {
        err = GetLastError();

        LogError( "SerialPort::Write failed: %s\n", GetErrorStr( err, errStr, sizeof( errStr )));

        return 0;
        
    }

    return bytesWritten;

} // Write

//***************************************************************************
/**
*   Resets the target
*/

void SerialPort::StrobeRTS( int strobeWidthInMsec )
{
    (void)strobeWidthInMsec;

    LogError( "SerialPort::StrobeRTS not implemented yet\n" );

} // StrobeRTS

/** @} */

