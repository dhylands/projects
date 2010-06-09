/****************************************************************************
*
*   Copyright (c) 2010 Dave Hylands     <dhylands@gmail.com>
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
*   @file   razor-imu
*
*   @brief  Test harness for the Razor IMU parser.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include <libgen.h>

#include "Log.h"
#include "Str.h"
#include "SerialPort.h"

#include "Razor-IMU.h"

#if defined( WIN32 )
#   include <windows.h>
#else
#   include <fnmatch.h>
#   include <dirent.h>
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#define DEFAULT_BAUD    "57600"

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
    OPT_FILE        = 'f',
    OPT_PORT        = 'p',
    OPT_VERBOSE     = 'v',
    OPT_HELP        = 'h',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,

};

static struct option gOption[] =
{
    { "baud",       required_argument,  NULL,       OPT_BAUD },
    { "debug",      no_argument,        NULL,       OPT_DEBUG },
    { "file",       required_argument,  NULL,       OPT_FILE },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { "port",       required_argument,  NULL,       OPT_PORT },
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { NULL }
};

SerialPort      gSerialPort;

bool            gUseSerial = false;

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*   Print out the program usage.
*/

void Usage()
{
    fprintf( stderr, "Usage: razor-imu-test <option(s)>\n" );
    fprintf( stderr, "  Send commands to bioloid devices\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -b, --baud=baud   Set the baudrate used\n" );
    fprintf( stderr, "  -p, --port=name   Set the serial port to use\n" );
    fprintf( stderr, "  -d, --debug       Enable debug features\n" );
    fprintf( stderr, "  -h, --help        Display this message\n" );
    fprintf( stderr, "  -v, --verbose     Turn on verbose messages\n" );

} // Usage

//***************************************************************************
/**
*   Called when one line of data is parsed from the IMU sensor.
*/

static void EulerCallback( RAZOR_IMU_t *instance, int16_t roll, int16_t pitch, int16_t yaw )
{
    printf( "Roll: %6d Pitch: %6d Yaw: %6d\r", roll, pitch, yaw );
    fflush( stdout );
}

//***************************************************************************
/**
*   main
*/

int main( int argc, char **argv )
{
    char                shortOptsStr[ sizeof( gOption ) / sizeof( gOption[ 0 ] ) + 1 ];
    char               *shortOpts = shortOptsStr;
    struct option      *scanOpt;
    int                 opt;
    const char         *baudStr = DEFAULT_BAUD;
    const char         *portStr = DEFAULT_PORT;
    RAZOR_IMU_t         imu;
    const char *        fileName = NULL;

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

            case OPT_FILE:
            {
                fileName = optarg;
                break;
            }

            case OPT_PORT:
            {
                portStr = optarg;
                gUseSerial = true;
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

    RAZOR_IMU_Init( &imu );
    imu.m_debug = gDebug;

    imu.m_eulerCallback = EulerCallback;

    if ( fileName != NULL )
    {
        FILE   *fs;
        char    ch;

        if (( fs = fopen( fileName, "rb" )) == NULL )
        {
            fprintf( stderr, "Unable to open file: '%s'\n", fileName );
            exit( 1 );
        }

        while (( ch = fgetc( fs )) >= 0 )
        {
            RAZOR_IMU_ProcessChar( &imu, ch );
        }
    }
    else
    {
        if ( !gSerialPort.Open( portStr, baudStr ))
        {
            exit( 1 );
        }
    
        while ( 1 )
        {
            char    ch;
    
            gSerialPort.Read( &ch, 1 );
            RAZOR_IMU_ProcessChar( &imu, ch );
        }
    }
    printf( "\n" );

    exit( 0 );
    return 0;
}

