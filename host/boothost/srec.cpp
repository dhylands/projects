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

// ---- Include Files ------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "srec.h"

// ---- Public Variables ---------------------------------------------------
// ---- Private Constants and Types ----------------------------------------
// ---- Private Variables --------------------------------------------------
// ---- Private Function Prototypes ----------------------------------------

static bool GetByte( const char **s, unsigned char *b );
static bool GetNibble( const char **s, unsigned char *b );

// ---- Functions ----------------------------------------------------------

/**
 * @addtogroup SRecord
 * @{
 */

/***************************************************************************/

SRecordParser::SRecordParser()
   : m_inSeg( false ),
     m_segAddr( 0 ),
     m_segLen( 0 )
{
   // Currently nothing to do.
}

/***************************************************************************/
// virtual

SRecordParser::~SRecordParser()
{
   // Currently nothing to do.
}

/***************************************************************************/
// virtual  

bool SRecordParser::Data( const SRecordData *sRecData )
{
   return true;
}

/***************************************************************************/

void SRecordParser::Error( unsigned lineNum, const char *fmt, ... )
{
   va_list  args;

   va_start( args, fmt );
   vError( lineNum, fmt, args );
   va_end( args );
}

/***************************************************************************/
// virtual

bool SRecordParser::FinishSegment( unsigned addr, unsigned len )
{
   return true;
}

/***************************************************************************/

bool SRecordParser::Flush()
{
   if ( m_inSeg )
   {
      if ( !FinishSegment( m_segAddr,  m_segLen ))
      {
         return false;
      }
      m_inSeg = false;
   }
   return true;
}

/***************************************************************************/

bool SRecordParser::GetByte
(
   const char   **s, 
   unsigned char *b, 
   unsigned       lineNum, 
   const char    *label 
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
}

/***************************************************************************/

bool SRecordParser::GetNibble
(
   const char   **s, 
   unsigned char *b ,
   unsigned       lineNum, 
   const char    *label 
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
}

/***************************************************************************/
// virtual

bool SRecordParser::Header( const SRecordHeader *sRecHdr )
{
   return true;
}

/***************************************************************************/

bool SRecordParser::ParsedData( const SRecordData *sRecData )
{
   if (( m_inSeg ) && ( sRecData->m_addr != ( m_segAddr + m_segLen )))
   {
      Flush();
   }

   if ( !m_inSeg )
   {
      m_inSeg = true;
      m_segAddr = sRecData->m_addr;
      m_segLen  = 0;

      if ( !StartSegment( m_segAddr ))
      {
         return false;
      }
   }

   if ( !Data( sRecData ))
   {
      return false;
   }

   m_segLen += sRecData->m_dataLen;

   return true;
}

/***************************************************************************/
// virtual 

void  SRecordParser::ParseError( unsigned lineNum, const char *str )
{
   std::cerr << "Error: ";
   if ( lineNum > 0 )
   {
      std::cerr << "line " << lineNum << ": ";
   }
   std::cerr << str << std::endl;
}

/***************************************************************************/

bool SRecordParser::ParseFile( const char *fileName )
{
   FILE  *fs;

   if (( fs = fopen( fileName, "rt" )) == NULL )
   {
      return false;
   }
   
   bool rc = ParseFile( fs );

   fclose( fs );

   return rc;
}

/***************************************************************************/

bool SRecordParser::ParseFile( FILE *fs )
{
   unsigned lineNum = 0;
   char     line[ 100 ];

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
}

/***************************************************************************/

bool SRecordParser::ParseLine( unsigned lineNum, const char *line )
{
   SRecordData    sRecData;
   SRecordHeader  sRecHdr;
   unsigned char  data[ 50 ];

   memset( &sRecData, 0, sizeof( sRecData ));
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
         memset( &sRecHdr, 0, sizeof( sRecHdr ));

         sRecHdr.m_lineNum = lineNum;
         memcpy( sRecHdr.m_module,  &data[ 2  ], sizeof( sRecHdr.m_module ) - 1 );
         memcpy( sRecHdr.m_ver,     &data[ 22 ], sizeof( sRecHdr.m_ver ) - 1 );
         memcpy( sRecHdr.m_rev,     &data[ 24 ], sizeof( sRecHdr.m_rev ) - 1 );
         memcpy( sRecHdr.m_comment, &data[ 26 ], sizeof( sRecHdr.m_comment ) - 1 );

         Flush();
         Header( &sRecHdr );
         break;
      }

      case '1':
      case '2':
      case '3':
      {
         memset( &sRecData, 0, sizeof( sRecData ));
         
         sRecData.m_lineNum         = lineNum;
         sRecData.m_addrLen         = line[ 1 ] - '1' + 2;
         sRecData.m_recType         = line[ 1 ] - '0';
         sRecData.m_checksumCalc    = checksumCalc;
         sRecData.m_checksumFound   = checksumFound;

         unsigned char *x = data;

         for ( int addrIdx = 0; addrIdx < sRecData.m_addrLen; addrIdx++ ) 
         {
            sRecData.m_addr <<= 8;
            sRecData.m_addr += *x++;
         }
         sRecData.m_dataLen = lineLen - sRecData.m_addrLen - 1;
         memcpy( sRecData.m_data, x, sRecData.m_dataLen );

         ParsedData( &sRecData );
         break;
      }

      case '5':
      {
         Flush();
         break;
      }

      case '7':
      case '8':
      case '9':
      {
         memset( &sRecData, 0, sizeof( sRecData ));
         
         sRecData.m_lineNum         = lineNum;
         sRecData.m_addrLen         = '9' - line[ 1 ] + 2;
         sRecData.m_recType         = line[ 1 ] - '0';
         sRecData.m_checksumCalc    = checksumCalc;
         sRecData.m_checksumFound   = checksumFound;

         unsigned char *x = data;

         for ( int addrIdx = 0; addrIdx < sRecData.m_addrLen; addrIdx++ ) 
         {
            sRecData.m_addr <<= 8;
            sRecData.m_addr += *x++;
         }

         Flush();
         StartAddress( &sRecData );
         break;
      }

      default:
      {
         Error( lineNum, "Unrecognized S-Record: S%c", line[ 1 ] );
         return false;
      }
   }

   return true;
}

/***************************************************************************/
// virtual

bool SRecordParser::StartAddress( const SRecordData *sRecInfo )
{
   return true;
}

/***************************************************************************/
// virtual

bool SRecordParser::StartSegment( unsigned addr )
{
   return true;
}

/***************************************************************************/

void SRecordParser::vError( unsigned lineNum, const char *fmt, va_list args )
{
   char  errStr[ 256 ];

   _vsnprintf( errStr, sizeof( errStr ), fmt, args );

   ParseError( lineNum, errStr );
}

/** @} */

