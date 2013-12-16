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

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/unistd.h>
#include <string.h>
#include <sys/ioctl.h>

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

static struct
{
    speed_t     speed;
    unsigned    baudRate;
} gBaudTable[] =
{
    { B50,          50 },
    { B75,          75 },
    { B110,        110 },
    { B134,        134 },
    { B150,        150 },
    { B200,        200 },
    { B300,        300 },
    { B600,        600 },
    { B1200,      1200 },
    { B1800,      1800 },
    { B2400,      2400 },
    { B4800,      4800 },
    { B9600,      9600 },
    { B19200,    19200 },
    { B38400,    38400 },
    { B57600,    57600 },
    { B115200,  115200 },
    { B230400,  230400 },
    { B1000000,1000000 }
};

#define ARRAY_LEN(x)    ( sizeof( x ) / sizeof( x[ 0 ]))

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
    : m_fd( -1 )
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
    if ( m_fd >= 0 )
    {
        close( m_fd );

        m_fd = -1;
    }

} // Close

//***************************************************************************
/**
*   Opens a serial port.
*/

bool SerialPort::Open( const char *inDevName, const char *param )
{
    char        devName[ 40 ];
    unsigned    baudRate;

    devName[ 0 ] = '\0';

#if 1
    if ( inDevName[ 0 ] != '/' )
    {
        StrMaxCpy( devName, "/dev/", sizeof( devName ));
    }
#endif
    StrMaxCat( devName, inDevName, sizeof( devName ));

    // Translate the params, if any

    speed_t speed = B0;
    if ( param == NULL )
    {
        speed = B38400;
        baudRate = 38400;
    }
    else
    {
        baudRate = atoi( param );

        for ( unsigned i = 0; i < ARRAY_LEN( gBaudTable ); i++ )
        {
            if ( gBaudTable[ i ].baudRate == baudRate )
            {
                speed = gBaudTable[ i ].speed;
                break;
            }
        }

        if ( speed == B0 )
        {
            LogError( "Unrecognized baud rate: '%s'\n", param );
            return false;
        }
    }

    LogVerbose( "Port: '%s' Baud: %d\n", devName, baudRate );

    if (( m_fd = open( devName, O_RDWR | O_EXCL )) < 0 )
    {
        LogError( "Unable to open serial port '%s': %s\n", devName, strerror( errno ));
        return false;
    }

    // Setup the serial port

    struct termios  attr;

    if ( tcgetattr( m_fd, &attr ) < 0 )
    {
        LogError( "A: Call to tcgetattr failed: %s\n", strerror( errno ));
        return false;
    }

    attr.c_iflag = 0;
    attr.c_oflag = 0;
    attr.c_cflag = CLOCAL | CREAD | CS8;
    attr.c_lflag = 0;
    attr.c_cc[ VTIME ] = 0;   // timeout in tenths of a second
    attr.c_cc[ VMIN ] = 1;

    cfsetispeed( &attr, speed );
    cfsetospeed( &attr, speed );

    if ( tcsetattr( m_fd, TCSAFLUSH, &attr ) < 0 )
    {
        LogError( "Call to tcsetattr failed: %s\n", strerror( errno ));
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
    int  bytesRead;

    if ( m_fd < 0 )
    {
        return 0;
    }

    bytesRead = read( m_fd, buf, bufSize );
    if ( bytesRead < 0 )
    {
        if ( errno != EBADF )
        {
            // We get EBADF returned if the serial port is closed on
            // us.

            LogError( "read failed: %d\n", errno );
        }
        return 0;
    }

    return bytesRead;

} // Read

//***************************************************************************
/**
*   Sets the timeout to use when waiting for data. 0 = infinite
*   The timeout is specified in milliseconds.
*/
bool SerialPort::SetTimeout( unsigned timeout )
{
    struct termios  attr;

    if ( m_fd < 0 )
    {
        return false;
    }

    if ( tcgetattr( m_fd, &attr ) < 0 )
    {
        LogError( "B: Call to tcgetattr failed: %s\n", strerror( errno ));
        return false;
    }

    if ( timeout == 0 )
    {
        attr.c_cc[ VTIME ] = 0;
        attr.c_cc[ VMIN ]  = 1;
    }
    else
    {
        // Note. termios only supports timeouts specified in tenths of
        // a second, so we need convert from milliseconds.

        timeout = ( timeout + 99 ) / 100;
        if ( timeout > 255 )
        {
            timeout = 255;
        }

        attr.c_cc[ VTIME ] = (uint8_t)timeout;   // timeout in tenths of a second
        attr.c_cc[ VMIN ]  = 0;
    }

    if ( tcsetattr( m_fd, TCSAFLUSH, &attr ) < 0 )
    {
        LogError( "Call to tcsetattr failed: %s\n", strerror( errno ));
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
    int  bytesWritten;

    bytesWritten = write( m_fd, buf, bytesToWrite );
    if ( bytesWritten < 0 )
    {
        LogError( "write failed: %d\n", errno );
        bytesWritten = 0;
    }

    return bytesWritten;

} // Write

//***************************************************************************
/**
*   Resets the target using the DTR and CTS line
*/

void SerialPort::StrobeDTRRTS()
{
    int rc;
    unsigned int status;

    rc = ioctl(m_fd, TIOCMGET, &status);
    if ( rc < 0 )
    {
        perror("ioctl('TIOCMGET')");
        return;
    }
    printf("Clearing DTR/RTS\n");
    status &= ~(TIOCM_DTR | TIOCM_RTS);
    rc = ioctl(m_fd, TIOCMSET, &status);
    if ( rc < 0 )
    {
        perror("ioctl('TIOCMSET')");
        return;
    }
    usleep(250 * 1000);

    printf("Setting DTR/RTS\n");
    status |= (TIOCM_DTR | TIOCM_RTS);
    rc = ioctl(m_fd, TIOCMSET, &status);
    if ( rc < 0 )
    {
        perror("ioctl('TIOCMSET')");
        return;
    }
    usleep(50 * 1000);

} // StrobeDTRRTS

/** @} */

