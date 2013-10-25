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
*   @file   ReadFile.cpp
*
*   @brief  This file contains the ReadFile implementation
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#include "ReadFile.h"
#include "FileParser.h"
#include "ParserCallback.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

class FileParserCallback : public ParserCallback
{
public:

    //-----------------------------------------------------------------------
    //  Called by ParsedData whenever a new segment is detected. In this
    //  context, a segment is a contiguous block of data.

    virtual  bool  StartSegment( unsigned addr );

    //-----------------------------------------------------------------------
    //  Called when a data line is parsed from the file. 

    virtual  bool  Data( const RecordData *recData );

    FileInfo    *m_fileInfo;
};

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup BootHost
 * @{
 */

//***************************************************************************
/**
*   Reads a file into memory.
*/

FileInfo *ReadFile( const char *fileName )
{
    FileParserCallback  callback;
    FileParser          parser;

    callback.m_fileInfo = new FileInfo;
    callback.m_fileInfo->fileName = strdup( fileName );

    if ( !parser.ParseFile( fileName, &callback ))
    {
        FreeFile( callback.m_fileInfo );
        return NULL;
    }

    return callback.m_fileInfo;

} // ReadFile

//***************************************************************************
/**
*   Releases the memory associated with the file info.
*/

void FreeFile( FileInfo *fileInfo )
{
    if ( fileInfo != NULL )
    {
        FileSegment *segment = fileInfo->head;

        while ( segment != NULL )
        {
            FileSegment *nextSegment = segment->next;

            delete segment;

            segment = nextSegment;
        }

        free( fileInfo->fileName );
        delete fileInfo;
    }

} // FreeFile

//***************************************************************************
/**
*   Called when a new segment is detected.
*/

bool FileParserCallback::StartSegment( unsigned addr )
{
    FileSegment *segment = new FileSegment;

    memset( segment, 0, sizeof( *segment ));
    memset( &segment->data[ 0 ], 0xFF, sizeof( segment->data ));

    segment->address = addr;
    segment->lenThisSegment = 0;

    if ( m_fileInfo->tail == NULL )
    {
        m_fileInfo->head = segment;
    }
    else
    {
        m_fileInfo->tail->next = segment;
    }
    m_fileInfo->tail = segment;

    return true;
    
} // StartSegment

//***************************************************************************
/**
*   Called when a data line is parsed from the file. 
*/

bool FileParserCallback::Data( const ParserCallback::RecordData *recData )
{
    FileSegment *segment = m_fileInfo->tail;
    unsigned spaceRemaining = sizeof( segment->data ) - segment->lenThisSegment;
    unsigned offset = 0;

    // Copy in as much data as will fit

    unsigned    copyLen = recData->m_dataLen;
    if ( copyLen > spaceRemaining )
    {
        copyLen = spaceRemaining;
    }
    if ( copyLen > 0 )
    {
        memcpy( &segment->data[ segment->lenThisSegment ], &recData->m_data[ 0 ], copyLen );
        segment->lenThisSegment += copyLen;
        offset += copyLen;
    }

    if ( offset < recData->m_dataLen )
    {
        // We've filled this segment, allocate a new one

        StartSegment( recData->m_addr + offset );

        copyLen = recData->m_dataLen - offset;
        segment = m_fileInfo->tail;
        memcpy( &segment->data[ 0 ], &recData->m_data[ offset ], copyLen );
        segment->lenThisSegment = copyLen;
    }

    return true;

} // Data

/** @} */

