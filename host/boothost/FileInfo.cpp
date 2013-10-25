/****************************************************************************
*
*   Copyright (c) 2008 Dave Hylands     <dhylands@gmail.com>
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
#include <stdio.h>
#include "FileParser.h"

class InfoCallback : public ParserCallback
{
public:

    //-----------------------------------------------------------------------
    //  Called by ParsedData whenever a new segment is detected. In this
    //  context, a segment is a contiguous block of data.

    virtual  bool  StartSegment( unsigned addr );

    //-----------------------------------------------------------------------
    //  Called when a data line is parsed from the file. 

    virtual  bool  Data( const RecordData *recData );

    //-----------------------------------------------------------------------
    //  Called when a segment has been finished. 

    virtual  bool  FinishSegment( unsigned addr, unsigned len );

};

bool InfoCallback::StartSegment( unsigned addr )
{
    printf( "StartSegment: 0x%08x\n", addr );

	return true;
}

bool InfoCallback::Data( const RecordData *recData )
{
    printf( "Data: 0x%08x len: %d\n", recData->m_addr, recData->m_dataLen );

	return true;
}

bool InfoCallback::FinishSegment( unsigned addr, unsigned len )
{
    printf( "FinisSegment: 0x%08x, len: %d\n", addr, len );

	return true;
}

int main( int argc, char **argv )
{
    InfoCallback    callback;

    for ( int arg = 1; arg < argc; arg++ ) 
    {
        FileParser  p;

        p.ParseFile( argv[ arg ], &callback );
    }
    return 0;
}
