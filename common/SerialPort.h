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
*   @file   SerialPort.h
*
*   @brief  This file contains the definitions for the SerialPort class.
*
****************************************************************************/
/**
*   @defgroup   SerialPort   Serial Port
*
*   @brief      Performs serial communications using Unix style serial ports.
*
****************************************************************************/

#if !defined( SERIALPORT_H )
#define SERIALPORT_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stddef.h>
#include <stdint.h>

#if defined( WIN32 )
#include <windows.h>
#endif

/**
 * @addtogroup SerialPort
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The SerialPort class encapsulates serial port communcations using
*   unix style serial communications. This also works under cygwin (for Win32)
*/

class SerialPort
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    SerialPort();

    //------------------------------------------------------------------------
    // Destructor

    ~SerialPort();

    //------------------------------------------------------------------------
    // Opens a serial port.

    bool Open( const char *devName, const char *param = NULL );

    //------------------------------------------------------------------------
    // Closes a previsouly opened serial port.

    void Close();

    //------------------------------------------------------------------------
    //  Reads data from the serial port.

    size_t Read( void *buf, size_t bytesToRead );

    //------------------------------------------------------------------------
    //  Sets the timeout to use when waiting for data. 0 = infinite
    //  The timeout is specified in milliseconds.

    bool SetTimeout( unsigned timeout );

    //------------------------------------------------------------------------
    //  Writes data to the serial port.

    size_t Write( const void *buf, size_t bytesToWrite );

    //------------------------------------------------------------------------
    //  Strobes the RTS line.

    void StrobeRTS( int strobeWidthInMsec );

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    SerialPort( const SerialPort & copy );
    SerialPort &operator =( const SerialPort &rhs );

    //------------------------------------------------------------------------

#if defined( WIN32 )
    HANDLE  m_serialHndl;
#else
    int     m_fd;
#endif
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // SERILAPORT_H

