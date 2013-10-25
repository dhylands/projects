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
/**
*  @defgroup   SRecord  Classes for parsing S-Record files.
*
****************************************************************************/

#if !defined( SREC_H )
#define SREC_H           /**< Include Guard                           */

// ---- Include Files ------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

/**
 * @addtogroup SRecord
 * @{
 */

// ---- Constants and Types ------------------------------------------------

/**
 * Describes the S-Record header (S-Record type 0). Each member is null
 * terminated.
 */

struct SRecordHeader
{
   unsigned m_lineNum;        //!< Line number of header in S-Record file.
   char     m_module[ 21 ];   //!< Module Name.
   char     m_ver[ 3 ];       //!< Version Number.
   char     m_rev[ 3 ];       //!< Revision Number.
   char     m_comment[ 37 ];  //!< Text Comment.
};

/**
 * Describes an S-Record which contains data (S-Record types 1, 2, and 3)
 */

struct SRecordData
{
   unsigned       m_lineNum;        //!< Line number of data record in S-Record file.
   unsigned       m_addr;           //!< Address that was parsed.
   unsigned       m_addrLen;        //!< Number of bytes in the address.
   unsigned char  m_data[ 64 ];     //!< Data that was parsed.
   unsigned char  m_dataLen;        //!< Number of bytes of data.
   unsigned char  m_recType;        //!< Type of S-Record.
   unsigned char  m_checksumCalc;   //!< Checksum that we calculated.
   unsigned char  m_checksumFound;  //!< Checksum found in the S-Record.
};

// ---- Variable Externs ---------------------------------------------------

// ---- Classes ------------------------------------------------------------

/**
 * Class for parsing S-Record files.
 */

class SRecordParser
{
public:

   //-----------------------------------------------------------------------
   /// Default Constructor.
   
   SRecordParser();

   //-----------------------------------------------------------------------
   /// Destructor.
   
   virtual  ~SRecordParser();

   //-----------------------------------------------------------------------
   /**
   *  Forces an open segment (if any) to be finished.
   */

   bool Flush();

   //-----------------------------------------------------------------------
   /**
   *  Parses the S-Record file named by @a fileName.
   *
   *  @param   fileName (in)  Name of the file to parse.
   *
   *  @return  true if the file was parsed successfully, false otherwise.
   */
   
   bool  ParseFile( const char *fileName );
   
   //-----------------------------------------------------------------------
   /**
   *  Parses the S-Record file indicated by @a fs.
   *
   *  @param   fs (mod)  File to parse (assumes that it's positioned correctly).
   *
   *  @return  true if the file was parsed successfully, false otherwise.
   */
   
   bool  ParseFile( FILE *fs );
   
   //-----------------------------------------------------------------------
   /**
   *  Parses a single line from an S-Record file.
   *
   *  @param   line  (in)  Line from S-Record file to parse.
   *
   *  @return  true if the line was parsed successfully, false otherwise.
   */
   
   bool  ParseLine( unsigned lineNum, const char *line );

protected:

   //-----------------------------------------------------------------------
   /**
   *  Called when an S-Record data line is parsed. This is intended to
   *  be over-ridden by a derived class.
   *
   *  @param   sRecInfo (in)  Contains the information that was parsed.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   virtual  bool  Data( const SRecordData *sRecData );

   //-----------------------------------------------------------------------
   /**
   *  Called when a segment has been finished. This is intended to be
   *  over-ridden by a derived class.
   *
   *  @param   addr  (in) Address of the segment just finished.
   *  @param   len   (in) Length of the segment just finished.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */

   virtual  bool  FinishSegment( unsigned addr, unsigned len );
   
   //-----------------------------------------------------------------------
   /**
   *  Called when an S-Record header line is parsed. This is intended to
   *  be over-ridden by a derived class.
   *
   *  @param   sRecHdr (in)  Contains the information that was parsed.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   virtual  bool  Header( const SRecordHeader *sRecHdr );
   
   //-----------------------------------------------------------------------
   /**
   *  Called when an error is encountered parsing the file. This function
   *  currently sends an error message to std::cerr, but can be over-ridden
   *  by a derived class requiring different behaviour.
   *
   *  @param   sRecInfo (in)  Contains the information that was parsed.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   virtual  void  ParseError( unsigned lineNum, const char *str );
   
   //-----------------------------------------------------------------------
   /**
   *  Called when an S-Record start address line is parsed. This function
   *  is intended to be over-ridden by a derived class.
   *
   *  @param   sRecInfo (in)  Contains the information that was parsed.
   *           In particular, the m_addr field contains the parsed start 
   *           address.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   virtual  bool  StartAddress( const SRecordData *sRecData );

   //-----------------------------------------------------------------------
   /**
   *  Called by ParsedData whenever a new segment is detected. This function
   *  is intended to be over-ridden by a derived class.
   *
   *  @param   addr  (in)  Address of the new segment.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   virtual  bool  StartSegment( unsigned addr );

private:

   //-----------------------------------------------------------------------
   /**
   *  Parses a single byte from a string containing ASCII Hex characters.
   *
   *  @param   s        (mod) Pointer to string. Will be advanced.
   *  @param   b        (out) Byte that was parsed.
   *  @param   lineNum  (in)  Line number, used for reporting errors).
   *  @param   label    (in)  Error string (used for reporting errors).
   *
   *  @return  true, if a byte was parsed successfully, false otherwise.
   */

   bool GetByte
   (
      const char   **s, 
      unsigned char *b, 
      unsigned       lineNum, 
      const char    *label 
   );

   //-----------------------------------------------------------------------
   /**
   *  Parses a single nibble from a string containing ASCII Hex characters.
   *
   *  @param   s        (mod) Pointer to string. Will be advanced.
   *  @param   b        (out) nibble that was parsed.
   *  @param   lineNum  (in)  Line number, used for reporting errors).
   *  @param   label    (in)  Error string (used for reporting errors).
   *
   *  @return  true, if a nibble was parsed successfully, false otherwise.
   */
   
   bool GetNibble
   (
      const char   **s, 
      unsigned char *b ,
      unsigned       lineNum, 
      const char    *label 
   );

   //-----------------------------------------------------------------------
   /**
   *  Called when an error is encountered parsing the file.
   *
   *  @param   fmt (in)  As per printf.
   */
   
   void  Error( unsigned lineNum, const char *fmt, ... );

   //-----------------------------------------------------------------------
   /**
   *  Called when an S-Record data line is parsed. This routine will, 
   *  in turn, call StartSegment(), Data, and FinishSegment() as 
   *  appropriate.
   *
   *  @param   sRecInfo (in)  Contains the information that was parsed.
   *
   *  @return  true if parsing should continue, false if it should stop.
   */
   
   bool  ParsedData( const SRecordData *sRecData );

   //-----------------------------------------------------------------------
   /**
   *  Called when an error is encountered parsing the file.
   *
   *  @param   fmt   (in)  As per vprintf.
   *  @param   args  (in)  As per vprintf.
   */
   
   void  vError( unsigned lineNum, const char *fmt, va_list args );
   
   //-----------------------------------------------------------------------

   bool        m_inSeg;    ///< Are we currently inside a segment?
   unsigned    m_segAddr;  ///< Address of segment currently being parsed.
   unsigned    m_segLen;   ///< Length of segment currently being parsed.
};

/** @} */

#endif /* SREC_H */

