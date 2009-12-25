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
*   @file   bioloid.cpp
*
*   @brief  Implements a PC version of the bioloid command line interface
*           which allows bioloid commands to be sent to either a real
*   bioloid bus, or a simulated one.
*
****************************************************************************/

#if defined( AVR )
#define USE_COMMAND_LINE    0
#else
#define USE_COMMAND_LINE    1
#endif

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#if USE_COMMAND_LINE
#include <getopt.h>
#endif

#include "Log.h"
#include "BioloidBus.h"
#include "BioloidDevice.h"
#include "BioloidCommandLine.h"
#include "bioloid-reg-servo.h"
#include "SerialPort.h"
#include "SerialBus.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#define DEFAULT_BAUD    "1000000"

#if defined( linux )
#   define  DEFAULT_PORT    "ttyUSB0"
#else
#   define  DEFAULT_PORT    "com1"
#endif

// ---- Private Variables ---------------------------------------------------

enum
{
    // Options assigned a single character code can use that charater code
    // as a short option.

    OPT_BAUD        = 'b',
    OPT_DEBUG       = 'd',
    OPT_PORT        = 'p',
    OPT_VERBOSE     = 'v',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,

    OPT_HELP,
};

#if USE_COMMAND_LINE
static struct option gOption[] =
{
    { "baud",       required_argument,  NULL,       OPT_BAUD },
    { "debug",      no_argument,        NULL,       OPT_DEBUG },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { "port",       required_argument,  NULL,       OPT_PORT },
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { NULL }
};
#endif

SerialPort      gSerialPort;
SerialBus       gSerialBus;
BioloidDevice   gDev( &gSerialBus, 1 );

static BLD_DevType_t    gDevType[] =
{
    { "servo",   BLD_gServoReg },
    { NULL, NULL },
};

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*   Print out the program usage.
*/

#if USE_COMMAND_LINE
void Usage()
{
    fprintf( stderr, "Usage: bioloid <option(s)> <file>\n" );
    fprintf( stderr, "  Send commands to bioloid devices\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -b, --baud=baud   Set the baudrate used\n" );
    fprintf( stderr, "  -p, --port=name   Set the serial port to use\n" );
    fprintf( stderr, "  -d, --debug       Enable debug features\n" );
    fprintf( stderr, "  -h, --help        Display this message\n" );
    fprintf( stderr, "  -v, --verbose     Turn on verbose messages\n" );

} // Usage
#endif

//***************************************************************************
/**
*   main
*/

int main( int argc, char **argv )
{
#if USE_COMMAND_LINE
    char                shortOptsStr[ sizeof( gOption ) / sizeof( gOption[ 0 ] ) + 1 ];
    char               *shortOpts = shortOptsStr;
    struct option      *scanOpt;
    int                 opt;
    int                 arg;
    const char         *baudStr = DEFAULT_BAUD;
    const char         *portStr = DEFAULT_PORT;
#endif
    char                line[ 80 ];
    BioloidCommandLine  cmdLine;

#if USE_COMMAND_LINE
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

            case OPT_BAUD:
            {
                baudStr = optarg;
                break;
            }

            case OPT_DEBUG:
            {
                gDebug = 1;
                break;
            }

            case OPT_PORT:
            {
                portStr = optarg;
                break;
            }

            case OPT_VERBOSE:
            {
                gVerbose = 1;
                break;
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

    LogDebug( "Debug enabled\n" );
    LogVerbose( "Verbose enabled\n" );
    LogVerbose( "Port: %s Baud: %s\n", portStr, baudStr );
#endif

    if ( !gSerialPort.Open( portStr, baudStr ))
    {
        LogError( "Unable to open serial port '%s'", portStr );
        exit( 1 );
    }
    gSerialBus.SetSerialPort( &gSerialPort );
    gSerialBus.SetDebug( gDebug != 0 );

    cmdLine.RegisterDevices( gDevType );

    cmdLine.SetBus( &gSerialBus );

    printf( "> " );
    while ( fgets( line, sizeof( line ), stdin ) != NULL )
    {
        fflush( stdout );
        if ( !cmdLine.ProcessLine( line ))
        {
            break;
        }

        printf( "> " );
    }

    exit( 0 );
    return 0;
}


