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

#include <stddef.h>
#include <string.h>
#include "Str.h"
#include "DumpMem.h"
#include "Log.h"
#include "Bioloid.h"
#include "bioloid-reg.h"
#include "BioloidCommandLine.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

static char             gDelim[] = " \r\n\t";

static uint8_t          gErrorBuf[ 80 ];
static uint8_t          gReadBuf[ 80 ];

// We use the register data structure to parse our global data, so we
// need to make it look sort of like what a real device has (8 and 16-bit
// data types - arranged contigupously in memory)

struct Global_t
{
    uint8_t     m_showPackets;
    uint8_t     m_timeout;

};

static  Global_t    gGlobal;

#define GLBL_OFS(x) offsetof( Global_t, x )

static BLD_Reg_t   gGlobalReg[] =
{
    { GLBL_OFS( m_showPackets ),    "show-packets",     BLD_REG_FLAG_8_RW,  0,    1,  BLD_RegFmtOnOff,  BLD_RegParseOnOff },
    { GLBL_OFS( m_timeout ),        "timeout",          BLD_REG_FLAG_8_RW,  0,  255,  NULL,             NULL },
};

static BLD_DevType_t   gGlobalDevType =
{
    "global",                                       // devTypeStr
    0,                                              // model
    sizeof( gGlobalReg ) / sizeof( gGlobalReg[0] ), // numRegs
    gGlobalReg,                                     // reg
    sizeof( gGlobal ),                              // numRegBytes
};

class GlobalDevice : public BioloidDevice
{
public:
    GlobalDevice();
    virtual ~GlobalDevice();

    virtual Bioloid::Error Read( uint8_t offset, void *data, uint8_t numBytes );
    virtual Bioloid::Error Write( uint8_t offset, const void *data, uint8_t numBytes );

private:

};

GlobalDevice::GlobalDevice()
{
}

GlobalDevice::~GlobalDevice()
{
}

Bioloid::Error GlobalDevice::Read( uint8_t offset, void *data, uint8_t numBytes )
{
    if ( offset + numBytes <= sizeof( gGlobal ))
    {
        memcpy( data, (uint8_t *)&gGlobal + offset, numBytes );
        return Bioloid::ERROR_NONE;
    }
    return Bioloid::ERROR_RANGE;
}

Bioloid::Error GlobalDevice::Write( uint8_t offset, const void *data, uint8_t numBytes )
{
    if ( offset + numBytes <= sizeof( gGlobal ))
    {
        memcpy( (uint8_t *)&gGlobal + offset, data, numBytes );
        return Bioloid::ERROR_NONE;
    }
    return Bioloid::ERROR_RANGE;
}

static  GlobalDevice    gGlobalDev;

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
    BLD_Reg_t  *reg;
    unsigned    regIdx;

    Log( "Addr Size Min  Max Name\n" );
    Log( "---- ---- ---  --- --------------------\n" );

    for ( regIdx = 0; regIdx < devType->numRegs; regIdx++ )
    {
        reg = &devType->reg[ regIdx];

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

    if ( !line.NextNum( offset ))
    {
        LogError( "Invalid offset specified: '%s'\n", line.PrevToken() );
        return false;
    }

    while (( *numBytes < maxLen ) && line.NextNum( &data[ *numBytes ] ))
    {
        (*numBytes)++;
    }

    return true;
}

//***************************************************************************
/**
*   Parses a register name
*/

bool BioloidCommandLine::ParseRegisterName( StrTokenizer &line, BLD_DevType_t *devType, BLD_Reg_t **outRegp )
{
    char        *regStr;
    unsigned     regIdx;

    if (( regStr = line.NextToken()) == NULL )
    {
        LogError( "No register specified\n" );
        return false;
    }

    for ( regIdx = 0; regIdx < devType->numRegs; regIdx++ )
    {
        if ( strcmp( regStr, devType->reg[regIdx].name ) == 0 )
        {
            *outRegp = &devType->reg[regIdx];
            return true;
        }
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
bool BioloidCommandLine::PrintError( Bioloid::Error err )
{
    char   *str = (char *)&gErrorBuf[0];

    str[0] = '\0';

    if ( err == Bioloid::ERROR_NONE )
    {
        return false;
    }

    if ( err > 0xff )
    {
        const char *errStr;

        switch ( err )
        {
            case Bioloid::ERROR_NOT_DONE:       errStr = "Not Done";        break;
            case Bioloid::ERROR_TIMEOUT:        errStr = "Timeout";         break;
            case Bioloid::ERROR_TOO_MUCH_DATA:  errStr = "Too Much Data";   break;
            default:                            errStr = "***Unknown***";   break;
        }

        StrMaxCpy( str, errStr, sizeof( gErrorBuf ));
    }
    else
    {
        AddErrorStr( err, Bioloid::ERROR_RESERVED,       str, sizeof( gErrorBuf ), "Reserved" );
        AddErrorStr( err, Bioloid::ERROR_INSTRUCTION,    str, sizeof( gErrorBuf ), "Instruction" );
        AddErrorStr( err, Bioloid::ERROR_OVERLOAD,       str, sizeof( gErrorBuf ), "Overload" );
        AddErrorStr( err, Bioloid::ERROR_CHECKSUM,       str, sizeof( gErrorBuf ), "Checksum" );
        AddErrorStr( err, Bioloid::ERROR_RANGE,          str, sizeof( gErrorBuf ), "Range" );
        AddErrorStr( err, Bioloid::ERROR_OVERHEATING,    str, sizeof( gErrorBuf ), "Over Heating" );
        AddErrorStr( err, Bioloid::ERROR_ANGLE_LIMIT,    str, sizeof( gErrorBuf ), "Angle Limit" );
        AddErrorStr( err, Bioloid::ERROR_INPUT_VOLTAGE,  str, sizeof( gErrorBuf ), "Input Voltage" );
    }
    Log( "%s\n", str );

    return true;
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

    Log( "ID: %3d Model: %5u Version: %5u\n", dev->ID(), model, version );

    return true;
}

//***************************************************************************
/**
*   Called to process the get and get-raw commands
*/

void BioloidCommandLine::ProcessDeviceGetCommand( BLD_DevType_t  *devType, Bioloid::ID_t id, StrTokenizer &line, bool raw )
{
    BLD_Reg_t  *reg;
    char        str[ 40 ]; 
    unsigned    val;
    int         strWidth;
    int         i;
    unsigned    regIdx;

    if ( id == Bioloid::BROADCAST_ID )
    {
        LogError( "Broadcast ID not valid with get command\n" );
        return;
    }

    if ( strncmp( line.Remainder(), "all", 3 ) == 0 )
    {
        uint8_t numBytes;

        reg = devType->reg;

        numBytes = devType->numRegBytes;
        if ( numBytes > sizeof( gReadBuf ))
        {
            LogError( "gReadBuf is only %d bytes, numRegBytes is %d\n",
                      sizeof( gReadBuf ), devType->numRegBytes );
            return;
        }

        if ( PrintError( m_device.Read( 0, gReadBuf, numBytes )))
        {
            return;
        }

        if ( raw )
        {
            strWidth = 5;
        }
        else
        {
            strWidth = 15;
        }
        for ( i = 0; i < strWidth; i++ )
        {
            str[i] = '-';
        }
        str[i] = '\0';

        Log( "Addr Size %-*s Name\n", strWidth, "Value" );
        Log( "---- ---- %s --------------------\n", str );

        for ( regIdx = 0; regIdx < devType->numRegs; regIdx++ )
        {
            reg = &devType->reg[ regIdx ];

            if ( reg->flags & BLD_REG_FLAG_16BIT )
            {
                val = *(uint16_t *)&gReadBuf[ reg->address ];
            }
            else
            {
                val = gReadBuf[ reg->address ];
            }

            if ( raw )
            {
                snprintf( str, sizeof( str ), "%5u", val );
                strWidth = 5;
            }
            else
            if ( reg->fmtFunc == NULL )
            {
                snprintf( str, sizeof( str ), "%u", val );
            }
            else
            {
                reg->fmtFunc( reg, val, str, sizeof( str ));
            }

            Log( "0x%02x %s %d %-*s %s\n", 
                reg->address, 
                reg->flags & BLD_REG_FLAG_WR ? "rw" : "ro",
                reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1, 
                strWidth, str, 
                reg->name );

            reg++;
        }
    }
    else
    {
        if ( !ParseRegisterName( line, devType, &reg ))
        {
            return;
        }

        if (( reg->flags & BLD_REG_FLAG_16BIT ) != 0 )
        {
            uint16_t    val16;

            if ( PrintError( m_device.Read( reg->address, &val16, sizeof( val16 ))))
            {
                return;
            }

            val = val16;
        }
        else
        {
            uint8_t    val8;

            if (PrintError( m_device.Read( reg->address, &val8, sizeof( val8 ))))
            {
                return;
            }

            val = val8;
        }

        if (( reg->fmtFunc == NULL ) || raw )
        {
            snprintf( str, sizeof( str ), "%5u", val );
        }
        else
        {
            reg->fmtFunc( reg, val, str, sizeof( str ));
        }

        Log( "Read: %s\n", str );
    }
}

//***************************************************************************
/**
*   Called to process the set and set-raw commands
*/

void BioloidCommandLine::ProcessDeviceSetCommand( BLD_DevType_t  *devType, Bioloid::ID_t id, StrTokenizer &line, bool raw )
{
    BLD_Reg_t  *reg;
    uint16_t    val16;

    if ( !ParseRegisterName( line, devType, &reg ))
    {
        return;
    }
    if (( reg->flags & BLD_REG_FLAG_WR ) == 0 )
    {
        LogError( "Register %s is read-only\n", reg->name );
        return;
    }

    if ( raw || ( reg->parseFunc == NULL ))
    {
        if ( !line.NextNum( &val16 ))
        {
            LogError( "Invalid value specified: '%s'\n", line.PrevToken() );
            return;
        }

        if (( val16 < reg->minVal  ) || ( val16 > reg->maxVal ))
        {
            LogError( "Value %u is out of range (%u - %u)\n", val16, reg->minVal, reg->maxVal );
            return;
        }
    }
    else
    {
        if ( !reg->parseFunc( reg, line, &val16 ))
        {
            return;
        }
    }

    PrintError( m_device.Write( reg->address, &val16, reg->flags & BLD_REG_FLAG_16BIT ? 2 : 1 ));
}

//***************************************************************************
/**
*   Processes the global get command.
*/

void ProcessGlobalGetCommand( StrTokenizer &line )
{
    (void)line;
}

//***************************************************************************
/**
*   Processes the global set command.
*/

void ProcessGlobalSetCommand( StrTokenizer &line )
{
    (void)line;
}

//***************************************************************************
/**
*   Processes one line of data
*/

bool BioloidCommandLine::ProcessLine( char *lineStr )
{
    char           *devTypeStr;
    BLD_DevType_t  *devType;
    char            token[ 20 ];
    StrTokenizer    line( lineStr, token, sizeof( token ));
    unsigned        devTypeIdx;

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
        uint8_t numIds;

        if ( !line.NextNum( &numIds ))
        {
            numIds = 32;
        }

        if ( numIds < 100 )
        {
            bool    servoIdFound    = m_bus->Scan( DevFound, 0, numIds );
            bool    sensorIdFound   = m_bus->Scan( DevFound, 100, numIds );

            if ( !servoIdFound && !sensorIdFound )
            {
                Log( "No devices found\n" );
            }
        }
        else
        {
            if ( !m_bus->Scan( DevFound, 0 , numIds ))
            {
                Log( "No devices found\n" );
            }
        }

        return true;
    }

    if ( strcmp( devTypeStr, "dev-types" ) == 0 )
    {
        for ( devTypeIdx = 0; devTypeIdx < m_numDevTypes; devTypeIdx++ )
        {
            devType = m_devType[devTypeIdx];

            Log( "%-10s Model: %5u %2d registers\n", 
                 devType->devTypeStr, devType->model, devType->numRegs );
        }
        return true;
    }

    if ( strcmp( devTypeStr, "quit" ) == 0 )
    {
        return false;
    }

    // Since it's not one of those - assume it's a device type

    devType = NULL;
    for ( devTypeIdx = 0; devTypeIdx < m_numDevTypes; devTypeIdx++ )
    {
        if ( strcmp( m_devType[ devTypeIdx ]->devTypeStr, devTypeStr ) == 0 )
        {
            devType = m_devType[ devTypeIdx ];
            break;
        }
    }

    if ( devTypeIdx >= m_numDevTypes )
    {
        LogError( "Unrecognized device type: '%s'\n", devTypeStr );
        return true;
    }

    Bioloid::ID_t id;

    if ( !line.NextNum( &id ))
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

    if (( cmdStr = line.NextToken()) == NULL )
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

        if (( err = m_device.Ping()) == Bioloid::ERROR_NONE )
        {
            Log( "%s %d Response Received\n", devType->devTypeStr, id );
        }
        else
        {
            Log( "%s %d ", devType->devTypeStr, id );
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

        if ( !line.NextNum( &offset ))
        {
            LogError( "Invalid offset specified: '%s'\n", line.PrevToken() );
            return true;
        }
        if ( !line.NextNum( &numBytes ))
        {
            LogError( "Invalid numBytes specified: '%s'\n", line.PrevToken() );
            return true;
        }
        if ( numBytes > sizeof( gReadBuf ))
        {
            LogError( "Only able to a maximum of %d bytes\n", sizeof( gReadBuf ));
            return true;
        }

        if ( PrintError( m_device.Read( offset, gReadBuf, numBytes )))
        {
            return true;
        }

        DumpMem( "Read", offset, gReadBuf, numBytes );
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
        ProcessDeviceGetCommand( devType, id, line, false );
    }
    else
    if ( strcmp( cmdStr, "get-raw" ) == 0 )
    {
        ProcessDeviceGetCommand( devType, id, line, true );
    }
    else
    if ( strcmp( cmdStr, "set" ) == 0 )
    {
        ProcessDeviceSetCommand( devType, id, line, false );
    }
    else
    if ( strcmp( cmdStr, "set-raw" ) == 0 )
    {
        ProcessDeviceSetCommand( devType, id, line, true );
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

void BioloidCommandLine::RegisterDeviceTypes( unsigned numDevTypes, BLD_DevType_t **devType )
{
    m_numDevTypes = numDevTypes;
    m_devType = devType;
}

