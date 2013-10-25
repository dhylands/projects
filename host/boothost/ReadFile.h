/****************************************************************************
*
*   Copyright (c) 2005 - 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   ReadFile.h
*
*   @brief  This file contains the ReadFile interface.
*
****************************************************************************/

#if !defined( READFILE_H )
#define READFILE_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>

#if !defined( PARSERCALLBACK_H )
#   include "ParserCallback.h"
#endif

/**
 * @addtogroup BootHost
 * @{
 */

// ---- Constants and Types -------------------------------------------------

struct FileSegment
{
    unsigned        address;
    unsigned        lenThisSegment;
    unsigned char   data[ 1024 ];

    FileSegment    *next;
};

struct FileInfo
{
    char           *fileName;

    FileSegment    *head;
    FileSegment    *tail;
};

// ---- Functions -----------------------------------------------------------

FileInfo *ReadFile( const char *fileName );
void FreeFile( FileInfo *fileInfo );

/** @} */

#endif // READFILE_H

