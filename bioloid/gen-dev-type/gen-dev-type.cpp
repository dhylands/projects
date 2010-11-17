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
*   @file   gen-dev-type.cpp
*
*   @brief  Reads a bioloid device file and creates a class derived from
*           the BioloidDevice class.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#include "Log.h"
#include "Str.h"

#include "bioloid-reg.h"

#include "DevTypeParser.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

enum
{
    // Options assigned a single character code can use that charater code
    // as a short option.

    OPT_DEBUG       = 'd',
    OPT_VERBOSE     = 'v',
    OPT_HELP        = 'h',

    // Options from this point onwards don't have any short option equivalents

    OPT_FIRST_LONG_OPT   = 0x80,

};

static struct option gOption[] =
{
    { "debug",      no_argument,        NULL,       OPT_DEBUG },
    { "help",       no_argument,        NULL,       OPT_HELP },
    { "verbose",    no_argument,        NULL,       OPT_VERBOSE },
    { NULL }
};

static  const char *gFileName;

static  char    gSymbolStr[ 100 ];

/* ---- Private Function Prototypes --------------------------------------- */

static const char *CSymbol( const char *str );
static const char *CUpperSymbol( const char *str );

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup Bioloid
 * @{
 */

/***************************************************************************/
/**
*   Given a symbol, make sure that it can be used a C symbol.
*/

const char *CSymbol( const char *str )
{
    char       *dst_s = gSymbolStr;
    const char *src_s = str;
    int         makeUpper = 1;

    while ( *src_s != '\0' )
    {
        if ( isalnum( *src_s ))
        {
            if ( makeUpper )
            {
                *dst_s = toupper( *src_s );
                makeUpper = 0;
            }
            else
            {
                *dst_s = *src_s;
            }
            dst_s++;
        }
        else
        {
            makeUpper = 1;
        }

        src_s++;
    }
    *dst_s = '\0';

    return gSymbolStr;
}

/***************************************************************************/
/**
*   Given a symbol, make sure that it can be used as a C symbol and is
*   all uppercase.
*/

const char *CLowerSymbol( const char *str )
{
    char *dst_s = gSymbolStr;
    const char *src_s = str;

    while ( *src_s != '\0' )
    {
        if ( isalnum( *src_s ))
        {
            *dst_s = tolower( *src_s );
        }
        else
        {
            *dst_s = '_';
        }

        dst_s++;
        src_s++;
    }
    *dst_s = '\0';

    return gSymbolStr;
}

/***************************************************************************/
/**
*   Given a symbol, make sure that it can be used as a C symbol and is
*   all uppercase.
*/

const char *CUpperSymbol( const char *str )
{
    char *dst_s = gSymbolStr;
    const char *src_s = str;

    while ( *src_s != '\0' )
    {
        if ( isalnum( *src_s ))
        {
            *dst_s = toupper( *src_s );
        }
        else
        {
            *dst_s = '_';
        }

        dst_s++;
        src_s++;
    }
    *dst_s = '\0';

    return gSymbolStr;
}

/***************************************************************************/
/**
*   Given a parsed device type, generate the source file
*/

void GenerateDeviceType( BLD_DevType_t *devType )
{
    char        genFileName[ FILENAME_MAX ];
    char        guardStr[ 60 ];
    FILE       *genFs;
    const char *baseName;
    char       *s;

    if (( baseName = strrchr( gFileName, '/' )) == NULL )
    {
        baseName = genFileName;
    }
    else
    {
        baseName++;
    }
    LogVerbose( "baseName = '%s'\n", baseName );

    StrMaxCpy( genFileName, baseName, sizeof( genFileName ));
    if (( s = strrchr( genFileName, '.' )) != NULL )
    {
        *s = '\0';
    }
    StrMaxCat( genFileName, ".h", sizeof( genFileName ));

    if (( genFs = fopen( genFileName, "w" )) == NULL )
    {
        fprintf( stderr, "Unable to open '%s' for writing\n", genFileName );
        return;
    }

    Log( "Generating %s from %s ...\n", genFileName, gFileName );

    // Figure out the guard symbol

    StrMaxCpy( guardStr, CUpperSymbol( genFileName ), sizeof( guardStr ));

    fprintf( genFs, "/****************************************************************************\n" );
    fprintf( genFs, "*\n" );
    fprintf( genFs, "* This file was automatically generated from %s\n", gFileName );
    fprintf( genFs, "*\n" );
    fprintf( genFs, "****************************************************************************/\n" );
    fprintf( genFs, "\n" );
    fprintf( genFs, "#if !defined( %s )\n", guardStr );
    fprintf( genFs, "#define %s\n", guardStr );
    fprintf( genFs, "\n" );
    fprintf( genFs, "#include \"BioloidDevice.h\"\n" );
    fprintf( genFs, "\n" );
    fprintf( genFs, "class Bioloid%s : public BioloidDevice\n", CSymbol( devType->devTypeStr ));
    fprintf( genFs, "{\n" );
    fprintf( genFs, "public:\n" );

    // Generate the constants for the register offsets
    fprintf( genFs, "    enum\n" );
    fprintf( genFs, "    {\n" );

    for ( unsigned regIdx = 0; regIdx < devType->numRegs; regIdx++ )
    {
        BLD_Reg_t   *reg = &devType->reg[ regIdx ];

        fprintf( genFs, "        REG_%-24s = 0x%02x, // %2d\n", CUpperSymbol( reg->name ), reg->address, reg->address );
    }

    fprintf( genFs, "    };\n\n" );

    // Generate the inline get/set methods

    for ( unsigned regIdx = 0; regIdx < devType->numRegs; regIdx++ )
    {
        BLD_Reg_t   *reg = &devType->reg[ regIdx ];

        const char *typeStr;

        if ( reg->flags & BLD_REG_FLAG_16BIT )
        {
            typeStr = "uint16_t";
        }
        else
        {
            typeStr = "uint8_t";
        }

        fprintf( genFs, "    %s get_%s()\n", typeStr, CLowerSymbol( reg->name ));
        fprintf( genFs, "    {\n" );
        fprintf( genFs, "        %s val;\n", typeStr );
        fprintf( genFs, "        if ( Read( REG_%s, &val ) == Bioloid::ERROR_NONE )\n", CUpperSymbol( reg->name ));
        fprintf( genFs, "            return val;\n" );
        fprintf( genFs, "        return 0;\n" );
        fprintf( genFs, "    }\n\n" );

        if (( reg->flags & BLD_REG_FLAG_WR ) != 0 )
        {
            fprintf( genFs, "    void set_%s( %s val )\n", CLowerSymbol( reg->name ), typeStr );
            fprintf( genFs, "    {\n" );
            fprintf( genFs, "        Write( REG_%s, &val, sizeof( val ))\n", CUpperSymbol( reg->name ));
            fprintf( genFs, "    }\n\n" );
        }
    }

    fprintf( genFs, "};\n" );
    fprintf( genFs, "\n" );
    fprintf( genFs, "#endif // %s\n", guardStr );

    fclose( genFs );
}

/***************************************************************************/
/**
*   Print out the program usage.
*/

static void ProcessDeviceType( const char *fileName )
{
    DevTypeParser    dtp;

    gFileName = fileName;

    dtp.ParseFile( fileName, GenerateDeviceType );
}

/***************************************************************************/
/**
*   Print out the program usage.
*/

void Usage()
{
    fprintf( stderr, "Usage: gen-dev-type <option(s)>\n" );
    fprintf( stderr, "  Send commands to bioloid devices\n" );
    fprintf( stderr, "\n" );
    fprintf( stderr, "  -d, --debug       Enable debug features\n" );
    fprintf( stderr, "  -h, --help        Display this message\n" );
    fprintf( stderr, "  -v, --verbose     Turn on verbose messages\n" );

} // Usage

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

            case OPT_DEBUG:
            {
                gDebug = 1;
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

    for ( int arg = 0; arg < argc; arg++ )
    {
        const char *fileName = argv[ arg ];

        Log( "Processing %s ...\n", fileName );

        ProcessDeviceType( fileName );
    }
}

/** @} */

