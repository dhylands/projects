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
*   @file   ParserCallback.h
*
*   @brief  This file implements the ParserCallback interface.
*
****************************************************************************/

#if !defined( PARSERCALLBACK_H )
#define PARSERCALLBACK_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

/**
 * @addtogroup FileParser
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The ParserCallback class is called by the FileParser class whenever
*   a significant event occurs.
*/

class ParserCallback
{
public:

	struct RecordData
	{
        unsigned       m_lineNum;        ///< Line number of data record in ASCII file.
        unsigned       m_addr;           ///< Address that was parsed.
        unsigned char  m_dataLen;        ///< Number of bytes of data.
        unsigned char  m_data[ 64 ];     ///< Data that was parsed.
	};

    //------------------------------------------------------------------------
    // Default constructor

    ParserCallback();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~ParserCallback();

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

    //-----------------------------------------------------------------------
    //  Called when an error is encountered parsing the file. This function
    //  currently sends an error message to stderr, but can be over-ridden
    //  by a derived class requiring different behaviour.

    virtual  void  ParseError( unsigned lineNum, const char *str );

private:


    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    ParserCallback( const ParserCallback & copy );
    ParserCallback &operator =( const ParserCallback &rhs );

    //------------------------------------------------------------------------

};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // PARSERCALLBACK_H

