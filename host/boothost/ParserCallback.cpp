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
*   @file   ParserCallback.cpp
*
*   @brief  This file contains the "base" implementation of the callback
*           class.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>

#include "ParserCallback.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup FileParser
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

ParserCallback::ParserCallback()
{
    // Nothing to do
}

//***************************************************************************
/**
*   Destructor
*/

ParserCallback::~ParserCallback()
{
    // Nothing to do.
}

//***************************************************************************
/**
*   Called by ParsedData whenever a new segment is detected. In this
*   context, a segment is a contiguous block of data.
* 
*   This function is intended to be over-ridden by a derived class.
*
*   @param   addr  (in)  Address of the new segment.
*   
*   @return  true if parsing should continue, false if it should stop.
*
*   virtual
*/

bool ParserCallback::StartSegment( unsigned addr )
{
    (void)addr;

    return true;

} // StartSegment

//***************************************************************************
/**
*   Called when a data line is parsed from the file. This is intended to
*   be over-ridden by a derived class.
*
*   @param   recData (in)  Contains the information that was parsed.
*
*   @return  true if parsing should continue, false if it should stop.
*
*   virtual
*/

bool ParserCallback::Data( const ParserCallback::RecordData *recData )
{
    (void)recData;

    return true;

} // Data

//***************************************************************************
/**
*   Called when a segment has been finished. This is intended to be
*   over-ridden by a derived class.
*
*   @param   addr  (in) Address of the segment just finished.
*   @param   len   (in) Length of the segment just finished.
*
*   @return  true if parsing should continue, false if it should stop.
*
*   virtual
*/

bool ParserCallback::FinishSegment( unsigned addr, unsigned len )
{
    (void)addr;
    (void)len;

    return true;

} // FinishSegment

//***************************************************************************
/**
*   Called when an error is encountered parsing the file. This function
*   currently sends an error message to stderr, but can be over-ridden
*   by a derived class requiring different behaviour.
*
*   @param  lineNum (in)    Line number containing the error.
*   @param  str     (in)    String containing the error.
*
*   @return  true if parsing should continue, false if it should stop.
*
*   virtual
*/

void ParserCallback::ParseError( unsigned lineNum, const char *str )
{
    fprintf( stderr, "Error: " );

    if ( lineNum > 0 )
    {
        fprintf( stderr, "line %d: ", lineNum );
    }

    fprintf( stderr, "%s\n", str );

} // ParseError

/** @} */

