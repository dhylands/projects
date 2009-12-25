/****************************************************************************
*
*   Copyright (c) 2009 Dave Hylands     <dhylands@gmail.com>
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
*   @file   BioloidCommandLine.cpp
*
*   @brief  This file implements the BioloidCommandLine class, which 
*           parses command lines and sends the commands to devices on the
*   bioloid bus.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <string.h>
#include "Str.h"
#include "DumpMem.h"
#include "Log.h"
#include "Bioloid.h"
#include "BioloidCommandLine.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

static BLD_DevType_t   *gDevType;

static char             gDelim[] = " \r\n\t";

static uint8_t          gBuf[ 80 ];

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup bioloid
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

BioloidCommandLine::BioloidCommandLine()
    : m_bus( NULL )
{
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

BioloidCommandLine::~BioloidCommandLine()
{
}

//***************************************************************************
/**
*   Dumps information about the registers
*/

void BioloidCommandLine::DumpRegInfo( BLD_DevType_t *devType )
{
    BLD_Reg_t   *reg;

    Log( "Addr Size Min  Max Name\n" );
    Log( "---- ---- ---  --- --------------------\n" );

    for ( reg = devType->reg; reg->name != NULL; reg++ )
    {
        if (( reg->flags & BLD_REG_FLAG_WR ) == 0 )
        {
            Log( "0x%02x ro %d          %s\n", 
                 reg->address, reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1, reg->name );
        }
        else
        {
            Log( "0x%02x rw %d %3d %4d %s\n", 
                 reg->address, reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1, 
                 reg->minVal, reg->maxVal, reg->name );
        }
    }
}

//***************************************************************************
/**
*   Parses an offset and some data.
*/

bool BioloidCommandLine::ParseOffsetAndData( StrTokenizer &line, uint8_t *offset, uint8_t *numBytes, uint8_t *data, size_t maxLen )
{
    *numBytes = 0;

    if ( !line.NextNum( gDelim, offset ))
    {
        LogError( "Invalid offset specified: '%s'\n", line.PrevToken() );
        return false;
    }

    while (( *numBytes < maxLen ) && line.NextNum( gDelim, &data[ *numBytes ] ))
    {
        (*numBytes)++;
    }

    return true;
}

//***************************************************************************
/**
*   Parses a register name
*/

bool BioloidCommandLine::ParseRegisterName( StrTokenizer &line, BLD_DevType_t *devType, BLD_Reg_t **outReg )
{
    BLD_Reg_t   *reg;
    char        *regStr;

    if (( regStr = line.NextToken( gDelim )) == NULL )
    {
        LogError( "No register specified\n" );
        return false;
    }

    reg = devType->reg;
    while ( reg->name != NULL )
    {
        if ( strcmp( regStr, reg->name ) == 0 )
        {
            *outReg = reg;
            return true;
        }
        reg++;
    }

    LogError( "Unrecognized register name: '%s'\n", regStr );
    return false;
}

//***************************************************************************
/**
*   Parses the error code and prints the results.
*/
void BioloidCommandLine::AddErrorStr( Bioloid::Error err, Bioloid::Error mask, char *str, size_t maxLen, const char *errStr )
{
    if (( err & mask ) != 0 )
    {
        if ( str[0] != '\0' )
        {
            StrMaxCat( str, ",", maxLen );
        }
        StrMaxCat( str, errStr, maxLen );
    }
}

//***************************************************************************
/**
*   Prints the error code and prints the results.
*/
void BioloidCommandLine::PrintError( Bioloid::Error err )
{
    char   *str = (char *)&gBuf[0];

    str[0] = '\0';

    if ( err == Bioloid::ERROR_NONE )
    {
        return;
    }

    if ( err > 0xff )
    {
        char    *errStr;

        switch ( err )
        {
            case Bioloid::ERROR_NOT_DONE:       errStr = "Not Done";        break;
            case Bioloid::ERROR_TIMEOUT:        errStr = "Timeout";         break;
            case Bioloid::ERROR_TOO_MUCH_DATA:  errStr = "Too Much Data";   break;
            default:                            errStr = "***Unknown***";   break;
        }

        StrMaxCpy( str, errStr, sizeof( gBuf ));
    }
    else
    {
        AddErrorStr( err, Bioloid::ERROR_RESERVED,       str, sizeof( gBuf ), "Reserved" );
        AddErrorStr( err, Bioloid::ERROR_INSTRUCTION,    str, sizeof( gBuf ), "Instruction" );
        AddErrorStr( err, Bioloid::ERROR_OVERLOAD,       str, sizeof( gBuf ), "Overload" );
        AddErrorStr( err, Bioloid::ERROR_CHECKSUM,       str, sizeof( gBuf ), "Checksum" );
        AddErrorStr( err, Bioloid::ERROR_RANGE,          str, sizeof( gBuf ), "Range" );
        AddErrorStr( err, Bioloid::ERROR_OVERHEATING,    str, sizeof( gBuf ), "Over Heating" );
        AddErrorStr( err, Bioloid::ERROR_ANGLE_LIMIT,    str, sizeof( gBuf ), "Angle Limit" );
        AddErrorStr( err, Bioloid::ERROR_INPUT_VOLTAGE,  str, sizeof( gBuf ), "Input Voltage" );
    }
    Log( "%s\n", str );
}

//***************************************************************************
/**
*   Called from the Scan command when a device is found.
*/
static bool DevFound( BioloidBus *bus, BioloidDevice *dev )
{
    uint16_t    model;
    uint8_t     version;

    dev->Read( 0, &model );
    dev->Read( 2, &version );

    Log( "ID: %3d Model: 0x%04x Version: 0x%02x\n", dev->ID(), model, version );

    return true;
}

//***************************************************************************
/**
*   Processes one line of data
*/

bool BioloidCommandLine::ProcessLine( char *lineStr )
{
    char           *devTypeStr;
    BLD_DevType_t  *devType;
    char           *endPtr;
    char            token[ 20 ];
    StrTokenizer    line( lineStr, token, sizeof( token ));

    if ( m_bus == NULL )
    {
        LogError( "SetBus not called\n" );
        return false;
    }

    // Pull out the device type

    if (( devTypeStr = line.NextToken( gDelim )) == NULL )
    {
        // Empty line - ignore

        return true;
    }

    // Check for special non-device type commands

    if ( strcmp( devTypeStr, "action" ) == 0 )
    {
        m_bus->SendAction();

        // The action command is sent as a broadcast, so no response
        // is expected.

        return true;
    }
    if ( strcmp( devTypeStr, "scan" ) == 0 )
    {
        if ( !m_bus->Scan( DevFound ))
        {
            Log( "No devices found\n" );
        }

        return true;
    }

    if ( strcmp( devTypeStr, "quit" ) == 0 )
    {
        return false;
    }

    // Since it's not one of those - assume it's a device type

    devType = gDevType;
    while (( devType->devTypeStr != NULL )
        && ( strcmp( devType->devTypeStr, devTypeStr ) != 0 ))
    {
        devType++;
    }

    if ( devType->devTypeStr == NULL )
    {
        LogError( "Unrecognized device type: '%s'\n", devTypeStr );
        return true;
    }

    Bioloid::ID_t id;

    if ( !line.NextNum( gDelim, &id ))
    {
        if ( strcmp( line.PrevToken(), "reg" ) == 0 )
        {
            DumpRegInfo( devType );
            return true;
        }
        LogError( "Invalid ID: '%s'\n", line.PrevToken() );
        return true;
    }
    if ( id >= Bioloid::INVALID_ID )
    {
        LogError( "IDs must be 254 (0xFE) or less\n" );
        return true;
    }
    m_device.SetBusAndID( m_bus, id );

    char *cmdStr;

    if (( cmdStr = line.NextToken( gDelim )) == NULL )
    {
        LogError( "No command specified for %s %u\n", devType->devTypeStr, id );
        return true;
    }

    LogVerbose( "DevType: %s ID: %d Cmd: %s\n", devType->devTypeStr, id, cmdStr );

    if ( strcmp( cmdStr, "ping" ) == 0 )
    {
        Bioloid::Error err;

        if ( id == Bioloid::BROADCAST_ID )
        {
            LogError( "Broadcast ID not valid with ping command\n" );
            return true;
        }

        Log( "%s %d ", devType->devTypeStr, id );
        if (( err = m_device.Ping()) == Bioloid::ERROR_NONE )
        {
            Log( "Response Received\n" );
        }
        else
        {
            PrintError( err );
        }
    }
    else
    if (( strcmp( cmdStr, "read-data" ) == 0 )
    ||  ( strcmp( cmdStr, "rd" ) == 0 ))
    {
        uint8_t offset;
        uint8_t numBytes;

        if ( id == Bioloid::BROADCAST_ID )
        {
            LogError( "Broadcast ID not valid with read-data command\n" );
            return true;
        }

        if ( !line.NextNum( gDelim, &offset ))
        {
            LogError( "Invalid offset specified: '%s'\n", line.PrevToken() );
            return true;
        }
        if ( !line.NextNum( gDelim, &numBytes ))
        {
            LogError( "Invalid numBytes specified: '%s'\n", line.PrevToken() );
            return true;
        }

        PrintError( m_device.Read( offset, gBuf, numBytes ));

        DumpMem( "Read", offset, gBuf, numBytes );
    }
    else
    if (( strcmp( cmdStr, "write-data" ) == 0 )
    ||  ( strcmp( cmdStr, "wd" ) == 0 ))
    {
        uint8_t offset;
        uint8_t numBytes;
        uint8_t data[ 20 ];

        if ( !ParseOffsetAndData( line, &offset, &numBytes, data, sizeof( data )))
        {
            return true;
        }
        PrintError( m_device.Write( offset, data, numBytes ));
    }
    else
    if (( strcmp( cmdStr, "reg-write" ) == 0 )
    ||  ( strcmp( cmdStr, "rw" ) == 0 ))
    {
        uint8_t offset;
        uint8_t numBytes;
        uint8_t data[ 20 ];

        if ( !ParseOffsetAndData( line, &offset, &numBytes, data, sizeof( data )))
        {
            return true;
        }
        m_device.SendDeferredWrite( offset, data, numBytes );
    }
    else
    if ( strcmp( cmdStr, "get" ) == 0 )
    {
        BLD_Reg_t   *reg;

        if ( id == Bioloid::BROADCAST_ID )
        {
            LogError( "Broadcast ID not valid with get command\n" );
            return true;
        }

        if ( strncmp( line.Remainder(), "all", 3 ) == 0 )
        {
            reg = devType->reg;

            Log( "Addr Size Value Name\n" );
            Log( "---- ---- ----- --------------------\n" );

            while ( reg->name != NULL )
            {
                unsigned    val;

                if ( reg->flags & BLD_REG_FLAG_16BIT )
                {
                    uint16_t    val16;

                    m_device.Read( reg->address, &val16 );


                    val = val16;
                }
                else
                {
                    uint8_t    val8;

                    m_device.Read( reg->address, &val8 );

                    val = val8;
                }

                Log( "0x%02x %s %d %5u %s\n", 
                    reg->address, 
                    reg->flags & BLD_REG_FLAG_WR ? "rw" : "ro",
                    reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1, 
                    val, 
                    reg->name );

                reg++;
            }
        }
        else
        {
            if ( !ParseRegisterName( line, devType, &reg ))
            {
                return true;
            }
    
            if (( reg->flags & BLD_REG_FLAG_16BIT ) != 0 )
            {
                uint16_t    val16;
    
                PrintError( m_device.Read( reg->address, &val16, sizeof( val16 )));
                Log( "Read: %u\n", val16 );
            }
            else
            {
                uint16_t    val8;
    
                PrintError( m_device.Read( reg->address, &val8, sizeof( val8 )));
                Log( "Read: %u\n", val8 );
            }
        }
    }
    else
    if ( strcmp( cmdStr, "set" ) == 0 )
    {
        BLD_Reg_t  *reg;
        uint16_t    val16;

        if ( !ParseRegisterName( line, devType, &reg ))
        {
            return true;
        }
        if (( reg->flags & BLD_REG_FLAG_WR ) == 0 )
        {
            LogError( "Register %s is read-only\n", reg->name );
            return true;
        }

        if ( !line.NextNum( gDelim, &val16 ))
        {
            LogError( "Invalid value specified: '%s'\n", line.PrevToken() );
            return true;
        }

        if (( val16 < reg->minVal  ) || ( val16 > reg->maxVal ))
        {
            LogError( "Value %u is out of range (%u - %u)\n", val16, reg->minVal, reg->maxVal );
            return true;
        }

        PrintError( m_device.Write( reg->address, &val16, reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1 ));
    }
    else
    if ( strcmp( cmdStr, "reset" ) == 0 )
    {
        m_device.Reset();
    }
    else
    {
        LogError( "Unrecognized command: '%s'\n", cmdStr );
    }

    return true;
}

//***************************************************************************
/**
*   Register the devices that we'll recognize
*/

void BioloidCommandLine::RegisterDevices( BLD_DevType_t *devType )
{
    gDevType = devType;
}

