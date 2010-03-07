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
#define USE_NETBUS          0
#else
#define USE_COMMAND_LINE    1
#if defined( linux )
#define USE_NETBUS          0
#else
#define USE_NETBUS          1
#endif
#endif

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#if USE_COMMAND_LINE
#include <getopt.h>
#endif

#include <libgen.h>

#include "Log.h"
#include "Str.h"
#include "BioloidBus.h"
#include "BioloidDevice.h"
#include "BioloidCommandLine.h"
#include "bioloid-reg-servo.h"
#include "SerialPort.h"
#include "SerialBus.h"
#include "DevTypeParser.h"

#if USE_NETBUS
#   include "NetBus.h"
#endif

#if defined( WIN32 )
#   include <windows.h>
#else
#   include <fnmatch.h>
#   include <dirent.h>
#endif

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
    OPT_NET         = 'n',
    OPT_VERBOSE     = 'v',
    OPT_HELP        = 'h',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,

};

#if USE_COMMAND_LINE
static struct option gOption[] =
{
    { "baud",       required_argument,  NULL,       OPT_BAUD },
    { "debug",      no_argument,        NULL,       OPT_DEBUG },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { "port",       required_argument,  NULL,       OPT_PORT },
    { "net",        required_argument,  NULL,       OPT_NET },
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { NULL }
};
#endif

SerialPort      gSerialPort;
SerialBus       gSerialBus;

bool            gUseSerial = false;

#if USE_NETBUS
NetBus          gNetBus;
bool            gUseNet = false;
#endif

#define MAX_DEV_TYPES   20

static unsigned         gNumDevTypes; 
static BLD_DevType_t   *gDevType[ MAX_DEV_TYPES ];

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*   Adds a device type to the list of device types that we know about.
*/
void AddDevType( BLD_DevType_t *devType )
{
    unsigned    devTypeIdx;

    if ( gNumDevTypes >= MAX_DEV_TYPES )
    {
        LogError( "Too many device types (max of %d)\n", MAX_DEV_TYPES );
        return;
    }

    // Make sure this isn't a duplicate

    for ( devTypeIdx = 0; devTypeIdx < gNumDevTypes; devTypeIdx++ )
    {
        if ( stricmp( gDevType[ devTypeIdx ]->devTypeStr, devType->devTypeStr ) == 0 )
        {
            LogError( "Device Type '%s' already registered\n", devType->devTypeStr );
            return;
        }
    }

    // Not a device type we know about - add it to the list

    gDevType[ gNumDevTypes ] = devType;
    gNumDevTypes++;
}

/***************************************************************************/
/**
*   Read device types and registers
*/

bool ReadRegisterFiles( const char *exeDir )
{
    bool    rc = true;

#if defined( __WIN32__ )
    HANDLE          dir;
    WIN32_FIND_DATA fd;
    char            pathName[ MAX_PATH ];

    _makepath( pathName, "", exeDir, "reg-*", "bld" );

    if (( dir = FindFirstFile( pathName, &fd )) != INVALID_HANDLE_VALUE )
    {
        do
        {
            DevTypeParser   dtp;

            LogVerbose( "Parsing device type file '%s' ...\n", fd.cFileName );
            if ( !dtp.ParseFile( fd.cFileName, AddDevType ))
            {
                rc = false;
                break;
            }

        } while ( FindNextFile( dir, &fd ));
    }

    FindClose( dir );
#else
    DIR *dir;
    struct dirent *de;

    if (( dir = opendir( exeDir )) == NULL )
    {
        LogError( "Unable to open '%s'\n", exeDir );
        return false;
    }

    while (( de = readdir( dir )) != NULL )
    {
        if ( fnmatch( "reg-*.bld", de->d_name, FNM_CASEFOLD ) == 0 )
        {
            DevTypeParser   dtp;

            LogVerbose( "Parsing device type file '%s' ...\n", de->d_name );
            if ( !dtp.ParseFile( de->d_name, AddDevType ))
            {
                rc = false;
                break;
            }
        }
    }

    closedir( dir );
#endif
    return rc;
}

/***************************************************************************/
/**
*   Print out the program usage.
*/

#if USE_COMMAND_LINE
void Usage()
{
    fprintf( stderr, "Usage: bioloid <option(s)>\n" );
    fprintf( stderr, "  Send commands to bioloid devices\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -b, --baud=baud   Set the baudrate used\n" );
    fprintf( stderr, "  -p, --port=name   Set the serial port to use\n" );
    fprintf( stderr, "  -n, --net=name[:port] Set the network host to use\n" );
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
    const char         *baudStr = DEFAULT_BAUD;
    const char         *portStr = DEFAULT_PORT;
#endif
#if USE_NETBUS
    const char         *hostStr = NULL;
#endif
    char                line[ 80 ];
    BioloidCommandLine  cmdLine;

#if USE_COMMAND_LINE

    // Figure out which directory our executable came from

    char *exeDir = dirname( argv[0] );

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
                gUseSerial = true;
                break;
            }

#if USE_NETBUS
            case OPT_NET:
            {
                hostStr = optarg;
                gUseNet = true;
                break;
            }
#endif

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
#endif

#if USE_NETBUS
    if ( gUseSerial && gUseNet )
    {
        LogError( "Only specify network or serial, not both\n" );
        exit( 1 );
    }
#endif

    // Read in all of the reg-*.bld files

    ReadRegisterFiles( exeDir );

#if USE_NETBUS
    if ( gUseNet )
    {
        if ( !gNetBus.Open( hostStr ))
        {
            exit( 1 );
        }
        gNetBus.SetShowPackets( gDebug != 0 );
        cmdLine.SetBus( &gNetBus );
    }
    else
#endif
    {
        // Default to serial if no network specified

        if ( !gSerialPort.Open( portStr, baudStr ))
        {
            exit( 1 );
        }
        gSerialBus.SetSerialPort( &gSerialPort );
        gSerialBus.SetShowPackets( gDebug != 0 );
        cmdLine.SetBus( &gSerialBus );
    }

    cmdLine.RegisterDeviceTypes( gNumDevTypes, gDevType );

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

