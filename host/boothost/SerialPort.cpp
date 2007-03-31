/****************************************************************************
*
*     Copyright (c) 2005 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   SerialPort.cpp
*
*   @brief  This file implements the SerialPort class.
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
    { B230400,  230400 }
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
            fprintf( stderr, "Unrecognized baud rate: '%s'\n", param );
            return false;
        }
    }

    LogVerbose( "Port: '%s' Baud: %d\n", devName, baudRate );

    if (( m_fd = open( devName, O_RDWR | O_EXCL )) < 0 )
    {
        fprintf( stderr, "Unable to open serial port '%s': %s\n", devName, strerror( errno ));
        return false;
    }

    // Setup the serial port

    struct termios  attr;

    if ( tcgetattr( m_fd, &attr ) < 0 )
    {
        fprintf( stderr, "Call to tcgetattr failed: %s\n", strerror( errno ));
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
        fprintf( stderr, "Call to tcsetattr failed: %s\n", strerror( errno ));
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

            fprintf( stderr, "read failed: %d\n", errno );
        }
        return 0;
    }

    return bytesRead;

} // Read

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
        fprintf( stderr, "write failed: %d\n", errno );
        bytesWritten = 0;
    }

    return bytesWritten;

} // Write

//***************************************************************************
/**
*   Resets the target
*/

void SerialPort::ResetTarget()
{
    int bits = TIOCM_RTS;

    if ( m_useRTStoReset )
    {
        // On my computer downstairs, settinging the RTS line makes it low.

        ioctl( m_fd, TIOCMBIS, &bits );

        // Sleep for 10 msec to allow the reset to take effect.

        usleep( 10000 );
        
        // On my computer downstairs, clearing the RTS line makes it high.

        ioctl( m_fd, TIOCMBIC, &bits );
    }

} // ResetTarget

/** @} */

