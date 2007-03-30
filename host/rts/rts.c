/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
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
*  rts.c
*
*  PURPOSE:
*
*   Test program for toggling the RTS line on a serial port.
*
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <pthread.h>
#include <signal.h>
#include <getopt.h>
#include <termios.h>

/* ---- Public Variables ------------------------------------------------- */

int gFd = -1;

int gVal;

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************
*
*  main
*
****************************************************************************/

int main( int argc, char **argv )
{
    int     fd;
    int     rts = 0;
    char    line[ 200 ];
    char   *devName = argv [1];
    struct  termios attr;

    if ( argc != 2 )
    {
        fprintf( stderr, "Usage: rts port\n" );
        exit( 1 );
    }

    if (( fd = open( devName, O_RDWR | O_EXCL | O_NOCTTY | O_NDELAY )) < 0 )
    {
        fprintf( stderr, "Unable to open serial port '%s': %s\n", devName, strerror( errno ));
        exit( 2 );
    }
    fcntl( fd, F_SETFL, O_NONBLOCK );

    if ( tcgetattr( fd, &attr ) < 0 )
    {
        fprintf( stderr, "Call to tcgetattr failed: %s\n", strerror( errno ));
        exit( 3 );
    }

#if 1
    attr.c_iflag = IGNBRK | IGNPAR;
    attr.c_oflag = 0;
    attr.c_cflag = CLOCAL | CREAD | CS8 | B9600;
    attr.c_cc [VMIN] = 1;
    attr.c_cc [VTIME] = 0;
#else
    cfmakeraw( &attr );
    attr.c_cflag |= ( CLOCAL | CREAD );
    attr.c_cflag &= ~CRTSCTS;
#endif

    if ( tcsetattr( fd, TCSAFLUSH, &attr ) < 0 )
    {
        fprintf( stderr, "Call to tcsetattr failed: %s\n", strerror( errno ));
        exit( 4 );
    }

    printf( "Press RETURN to toggle the RTS state\n" );
    while ( fgets( line,  sizeof( line), stdin ) != NULL )
    {
        int bits;

        if ( ioctl( fd, TIOCMGET, &bits ) != 0 )
        {
            fprintf( stderr, "ioctl TIOCMGET failed\n" );
        }

        rts = !rts;

        bits = TIOCM_RTS;
        if ( rts )
        {
            printf( "Setting RTS, CTS = %d\n", ( bits & TIOCM_CTS ) != 0 );
            if ( ioctl( fd, TIOCMBIS, &bits ) != 0 )
            {
                fprintf( stderr, "ioctl TIOCMSET failed\n" );
            }
        }
        else
        {
            printf( "Clearing RTS, CTS = %d\n", ( bits & TIOCM_CTS ) != 0 );
            if ( ioctl( fd, TIOCMBIC, &bits ) != 0 )
            {
                fprintf( stderr, "ioctl TIOCMSET failed\n" );
            }
        }
    }

    close( fd );

} // main
