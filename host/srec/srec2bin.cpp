/***************************************************************************
*
*  srec2bin.c
*
*  This program reads an S-Record file and generates a binary file
*  which contains the data.
*
*  Since the address information is lost, this is primarily only useful for
*  taking a peek at the raw data stream.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <iostream>
#include <iomanip>

#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>

#include "srec.h"
#include "SRecMem.h"

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

//**************************************************************************

class SRecord2Bin : public SRecordParser
{
public:
            SRecord2Bin();
   virtual ~SRecord2Bin();

   bool     OpenOutputFile( const char *fileName );
   bool     CloseOutputFile();

protected:

   virtual  bool  FinishSegment( unsigned addr, unsigned len );
   virtual  bool  Header( const SRecordHeader *sRecHdr );
   virtual  bool  Data( const SRecordData *sRecData );
   virtual  bool  StartAddress( const SRecordData *sRecData );
   virtual  bool  StartSegment( unsigned addr );

private:

   std::ofstream                 m_outputFile;
};

//**************************************************************************

SRecord2Bin::SRecord2Bin()
{
}

//**************************************************************************

SRecord2Bin::~SRecord2Bin()
{
   CloseOutputFile();
}

//**************************************************************************

bool SRecord2Bin::CloseOutputFile()
{
    if ( !m_outputFile.is_open() )
    {
        return true;
    }
    
    if ( !Flush() )
    {
        return false;
    }
    
    return true;
}

//**************************************************************************
// virtual

bool SRecord2Bin::Data( const SRecordData *sRecData )
{
    m_outputFile.write( reinterpret_cast< const char * >( &sRecData->m_data[0] ), sRecData->m_dataLen );

    return true;
}

//**************************************************************************
// virtual

bool SRecord2Bin::FinishSegment( unsigned addr, unsigned len )
{
    return true;
}

//**************************************************************************
// virtual

bool SRecord2Bin::Header( const SRecordHeader *sRecHdr )
{
    return true;
}

//**************************************************************************

bool SRecord2Bin::OpenOutputFile( const char *fileName )
{
    m_outputFile.open( fileName, std::ios::binary );
    if ( !m_outputFile )
    {
        std::cerr << "Unable to open '" << fileName << "' for writing." << std::endl;
        return false;
    }
    return true;
}

//**************************************************************************
// virtual

bool SRecord2Bin::StartAddress( const SRecordData *sRecData )
{
    return true;
}

//**************************************************************************
// virtual

bool SRecord2Bin::StartSegment( unsigned addr )
{
    return true;
}

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************
*
*  main
*
****************************************************************************/

int main( int argc, char **argv )
{
    if ( argc != 3 )
    {
        fprintf( stderr, "Usage: srec2bin srec-file bin-file\n" );
        exit( 1 );
    }
    
    SRecord2Bin sRec2Bin;
    
    if ( !sRec2Bin.OpenOutputFile( argv[ 2 ] ))
    {
        exit( 1 );
    }
    
    sRec2Bin.ParseFile( argv[ 1 ]);
    sRec2Bin.CloseOutputFile();
    
    exit( 0 );
    return 0;

} /* main */

