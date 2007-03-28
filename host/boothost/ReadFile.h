/****************************************************************************
*
*     Copyright (c) 2005 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
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

