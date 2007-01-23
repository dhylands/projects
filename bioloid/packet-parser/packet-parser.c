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
*   @file   packet-parser.c 
*
*   @brief  Implements a test harness for the biolois packet parser
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

#include "bioloid-pkt.h"
#include "Log.h"
#include "svn-version.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

enum
{
    // Options assigned a single character code can use that charater code
    // as a short option.

    OPT_HELP            = 'h',
    OPT_VERBOSE         = 'v',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,
    OPT_VERSION,
};

#define TRUE    1
#define FALSE   0

/* ---- Private Variables ------------------------------------------------- */

struct option gOption[] =
{
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { "version",    no_argument,        NULL,       OPT_VERSION },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { NULL }
};

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Usage
*/

static void Usage( void )
{
    fprintf( stderr, "Usage: packet-parser [options] fileName\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "Parses bytes from 'fileName' and prints the packet contents\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "The following options may be used to modify the behaviour\n" );
    fprintf( stderr, "--version             Print the version information for this program\n" );
    fprintf( stderr, "--verbose             Print additional information\n" );
    fprintf( stderr, "--help                Prints this information\n" );

} // Usage

//***************************************************************************
/**
*   Called when a packet is parsed.
*/

static void PacketReceived( BLD_Instance_t *inst, BLD_Packet_t *pkt, BLD_Error_t err )
{
    int     i;

    if ( err == BLD_ERROR_NONE )
    {
        Log( "Rcvd ID: 0x%02x Len: 0x%02x Cmd: 0x%02x Param:", 
             pkt->m_id, pkt->m_length, pkt->m_cmd );

        if ( pkt->m_length > 2 )
        {
            for ( i = 0; i < pkt->m_length - 2; i++ ) 
            {
                Log( " 0x%02x", pkt->m_param[ i ] );
            }
        }
        else
        {
            Log( " None" );
        }
        Log( "\n" );
    }
    else
    {
        Log( "CRC Error\n" );
    }

} // PakcetReceived

//***************************************************************************
/**
*   Main program
*/

int main( int argc, char **argv )
{
    char                shortOptsStr[ sizeof( gOption ) / sizeof( gOption[ 0 ] ) + 1 ];
    char               *shortOpts = shortOptsStr;
    struct option       *scanOpt;
    int                 opt;
    char                line[ 200 ];
    char               *fileName;
    FILE               *fs;
    int                 lineNum;
    BLD_Instance_t      inst;

    LogInit( stdout );

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

            case OPT_VERBOSE:
            {
                gVerbose = TRUE;
                break;
            }

            case OPT_VERSION:
            {   
                Log( "packet-parser: SVN Revision: %d\n", SVN_REVISION );
                exit( 0 );
            }

            case '?':
            case OPT_HELP:
            default:
            {
                LogError( "opt:%d\n", opt );
                Usage();
                exit( 1 );
            }
        }
    }
    argc -= optind;
    argv += optind;

    if ( argc != 1 )
    {
        LogError( "Usage: packet-parser file %d\n", argc );
        exit( 1 );
    }

    fileName = argv[ 0 ];

    if (( fs = fopen( fileName, "rb" )) == NULL )
    {
        LogError( "Unable to open '%s' for reading\n", fileName );
        exit( 1 );
    }

    lineNum = 0;
    BLD_Init( &inst );

    inst.m_pktRcvd = PacketReceived;

    while ( fgets( line, sizeof( line ), fs ) != NULL )
    {
        char *s = line;
        char *endPtr;

        lineNum++;

        LogVerbose( "Read line: %s", line );

        while ( *s != '\0' )
        {
            long val;

            // Skip leading spaces

            while ( isspace( *s ))
            {
                s++;
            }

            if (( *s == '#' ) || ( *s == '\0' ))
            {
                // Comment line. Ignore the rest.

                break;
            }

            // Parse a byte

            val = strtol( s, &endPtr, 16 );

            if (( *endPtr != '\0' ) && !isspace( *endPtr ))
            {
                LogError( "Line %d: Invalid number detected: '%s'\n", lineNum, s );
                break;
            }

            if (( val < 0 ) || ( val > 255 ))
            {
                LogError( "Line %d: Expecting value to be in the range 0-255, found %ld\n", val );
                break;
            }

//            LogVerbose( "Parsed: 0x%02x\n", val );

            BLD_ProcessChar( &inst, val );

            s = endPtr;
        }
    }

    fclose( fs );

} // main

