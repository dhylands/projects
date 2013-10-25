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
*   @file   I2C_BootLoader.cpp
*
*   @brief  This file contains common code shared amongst all I2C_BootLoaders.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "AvrInfo.h"
#include "Log.h"
#include "BootLoader.h"
#include "I2C_BootLoader.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup i2c
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

I2C_BootLoader::I2C_BootLoader( I2C_Adapter *i2cAdapter, I2C_Addr_t slaveAddr )
    : m_adapter( i2cAdapter ),
      m_slaveAddr( slaveAddr )
{
    // Nothing to do
}

//***************************************************************************
/**
*   Destructor
*/

I2C_BootLoader::~I2C_BootLoader()
{
    // Nnothing to do
}

//***************************************************************************
/**
*   Retrieves information about the device we're talking to.
*/

bool I2C_BootLoader::GetBootLoaderInfo( BootLoaderInfo_t *bootInfo )
{
    I2C_Adapter::Buffer buf( bootInfo, sizeof( *bootInfo ));

    buf.Clear();

    return m_adapter->Read( m_slaveAddr, BL_CMD_GET_INFO, &buf );

} // GetBootLoaderInfo

//***************************************************************************
/**
*   Downloads a file to the BootLoader
*/

bool I2C_BootLoader::DownloadFile( FileInfo *fileInfo )
{
    BootLoaderInfo_t    bootInfo;

    // Retrieve the bootloader info

    if ( !GetBootLoaderInfo(  &bootInfo ))
    {
        LogError( "Unable to retrieve bootloader info\n" );
        return false;
    }

    if ( gVerbose )
    {
        PrintBootLoaderInfo( bootInfo );
    }

    // Write the pages out

    FileSegment        *seg;
    I2C_Adapter::Buffer writeBuffer;
    unsigned            offset;
    unsigned            addr;

    Log( "\nProgramming - " );

    for ( seg = fileInfo->head; seg != NULL; seg = seg->next )
    {
        offset = 0;
        addr = seg->address;

        Log( "#" );

        while ( offset < seg->lenThisSegment )
        {
            writeBuffer.data = &seg->data[ offset ];

            if (( offset + BL_MAX_DATA_BYTES ) > seg->lenThisSegment )
            {
                writeBuffer.dataLen = seg->lenThisSegment - offset;
            }
            else
            {
                writeBuffer.dataLen = BL_MAX_DATA_BYTES;
            }

            WriteFlash( addr, &writeBuffer );

            offset += writeBuffer.dataLen;
            addr   += writeBuffer.dataLen;
        }
    }

    // Verify the pages

    uint8_t             buf[ BL_MAX_DATA_BYTES ];
    I2C_Adapter::Buffer readBuffer( buf, sizeof( buf ));

    Log( "\n  Verifying - " );

    for ( seg = fileInfo->head; seg != NULL; seg = seg->next )
    {
        offset = 0;
        addr = seg->address;

        Log( "#" );

        while ( offset < seg->lenThisSegment )
        {
            if (( offset + BL_MAX_DATA_BYTES ) > seg->lenThisSegment )
            {
                readBuffer.dataLen = seg->lenThisSegment - offset;
            }
            else
            {
                readBuffer.dataLen = BL_MAX_DATA_BYTES;
            }

            ReadFlash( addr, &readBuffer );
#if STANDALONE
            if (( seg == fileInfo->head ) && ( offset == 0 ))
            {
                Log( "Skipping verifiction\n" );
            }
#else
            if ( memcmp( readBuffer.data, &seg->data[ offset ], readBuffer.dataLen ) != 0 )
            {
                LogError( "Verify failed at 0x%06x\n", addr );
            }
#endif
            offset += readBuffer.dataLen;
            addr   += readBuffer.dataLen;
        }
    }
    Log( "\n" );

    // Run the application

    Log( "Running application...\n" );

    RunApp();

    Log( "\n" );

    return true;

} // DownloadFile

//***************************************************************************
/**
*   Dumps out the information gleaned from the BootLoader info.
*/

void I2C_BootLoader::PrintBootLoaderInfo( const BootLoaderInfo_t &bootInfo )
{
    const AvrInfo_t *avrInfo = FindAvrInfoBySignature( bootInfo.partNumber );
    const char      *avrString;

    if ( avrInfo == NULL )
    {
        avrString = "*** Unknown ***";
    }
    else
    {
        avrString = avrInfo->m_string;
    }

    Log( "I2C Dev Addr: 0x%02x\n",      bootInfo.devAddr );
    Log( "     Version: %d\n",          bootInfo.version );
    Log( "  MinVersion: %d\n",          bootInfo.minVersion );
    Log( " Part Number: 0x%04x (%s)\n", bootInfo.partNumber, avrString );
    Log( "    Reg Size: %4d\n",         bootInfo.regSize );
    Log( "    RAM Size: %4d\n",         bootInfo.ramSize );
    Log( " EEPROM Size: %4d\n",         bootInfo.eepromSize );
    Log( "   Page Size: %4d\n",         bootInfo.spmPageSize );
    Log( "  Flash Size: %4dk\n",        bootInfo.flashSize / 1024 );

} // PrintBootLoaderInfo

//***************************************************************************
/**
*   Reads data from memory inferred by @a cmd.
*/

bool I2C_BootLoader::ReadMemory( uint8_t cmd, uint32_t addr, I2C_Adapter::Buffer *readBuffer )
{
    BootLoaderReadCmd_t     readCmd;
    BootLoaderReadData_t    readData;

    I2C_Adapter::Buffer cmdBuf( &readCmd, sizeof( readCmd ));
    I2C_Adapter::Buffer dataBuf( &readData, sizeof( readData ));

    cmdBuf.Clear();
    dataBuf.Clear();

    if ( !m_adapter->GetI2CAddress( &readCmd.hostAddr ))
    {
        return false;
    }
    readCmd.numBytes = readBuffer->dataLen;
    if ( readCmd.numBytes > sizeof( readData.data ))
    {
        readCmd.numBytes = sizeof( readData.data );
    }
    readCmd.address = addr;

    if ( !m_adapter->Call( m_slaveAddr, cmd, &cmdBuf, &dataBuf ))
    {
        return false;
    }

    if ( readBuffer->dataLen > readData.numBytes )
    {
        readBuffer->dataLen = readData.numBytes;
    }

    if ( readBuffer->dataLen > 0 )
    {
        memcpy( readBuffer->data, readData.data, readBuffer->dataLen );
    }

    return true;

} // ReadMemory

//***************************************************************************
/**
*   Runs the application that's currently downloaded.
*/

bool I2C_BootLoader::RunApp()
{
    I2C_Adapter::Buffer cmdBuf;

    cmdBuf.Clear();

    return m_adapter->Write( m_slaveAddr, BL_CMD_RUN_APP, &cmdBuf );

} // RunApp

//***************************************************************************
/**
*   Writes data to memory inferred by @a cmd.
*/

bool I2C_BootLoader::WriteMemory( uint8_t cmd, uint32_t addr, I2C_Adapter::Buffer *writeBuffer )
{
    BootLoaderWriteCmd_t    writeCmd;
    BootLoaderWriteResult_t writeResult;

    I2C_Adapter::Buffer cmdBuf( &writeCmd, sizeof( writeCmd ));
    I2C_Adapter::Buffer dataBuf( &writeResult, sizeof( writeResult ));

    cmdBuf.Clear();
    dataBuf.Clear();

    if ( !m_adapter->GetI2CAddress( &writeCmd.hostAddr ))
    {
        return false;
    }
    writeCmd.numBytes = writeBuffer->dataLen;
    if ( writeCmd.numBytes > sizeof( writeCmd.data ))
    {
        writeCmd.numBytes = sizeof( writeCmd.data );
    }
    writeCmd.address = addr;

    if ( writeCmd.numBytes > 0 )
    {
        memcpy( writeCmd.data, writeBuffer->data, writeCmd.numBytes );
    }

    if ( !m_adapter->Call( m_slaveAddr, cmd, &cmdBuf, &dataBuf ))
    {
        return false;
    }
    writeBuffer->dataLen = writeResult.bytesWritten;

    return true;

} // WriteMemory

/** @} */

