/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of the 
*   BSD license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file   crc32.c
*
*   @brief  Program for calculating 32 bit CRC's which are compatible with
*           zip and/or u-boot.
*
****************************************************************************/

#include <stdio.h>
#include <stdint.h>

uint32_t    gResidueTable[ 256 ];

//***************************************************************************
/**
*   Builds the lookup table for processing 8-bits at a time.
*/

void BuildResidueTable( void )
{
    uint32_t    polynomial = 0xedb88320uL;
    uint32_t    byteVal;

    for ( byteVal = 0; byteVal < 256; byteVal++ ) 
    {
        uint32_t    residue = 0;
        uint32_t    testVal = byteVal;
        int         bitNum;

        for ( bitNum = 0; bitNum < 8; bitNum++ )
        {
            if ((( residue ^ testVal ) & 1uL ) == 0 )
            {
                residue >>= 1;
            }
            else
            {
                residue >>= 1;
                residue ^= polynomial;
            }
            testVal >>= 1;
        }
        gResidueTable[ byteVal ] = residue;
    }

} // BuildResidueTable

//***************************************************************************
/**
*   Calculate the 32-bit CRC for a file.
*/

int main( int argc, char **argv )
{
    int     arg;
    uint8_t buf[ 1024 ];

    BuildResidueTable();

    for ( arg = 1; arg < argc; arg++ ) 
    {
        FILE    *fs;
        char    *fileName = argv[ arg ];

        if (( fs = fopen( fileName, "rb" )) == NULL )
        {
            fprintf( stderr, "Unable to open '%s' for reading\n", fileName );
        }
        else
        {
            size_t  numBytes;
            int     i;
            uint32_t    crc = ~0uL;

            while (( numBytes = fread( buf, 1, sizeof( buf ), fs )) > 0 )
            {
                for ( i = 0; i < numBytes; i++ ) 
                {
                    crc = gResidueTable[( crc & 0xffuL ) ^ buf[ i ]] ^ ( crc >> 8 );
                }
            }
            crc = ~crc;

            fclose( fs );

            printf( "0x%08lx %s\n", crc, fileName );
        }
    }
    return 0;
}

