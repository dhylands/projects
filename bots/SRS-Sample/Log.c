/****************************************************************************
*
*   Copyright (c) 2005 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
/**
*
*   @file   Log.c
*
*   @brief  This file contains the implementation of the logging functions.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>

#include "Log.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#if defined( AVR )

#undef  Log
#undef  LogError
#undef  vLog
#undef  vStrXPrintf

#define Log         Log_P
#define LogError    LogError_P
#define vLog        vLog_P
#define vStrXPrintf vStrXPrintf_P

#define char        prog_char

#else

int gVerbose = 0;
int gDebug   = 0;

#endif

// ---- Private Variables ---------------------------------------------------

static  StrXPrintfFunc gLogPrintFunc = NULL;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Log
 * @{
 */

//***************************************************************************
/**
*   Sets the logging stream
*/

void LogInit( StrXPrintfFunc logPrintFunc )
{
    gLogPrintFunc = logPrintFunc;

} // LogInit

//***************************************************************************
/**
*   Logs a string using printf like semantics
*/

void Log
(
    const char *fmt,    ///< printf style format specifier
    ...                 ///< variable list of arguments
)
{
    va_list args;

    va_start( args, fmt );
    vLog( fmt, args );
    va_end( args );
}

//***************************************************************************
/**
*   Logs a string using printf like semantics
*/

void vLog
(
    const char *fmt,    ///< printf style format specified
    va_list     args    ///< variable list of arguments
)
{
    if ( gLogPrintFunc != NULL )
    {
        // For now we call printf directly. A better way would be to install
        // a callback which does the real work

        vStrXPrintf( gLogPrintFunc, NULL, fmt, args );
    }
}

/***************************************************************************/
/**
*   Logs an error
*/

void LogError
(
    const char *fmt,    ///< printf style format specifier
    ...                 ///< variable list of arguments
)
{
    va_list args;

    Log( "ERROR: " );

    va_start( args, fmt );
    vLog( fmt, args );
    va_end( args );

} // LogError

/** @} */

