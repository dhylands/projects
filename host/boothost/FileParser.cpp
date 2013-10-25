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
*   @file   FileParser.cpp
*
*   @brief  This file contains the implementation of the file parser.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <ctype.h>
                    
#include "FileParser.h"

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

FileParser::FileParser()
    : m_isSRecord( false ),
      m_isIHexRecord( false ),
      m_inSeg( false ),
      m_callback( NULL )
{
}

//***************************************************************************
/**
*   Destructor
*/

FileParser::~FileParser()
{
}

//***************************************************************************
/**
*  Called when an error is encountered parsing the file.
*/

void FileParser::Error
(
    unsigned lineNum,   ///< (in) Line number that the error occurred on.
    const char *fmt,    ///< (in) Format string as per vprintf.
    ...                 ///< (in) Varible arguments as per vprintf.
)
{
   va_list  args;

   va_start( args, fmt );
   vError( lineNum, fmt, args );
   va_end( args );
}

//***************************************************************************
/**
*  Called when an error is encountered parsing the file.
*/

void FileParser::vError
(
    unsigned lineNum,   ///< (in) Line number that the error occurred on.
    const char *fmt,    ///< (in) Format string as per vprintf.
    va_list args        ///< (in) Varible arguments as per vprintf.
)
{
   char  errStr[ 256 ];

   vsnprintf( errStr, sizeof( errStr ), fmt, args );

   m_callback->ParseError( lineNum, errStr );
}

//***************************************************************************
/**
*  Forces an open segment (if any) to be finished.
*/

bool FileParser::Flush()
{
   if ( m_inSeg )
   {
      if ( !m_callback->FinishSegment( m_segAddr,  m_segLen ))
      {
         return false;
      }
      m_inSeg = false;
   }
   return true;
}

//***************************************************************************
/**
*  Parses a single byte from a string containing ASCII Hex characters.
*
*  @return  true, if a byte was parsed successfully, false otherwise.
*/

bool FileParser::GetByte
(
    const char   **s,       ///< (mod) Pointer to string. Will be advanced.
    unsigned char *b ,      ///< (out) nibble that was parsed.
    unsigned       lineNum, ///< (in)  Line number, used for reporting errors).
    const char    *label    ///< (in)  Error string (used for reporting errors).
)
{
    unsigned char  b1, b2;

    if ( GetNibble( s, &b1, lineNum, label ) 
    &&   GetNibble( s, &b2, lineNum, label ))
    {
       *b = b1 << 4 | b2;
       return true;
    }

    return false;

} // GetByte

//***************************************************************************
/**
*  Parses two bytes from a string containing ASCII Hex characters.
*
*  @return  true, if a byte was parsed successfully, false otherwise.
*/

bool FileParser::GetWord
(
    const char    **s,       ///< (mod) Pointer to string. Will be advanced.
    unsigned short *b ,      ///< (out) nibble that was parsed.
    unsigned        lineNum, ///< (in)  Line number, used for reporting errors).
    const char     *label    ///< (in)  Error string (used for reporting errors).
)
{
    unsigned char  b1, b2;

    if ( GetByte( s, &b1, lineNum, label ) 
    &&   GetByte( s, &b2, lineNum, label ))
    {
       *b = (unsigned short)b1 << 8 | b2;
       return true;
    }

    return false;

} // GetWord

//***************************************************************************
/**
*  Parses a single nibble from a string containing ASCII Hex characters.
*
*  @return  true, if a nibble was parsed successfully, false otherwise.
*/

bool FileParser::GetNibble
(
    const char   **s,       ///< (mod) Pointer to string. Will be advanced.
    unsigned char *b ,      ///< (out) nibble that was parsed.
    unsigned       lineNum, ///< (in)  Line number, used for reporting errors).
    const char    *label    ///< (in)  Error string (used for reporting errors).
)
{
    char ch = **s;

    *s = *s + 1;

    if (( ch >= '0' ) && ( ch <= '9' ))
    {
       *b = ch - '0';
       return true;
    }

    if (( ch >= 'A' ) && ( ch <= 'F' ))
    {
       *b = ch - 'A' + 10;
       return true;
    }

    if (( ch >= 'a' ) && ( ch <= 'f' ))
    {
       *b = ch - 'a' + 10;
       return true;
    }

    Error( lineNum, "parsing %s, expecting hex digit, found '%c'", label, ch );
    return false;

} // GetNibble

//***************************************************************************
/**
*   Called when an S-Record data line is parsed. This routine will, 
*   in turn, call StartSegment(), Data, and FinishSegment() as 
*   appropriate.
*
*   @param  recData (in)  Contains the information that was parsed.
*
*   @return true if parsing should continue, false if it should stop.
*/

bool FileParser::ParsedData( const ParserCallback::RecordData *recData )
{
   if (( m_inSeg ) && ( recData->m_addr != ( m_segAddr + m_segLen )))
   {
      Flush();
   }

   if ( !m_inSeg )
   {
      m_inSeg = true;
      m_segAddr = recData->m_addr;
      m_segLen  = 0;

      if ( !m_callback->StartSegment( m_segAddr ))
      {
         return false;
      }
   }

   if ( !m_callback->Data( recData ))
   {
      return false;
   }

   m_segLen += recData->m_dataLen;

   return true;
}

//***************************************************************************
/** 
*   Parses the S-Record or the Intel-Hex file named by @a fileName.
*
*   @param  fileName (in) Name of the file to parse.
*   @param  callback (in) Callback class to pass parsed information to.  
*
*   @return  true if the file was parsed successfully, false otherwise.
*/

bool FileParser::ParseFile( const char *fileName, ParserCallback *callback )
{
    FILE  *fs;

    if (( fs = fopen( fileName, "rt" )) == NULL )
    {
       return false;
    }

    bool rc = ParseFile( fs, callback );

    fclose( fs );

    return rc;

} // ParseFile

//***************************************************************************
/**
*   Parses the S-Record file indicated by @a fs.
*
*   @param  fs       (mod) File to parse (assumes that it's positioned correctly).
*   @param  callback (in)  Callback class to pass parsed information to.  
*
*   @return  true if the file was parsed successfully, false otherwise.
*/

bool FileParser::ParseFile( FILE *fs, ParserCallback *callback )
{
    unsigned lineNum = 0;
    char     line[ 100 ];

    m_callback = callback;
    m_addrBase = 0;

    while ( fgets( line, sizeof( line ), fs ) != NULL )
    {
       lineNum++;

       if ( !ParseLine( lineNum, line ))
       {
          return false;
       }
    }
    Flush();

    return true;

} // ParseFile

//***************************************************************************
/**
*   Parses a single line from an S-Record file.
*
*   @param   line  (in)  Line from S-Record file to parse.
*
*   @return  true if the line was parsed successfully, false otherwise.
*/

bool FileParser::ParseLine( unsigned lineNum, const char *line )
{
    if ( !m_isSRecord && !m_isIHexRecord )
    {
        if ( line[ 0 ] == 'S' )
        {
            m_isSRecord = true;
        }
        else
        if ( line[ 0 ] == ':' )
        {
            m_isIHexRecord = true;
        }
    }
    if ( m_isSRecord )
    {
        return ParseSRecordLine( lineNum, line );
    }
    if ( m_isIHexRecord )
    {
        return ParseIHexLine( lineNum, line );
    }

    // Not a line we recognize. Treat it as a comment and ignore it.

    return true;

} // ParseLine


//***************************************************************************
/**
*   Parses a single line from an Intel Hex file.
*
*   The Intel Hex format looks like this:
*
*   : BC AAAA TT HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH CC <CR>
*
*   Note: Number of H's varies with data ..... this is example only.
*   Note: Spaces added for clarity
*
*   :     Start of record character
*   BC    Byte count
*   AAAA  Address to load data.  (tells receiving device where to load)
*   TT    Record type. (00=Data record, 01=End of file. No data in EOF record)
*   HH    Each H is one ASCII hex digit.  (2 hex digits=1 byte)
*   CC    Checksum of all bytes in record ( BC+AAAA+TT+HH......HH+CC=0 )
*/

bool FileParser::ParseIHexLine( unsigned lineNum, const char *line )
{
    ParserCallback::RecordData  recData;

    memset( &recData, 0, sizeof( recData ));

    if ( line[ 0 ] != ':' )
    {
        // In Intel Hex format, lines which don't start with ':' are
        // supposed to be ignored.

        return true;
    }

    const char *s = &line[ 1 ];
    unsigned char  dataLen;

    if ( !GetByte( &s, &dataLen, lineNum, "count" ))
    {
        return false;
    }
    if ( dataLen > sizeof( recData.m_data ))
    {
        Error( lineNum, "line length is longer than our internal buffer. LineLen = %d, internal buffer = %d\n",
               dataLen, sizeof( recData.m_data ));
        return false;
    }

    unsigned short  addr;
    if ( !GetWord( &s, &addr, lineNum, "addr" ))
    {
        return false;
    }

    unsigned char recType;
    if ( !GetByte( &s, &recType, lineNum, "recType" ))
    {
        return false;
    }

    unsigned char checksumCalc = dataLen + (( addr & 0xFF00 ) >> 8 ) + ( addr & 0x00FF ) + recType;

    for ( int i = 0; i < dataLen; i++ ) 
    {
        if ( !GetByte( &s, &recData.m_data[ i ], lineNum, "data" ))
        {
            return false;
        }
        checksumCalc += recData.m_data[ i ];
    }

    unsigned char checksumFound;

    if ( !GetByte( &s, &checksumFound, lineNum, "checksum" ))
    {
        return false;
    }

    if ((unsigned char)( checksumCalc + checksumFound ) != 0 )
    {
        Error( lineNum, "found checksum 0x%02x, expecting 0x%02x", 
               checksumFound, 0 - checksumCalc );
        return false;
    }

    switch ( recType )
    {
        case 0: // Data
        {
            recData.m_lineNum = lineNum;
            recData.m_addr    = m_addrBase + addr;
            recData.m_dataLen = dataLen;

            ParsedData( &recData );
            break;
        }

        case 1: // EOF
        {
            Flush();
            break;
        }

        case 2: // Segment
        {
            m_addrBase = ((unsigned)recData.m_data[ 0 ] << 12 ) | ((unsigned)recData.m_data[ 1 ] << 4 );
            break;
        }

        default:
        {
            Error( lineNum, "Unrecognized record type: %d", recType );
            return false;
        }
    }

	return true;

} // ParseIHexLine

//***************************************************************************
/**
*   Parses a single line from an S-Record file.
*/

bool FileParser::ParseSRecordLine( unsigned lineNum, const char *line )
{
    ParserCallback::RecordData  recData;
    unsigned char               data[ 50 ];
    
    memset( &recData, 0, sizeof( recData ));
    memset( data, 0, sizeof( data ));
    
    if ( line[ 0 ] != 'S' )
    {
        Error( lineNum, "doesn't start with an 'S'" );
        return false;
    }
    
    if ( !isdigit( line[ 1 ] ))
    {
        Error( lineNum, "expecting digit (0-9), found: '%c'", line[ 1 ]);
        return false;
    }
    
    const char *s = &line[ 2 ];
    unsigned char  lineLen;
    
    if ( !GetByte( &s, &lineLen, lineNum, "count" ))
    {
        return false;
    }
    
    unsigned char checksumCalc = lineLen;
    
    for ( int i = 0; i < ( lineLen - 1 ); i++ ) 
    {
        if ( !GetByte( &s, &data[ i ], lineNum, "data" ))
        {
            return false;
        }
        checksumCalc += data[ i ];
    }
    checksumCalc = ~checksumCalc;
    
    unsigned char checksumFound;
    
    if ( !GetByte( &s, &checksumFound, lineNum, "checksum" ))
    {
        return false;
    }
    
    if ( checksumFound != checksumCalc )
    {
        Error( lineNum, "found checksum 0x%02x, expecting 0x%02x", 
               checksumFound, checksumCalc );
        return false;
    }
    
    switch ( line[ 1 ] )
    {
        case '0':
        {
            // Header Record - We ignore this for now

            Flush();
            break;
        }
        
        case '1':   // 2 byte address
        case '2':   // 3 byte address
        case '3':   // 4 byte address
        {

            memset( &recData, 0, sizeof( recData ));
            
            recData.m_lineNum   = lineNum;
            int addrLen			= line[ 1 ] - '1' + 2;
            
            unsigned char *x = data;
            
            for ( int addrIdx = 0; addrIdx < addrLen; addrIdx++ ) 
            {
                recData.m_addr <<= 8;
                recData.m_addr += *x++;
            }
            recData.m_dataLen = lineLen - addrLen - 1;
            memcpy( recData.m_data, x, recData.m_dataLen );
            
            ParsedData( &recData );
            break;
        }
        
        case '5':   // Count of S1, S2, and S3 records
        {
            Flush();
            break;
        }
        
        case '7':   // 2 byte address
        case '8':   // 3 byte address
        case '9':   // 4 byte address
        {
            // Start Address - currently ignored

            Flush();
            break;
        }
        
        default:
        {
            Error( lineNum, "Unrecognized S-Record: S%c", line[ 1 ] );
            return false;
        }
    }
    
    return true;

} // ParseSRecordLine

