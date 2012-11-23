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
*   @file   Log.h
*
*   @brief  Contains some logging macros.
*
****************************************************************************/
/**
*   @defgroup   Log   Logging
*
*   @brief      Provides a common interface for logging.
*
****************************************************************************/

#if !defined( LOG_H )
#define LOG_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>
#include "Str.h"

/**
 * @addtogroup Log
 * @{
 */

#if defined( __cplusplus )
extern "C"
{
#endif

void LogInit( StrXPrintfFunc logPrintFunc );

#if defined( AVR )

#include <avr/pgmspace.h>

void Log( const char *fmt, ... );

void Log_P( const prog_char *fmt, ... );
void LogError_P( const prog_char *fmt, ... );
void vLog_P( const prog_char *fmt, va_list args );

#define Log( fmt, args... )         Log_P( PSTR( fmt ), ## args )
#define LogError( fmt, args... )    LogError_P( PSTR( fmt ), ## args )
#define vLog( fmt, va_list, args )  vLog_P( PSTR( fmt ), args )

#else

void Log( const char *fmt, ... );
void LogError( const char *fmt, ... );
void vLog( const char *fmt, va_list args );

#endif

extern  int     gVerbose;
extern  int     gDebug;

#define LogDebug( fmt, args... )    do { if ( gDebug )   { Log( fmt, ## args ); }} while (0)
#define LogVerbose( fmt, args... )  do { if ( gVerbose ) { Log( fmt, ## args ); }} while (0)

#if defined( __cplusplus )
}
#endif

/** @} */

#endif // LOG_H

