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
*   @file   FileParser.h
*
*   @brief  This file contains the FileParser interface.
*
****************************************************************************/
/**
*   @defgroup   FileParser   File Parser
*
*   @brief      Parses a file for downloading to a target.
*
****************************************************************************/

#if !defined( FILEPARSER_H )
#define FILEPARSER_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

#if !defined( PARSERCALLBACK_H )
#   include "ParserCallback.h"
#endif

/**
 * @addtogroup FileParser
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The FileParser class can parser S-Record or Intel Hex files.
*/

class FileParser
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    FileParser();

    //------------------------------------------------------------------------
    // Destructor

    ~FileParser();

    //-----------------------------------------------------------------------
    //  Parses the S-Record or the Intel-Hex file named by @a fileName.

    bool  ParseFile( const char *fileName, ParserCallback *callback );

    //-----------------------------------------------------------------------
    //  Parses the S-Record file indicated by @a fs.

    bool  ParseFile( FILE *fs, ParserCallback *callback );

protected:

    //-----------------------------------------------------------------------
    //  Parses a single line from an S-Record file.

    bool  ParseLine( unsigned lineNum, const char *line );

    //-----------------------------------------------------------------------
    //  Parses a single line from an Intel Hex file.

    bool  ParseIHexLine( unsigned lineNum, const char *line );

    //-----------------------------------------------------------------------
    //  Parses a single line from an S-Record file.

    bool  ParseSRecordLine( unsigned lineNum, const char *line );

private:

    //------------------------------------------------------------------------
    //  The copy constructor and assignment operator are not need for this
    //  class so we declare them private and don't provide an implementation.

    FileParser( const FileParser & copy );
    FileParser &operator =( const FileParser &rhs );

    //-----------------------------------------------------------------------
    //  Called when an error is encountered parsing the file.

    void  Error( unsigned lineNum, const char *fmt, ... );

    //-----------------------------------------------------------------------
    //  Called when an error is encountered parsing the file.

    void  vError( unsigned lineNum, const char *fmt, va_list args );

    //-----------------------------------------------------------------------
    //  Forces an open segment (if any) to be finished.

    bool Flush();

    //-----------------------------------------------------------------------
    //  Parses a single byte from a string containing ASCII Hex characters.

    bool GetByte
    (
       const char   **s, 
       unsigned char *b, 
       unsigned       lineNum, 
       const char    *label 
    );

    //-----------------------------------------------------------------------
    //  Parses a single nibble from a string containing ASCII Hex characters.

    bool GetNibble
    (
       const char   **s, 
       unsigned char *b ,
       unsigned       lineNum, 
       const char    *label 
    );

    //-----------------------------------------------------------------------
    //  Parses two bytes from a string containing ASCII Hex characters.

    bool GetWord
    (
       const char    **s, 
       unsigned short *b, 
       unsigned        lineNum, 
       const char     *label 
    );

    //------------------------------------------------------------------------
    //  Called when an S-Record data line is parsed. This routine will, 
    //  in turn, call StartSegment(), Data, and FinishSegment() as 
    //  appropriate.

    bool  ParsedData( const ParserCallback::RecordData *recData );

    //------------------------------------------------------------------------

    bool        m_isSRecord;    ///< Have we discovered an S-Record yet?
    bool        m_isIHexRecord; ///< Have we discovered an Intel Hex Record yet?

    bool        m_inSeg;        ///< Are we currently inside a segment?
    unsigned    m_segAddr;      ///< Address of segment currently being parsed.
    unsigned    m_segLen;       ///< Length of segment currently being parsed.
    unsigned    m_addrBase;     ///< Added to address

    ParserCallback  *m_callback;    ///< Callback to report parsed data to.


};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // FILEPARSER_H

