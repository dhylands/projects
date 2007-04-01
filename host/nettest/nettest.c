/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   wln-net.c
*
*   @brief  This file implements a simple network client for connecting
*           to the DPAC airborne Wifi unit.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <getopt.h>
#include <netdb.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

struct option gLongOption[] =
{
    // option       A  Flag   V  (has_arg, flag, val)
    // -----------  -  ----  ---
    { "debug",      0, NULL, 'd' },
    { "help",       0, NULL, 'h' },
    { "verbose",    0, NULL, 'v' },
    { 0 },

};

int gVerbose = 0;
int gDebug = 0;

int gSocket;

/* ---- Private Function Prototypes --------------------------------------- */

void *ReadNetThread( void *param );
void *ReadStdinThread( void *param );
void  Usage( void );

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Main program
*/

int main( int argc, char **argv )
{
    int         sig;
    int         rc;
    int         opt;
    char       *hostname;
    char       *portStr;
    in_port_t   port;
    sigset_t    termSig;
    pthread_t   readNetThreadId;
    pthread_t   readStdinThreadId;
    struct termios      stdin_tio;
    struct termios      stdin_tio_org;
    struct hostent     *hostent;
    struct sockaddr_in  saddr;

    // Parse the command line options

    while (( opt = getopt_long( argc, argv, "dhv", gLongOption, NULL )) > 0 )
    {
        switch ( opt )
        {
            case 'd':
            {
                gDebug = 1;
                break;
            }

            case 'v':
            {
                gVerbose = 1;
                break;
            }
            case '?':
            case 'h':
            {
                Usage();
                return 1;
            }
        }
    }

    if (( optind + 2 ) != argc )
    {
        fprintf( stderr, "Must specify hostname and port\n" );
        exit( 1 );
    }

    hostname = argv[ optind++ ];
    portStr  = argv [optind++ ];
    port = atoi( portStr );

    if (( hostent = gethostbyname( hostname )) == NULL )
    {
        fprintf( stderr, "Unable to translate hostname '%s' into an IP address\n", hostname );
        exit( 1 );
    }
    memset( &saddr, 0, sizeof( saddr ));

    saddr.sin_addr.s_addr = *(in_addr_t *)hostent->h_addr_list[ 0 ];
    saddr.sin_port = htons( port );
    saddr.sin_family = AF_INET;

    if (( gSocket = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
    {
        fprintf( stderr, "Call to socket failed: '%s'\n", strerror( errno ));
        exit( 1 );
    }

    if ( connect( gSocket, (struct sockaddr *)&saddr, sizeof( saddr )) < 0 )
    {
        fprintf( stderr, "Unable to connect to %s:%d\n",  hostname, port );
        exit( 1 );
    }

    // Put stdin & stdout in unbuffered mode.

    setbuf( stdin, NULL );
    setbuf( stdout, NULL );

    sigemptyset( &termSig );
    sigaddset( &termSig, SIGINT );
    sigaddset( &termSig, SIGTERM );

    pthread_sigmask( SIG_BLOCK, &termSig, NULL );

    // Put stdin in raw mode (i.e. turn off canonical mode). Canonical mode
    // causes the driver to wait for the RETURN character so that line editing
    // can take place. We also want to turn off ECHO.

    if ( tcgetattr( fileno( stdin ), &stdin_tio_org ) < 0 )
    {
        fprintf( stderr, "Unable to retrieve terminal settings: %s\n", strerror( errno ));
        exit( 5 );
    }

    stdin_tio = stdin_tio_org;
    stdin_tio.c_lflag &= ~( ICANON | ECHO );
    stdin_tio.c_iflag &= ~( INLCR | ICRNL );
    stdin_tio.c_cc[VTIME] = 0;
    stdin_tio.c_cc[VMIN] = 1;

    if ( tcsetattr( fileno( stdin ), TCSANOW, &stdin_tio ) < 0 )
    {
        fprintf( stderr, "Unable to update terminal settings: %s\n", strerror( errno ));
        exit( 6 );
    }

    // Kick off the network port reader thread.

    rc = pthread_create( &readNetThreadId, NULL, ReadNetThread, NULL );
    if ( rc != 0 )
    {
        fprintf( stderr, "Error creating ReadNetThread: %s\n", strerror( rc ));
        exit( 7 );
    }

    // Kick off the stdin reader thread

    rc = pthread_create( &readStdinThreadId, NULL, ReadStdinThread, NULL );
    if ( rc != 0 )
    {
        fprintf( stderr, "Error creating ReadStdinThread: %s\n", strerror( rc ));
        exit( 7 );
    }

    // Wait for a termmination signal

    if (( rc = sigwait( &termSig, &sig )) != 0 )
    {
        fprintf( stderr, "sigwait failed\n" );
    }
    else
    {
        fprintf( stderr, "Exiting...\n" );
    }

    pthread_cancel( readNetThreadId );
    pthread_cancel( readStdinThreadId );

    // Restore stdin back to the way it was when we started

    if ( tcsetattr( fileno( stdin ), TCSANOW, &stdin_tio_org ) < 0 )
    {
        fprintf( stderr, "Unable to update terminal settings: %s\n", strerror( errno ));
        exit( 6 );
    }

    // Unblock the termination signals so the user can kill us if we hang up
    // waiting for the reader threads to exit.

    pthread_sigmask( SIG_UNBLOCK, &termSig, NULL );

    pthread_join( readNetThreadId, NULL );
    pthread_join( readStdinThreadId, NULL );

    close( gSocket );

    if ( gVerbose )
    {
        fprintf( stderr, "Done\n" );
    }

    exit( 0 );
    return 0;   // Get rid of warning about not returning anything

} // main

/***************************************************************************/
/**
*   Thread which processes the incoming network data.
*/

void *ReadNetThread( void *param )
{
    while ( 1 )
    {
        char    ch;
        int     bytesRecv;

        if (( bytesRecv  = recv( gSocket, &ch, 1, 0 )) < 0 )
        {
            fprintf( stderr, "TCP recv failed: %s\n", strerror( errno ));
            exit( 1 );
        }

        if ( gDebug )
        {
            if (( ch < ' ' ) || ( ch > '~' ))
            {
                fprintf( stderr, "Net Read: 0x%02x '.'\n", ch );
            }
            else
            {
                fprintf( stderr, "Net Read: 0x%02x '%c'\n", ch, ch );
            }

        }

        putc( ch, stdout );
    }

    return NULL;

} // ReadNetThread

/***************************************************************************/
/**
*   Thread which processes the incoming serial from stdin.
*/

void *ReadStdinThread( void *param )
{
    while ( 1 )
    {
        char    ch;
        int     chInt = fgetc( stdin );
        if ( chInt < 0 )
        {
            fprintf( stderr, "Error reading stdin...\n" );
            break;
        }
        ch = (char)chInt;

        if ( gDebug )
        {
            if (( ch < ' ' ) || ( ch > '~' ))
            {
                fprintf( stderr, "stdin Read: 0x%02x '.'\n", ch );
            }
            else
            {
                fprintf( stderr, "stdin Read: 0x%02x '%c'\n", ch, ch );
            }

        }

        if ( send( gSocket, &ch, 1, 0 ) != 1 )
        {
            fprintf( stderr, "send to network failed: %s\n", strerror( errno ));
            break;
        }
    }

    return NULL;

} // ReadStdinThread

//***************************************************************************
/**
*  Usage
*/

void Usage()
{
    fprintf( stderr, "Usage: wln-net [option(s)] host port\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "Connect to a network port\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -d, --debug       Turn on debug output\n" );
    fprintf( stderr, "  -h, --help        Display this message\n" );
    fprintf( stderr, "  -v, --verbose     Turn on verbose messages\n" );

} // Usage


