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
****************************************************************************
*
*   User mode app which monitors multiple gpio pins and notifies a user mode
*   app about the events.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

//#include "svn-version.h"

#if !defined( SVN_REVISION )
#   define SVN_REVISION  0
#endif

#include "gpio-event-drv.h"

/* ---- Public Variables ------------------------------------------------- */

int     gDebug      = 0;
int     gVerbose    = 0;
int     gBinary     = 0;
int     gMonitor    = 0;
char   *gExecuteStr = NULL;
int     gSelect     = 0;

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

enum
{
    // Options assigned a single character code can use that charater code
    // as a short option.

    OPT_BINARY      = 'b',
    OPT_DEBUG       = 'd',
    OPT_EXECUTE     = 'e',
    OPT_MONITOR     = 'm',
    OPT_VERBOSE     = 'v',
    OPT_VERSION     = 'V',
    OPT_SELECT      = 's',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,

    OPT_HELP,
};

struct option gOption[] =
{
    { "binary",     no_argument,        NULL,       OPT_BINARY },
    { "execute",    required_argument,  NULL,       OPT_EXECUTE },
    { "monitor",    no_argument,        NULL,       OPT_MONITOR },
    { "select",     no_argument,        NULL,       OPT_SELECT },
    { "version",    no_argument,        NULL,       OPT_VERSION },
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { "debug",      no_argument,        NULL,       OPT_DEBUG },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { NULL }
};


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Usage
*/

static void Usage( void )
{
    fprintf( stderr, "Usage: gpio-event [options] gpio[:edge[:debounce]] ...\n" );
    fprintf( stderr, "  where edge can be rising, falling, both, or r, f, b\n" );
    fprintf( stderr, "  -b, --binary        Use the binary interface\n" );
    fprintf( stderr, "  -e, --execute CMD   Execute the given program\n" );
    fprintf( stderr, "  -m, --monitor       Monitor the gpio events\n" );
    fprintf( stderr, "  -s, --select        Wait for event using select\n" );
    fprintf( stderr, "  -V. --version       Print the version information for this program\n" );
    fprintf( stderr, "  -v, --verbose       Print additional information\n" );
    fprintf( stderr, "  -h, --help          Prints this information\n" );

} // Usage

/****************************************************************************
*
*  main
*
***************************************************************************/

int main( int argc, char **argv )
{
    char                shortOptsStr[ sizeof( gOption ) / sizeof( gOption[ 0 ] ) + 1 ];
    char               *shortOpts = shortOptsStr;
    struct option      *scanOpt;
    int                 opt;
    int                 arg;
    FILE               *fs;

    // Figure out the short options from our options structure

    for ( scanOpt = gOption; scanOpt->name != NULL; scanOpt++ ) 
    {
        if (( scanOpt->flag == NULL ) && ( scanOpt->val < OPT_FIRST_LONG_OPT ))
        {
            *shortOpts++ = (char)scanOpt->val;

            if ( scanOpt->has_arg != no_argument )
            {
                *shortOpts++ = ':';
            }
        }
    }
    *shortOpts++ = '\0';

    // Parse the command line options

    while (( opt = getopt_long( argc, argv, shortOptsStr, gOption, NULL )) != -1 )
    {
        switch ( opt )
        {
            case 0: 
            {
                // getopt_long returns 0 for entries where flag is non-NULL

                break;
            }

            case OPT_BINARY:
            {
                gBinary = 1;
                break;
            }

            case OPT_DEBUG:
            {
                gDebug = 1;
                break;
            }

            case OPT_EXECUTE:
            {
                gExecuteStr = optarg;
                break;
            }

            case OPT_MONITOR:
            {
                gMonitor = 1;
                break;
            }

            case OPT_SELECT:
            {
                gSelect = 1;
                break;
            }

            case OPT_VERBOSE:
            {
                gVerbose = 1;
                break;
            }

            case OPT_VERSION:
            {   
                printf( "gpio-event: SVN Revision: %d\n", SVN_REVISION );
                exit( 0 );
            }

            case '?':
            default:
            {
                fprintf( stderr, "opt:%d\n", opt );
            }
            case OPT_HELP:
            {
                Usage();
                exit( 1 );
            }
        }
    }
    argc -= optind;
    argv += optind;

    if (( fs = fopen( "/dev/gpio-event", "r" )) == NULL )
    {
        perror( "Check to make sure gpio_event_drv has been loaded. Unable to open /dev/gpio-event" );
        exit( 1 );
    }

    if ( gBinary )
    {
        ioctl( fileno( fs ), GPIO_EVENT_IOCTL_SET_READ_MODE, gBinary );
    }

    // Parse the gpio pins. Each pin can be followed by a modifier to indicate
    // the type of edges to monitor.

    for ( arg = 0; arg < argc; arg++ ) 
    {
        long                    gpio;
        char                   *gpioStr = argv[ arg ];
        char                   *endPtr;
        GPIO_EventMonitor_t     monitor;

        gpio = strtol( gpioStr, &endPtr, 0 );
        if ( gpio < 0 )
        {
            monitor.gpio = -gpio;
            monitor.onOff = 0;
        }
        else
        {
            monitor.gpio = gpio;
            monitor.onOff = 1;
        }

        if ( monitor.gpio > 255 )
        {
            fprintf( stderr, "Expecting gpio to be in the range 1-255, found: %d\n", monitor.gpio );
            exit( 1 );
        }
        if ( !ispunct( *endPtr ))
        {
            fprintf( stderr, "Expecting punctuation character, found '%c'\n", *endPtr );
            exit( 1 );
        }
        endPtr++;

        switch ( *endPtr )
        {
            case 'r':
            case 'R':   monitor.edgeType = GPIO_EventRisingEdge;    break;

            case 'f':
            case 'F':   monitor.edgeType = GPIO_EventFallingEdge;   break;

            case 'b':
            case 'B':   monitor.edgeType = GPIO_EventBothEdges;     break;

            default:
            {
                fprintf( stderr, "Expecting r, f, or b, found '%c'\n", *endPtr );
                exit( 1 );
            }
        }

        monitor.debounceMilliSec = 0;
        while ( isalpha( *endPtr ))
        {
            endPtr++;
        }
        if ( *endPtr != '\0' )
        {
            if ( !ispunct( *endPtr ))
            {
                fprintf( stderr, "Expecting punctuation character, found '%c'\n", *endPtr );
                exit( 1 );
            }
            endPtr++;

            monitor.debounceMilliSec = strtol( endPtr, NULL, 0 );
        }

        if ( ioctl( fileno( fs ), GPIO_EVENT_IOCTL_MONITOR_GPIO, &monitor ) != 0 )
        {
            perror( "ioctl GPIO_EVENT_IOCTL_MONITOR_GPIO failed" );
        }
    }

    if ( gMonitor || ( gExecuteStr != NULL ))
    {
        while ( 1 )
        {
            ssize_t numBytes;
            char    argStr[ 60 ];

            argStr[ 0 ] = '\0';

            if ( gSelect )
            {
                fd_set          readSet;
                struct  timeval tv;
                int     rc;

                printf( "Waiting for data " );
                fflush( stdout );

                while ( 1 )
                {
                    FD_ZERO( &readSet );
                    FD_SET( fileno( fs ), &readSet );
    
                    tv.tv_sec = 1;
                    tv.tv_usec = 0;
    
                    rc = select( fileno( fs ) + 1, &readSet, NULL, NULL, &tv );
    
                    if ( rc == -1 )
                    {
                        perror( "select failed" );
                    }
                    else
                    if ( rc > 0 )
                    {
                        // Data is available

                        break;
                    }
                    else
                    {
                        printf( "." );
                        fflush( stdout );
                    }
                }
                printf( "\n" );
            }

            if ( gBinary )
            {
                GPIO_Event_t    gpioEvent;

                if (( numBytes = fread( &gpioEvent, 1, sizeof( gpioEvent ), fs )) == sizeof( gpioEvent ))
                {
                    snprintf( argStr, sizeof( argStr ), "%2d %c %ld.%06ld",
                              gpioEvent.gpio,
                              (( gpioEvent.edgeType == GPIO_EventRisingEdge ) ? 'R' : 'F' ),
                              gpioEvent.time.tv_sec,
                              gpioEvent.time.tv_usec );

                    if ( gMonitor )
                    {
                        printf( "ReadB: '%s'\n", argStr );
                    }
                }
                else
                {
                    if ( numBytes > 0 )
                    {
                        fprintf( stderr, "Read unexpected number of bytes: %d, expecting %d\n",
                                 numBytes, sizeof( gpioEvent ));
                    }
                }
            }
            else
            {
                if ( fgets( argStr, sizeof( argStr ), fs ) != NULL )
                {
                    numBytes = strlen( argStr );

                    if ( argStr[ numBytes - 1 ] == '\n' )
                    {
                        argStr[ numBytes - 1 ] = '\0';
                    }

                    if ( gMonitor )
                    {
                        printf( "Read: '%s'\n", argStr );
                    }
                }
                else
                {
                    argStr[ 0 ] = '\0';
                }
            }

            if (( gExecuteStr != NULL ) && ( argStr[ 0 ] != '\0' ))
            {
                char    cmdStr[ 256 ];

                snprintf( cmdStr, sizeof( cmdStr ), "%s %s", gExecuteStr, argStr );

                if ( gVerbose )
                {
                    printf( "Executing '%s'\n", cmdStr );
                }
                system( cmdStr );
            }
        }
    }

    fclose( fs );

    exit( 0 );
    return 0;

} // main


