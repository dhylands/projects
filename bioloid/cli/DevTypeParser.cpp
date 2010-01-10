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
#include "DevTypeParser.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup bioloid
 * @{
 */

typedef struct
{
    const char         *name;
    BLD_RegFmtFunc      fmtFunc;
    BLD_RegParseFunc    parseFunc;

} FieldType_t;

static FieldType_t  gFieldType[] =
{
    { "BaudRate",           BLD_RegFmtBaud,         BLD_RegParseBaud },
    { "RDT",                BLD_RegFmtRDT,          BLD_RegParseRDT },
    { "Angle",              BLD_RegFmtAngle,        BLD_RegParseAngle },
    { "Temperature",        BLD_RegFmtTemp,         BLD_RegParseTemp },
    { "Voltage",            BLD_RegFmtVolt,         BLD_RegParseVolt },
    { "StatusRet",          BLD_RegFmtStatusRet,    BLD_RegParseStatusRet },
    { "Alarm",              BLD_RegFmtAlarm,        NULL},
    { "OnOff",              BLD_RegFmtOnOff,        BLD_RegParseOnOff },
    { "AngularVelocity",    BLD_RegFmtVelocity,     BLD_RegParseVelocity },
    { "Load",               BLD_RegFmtLoad,         NULL },
    { NULL }
};

//***************************************************************************
/**
*   Constructor
*/

DevTypeParser::DevTypeParser()
    : m_fs( NULL ),
      m_devType( NULL ),
      m_allocRegs( 0 ),
      m_addDevTypeFunc( NULL )
{
}

//***************************************************************************
/**
*   Destructor
*/

DevTypeParser::~DevTypeParser()
{
    if ( m_devType != NULL )
    {
        free( m_devType->reg );
    }
    delete m_devType;

    if ( m_fs != NULL )
    {
        fclose( m_fs );
    }
}

//***************************************************************************
/**
*   Parses a device type file
*/

bool DevTypeParser::ParseFile( const char *fileName, AddDevTypeFunc addDevTypeFunc )
{
    m_addDevTypeFunc = addDevTypeFunc;
    m_fileName = fileName;
    m_lineNum = 0;

    if (( m_fs = fopen( fileName, "r" )) == NULL )
    {
        LogError( "Unable to open '%s' for reading\n", fileName );
        return false;
    }

    while ( fgets( m_lineBuf, sizeof( m_lineBuf ), m_fs ) != NULL )
    {
        char    *s;

        m_lineNum++;

        // Strip off comments

        if (( s = strchr( m_lineBuf, '#' )) != NULL )
        {
            *s = '\0';
        }

        m_line.Init( m_lineBuf, m_token, sizeof( m_token ));

        if ( !ParseLine())
        {
            return false;
        }
    }
    return true;
}

//***************************************************************************
/**
*   Parses one line from the device type file
*/

bool DevTypeParser::ParseLine()
{
    const char *token = m_line.NextToken();

    if ( token == NULL )
    {
        // Ignore blank lines

        return true;
    }

    if ( m_devType != NULL )
    {
        if ( stricmp( token, "Model:" ) == 0 )
        {
            if ( !m_line.NextNum( &m_devType->model ))
            {
                LogError( "Numeric model required: found '%s' %s:%d\n", m_line.PrevToken(), m_fileName, m_lineNum );
                return false;
            }

            return true;
        }

        if ( stricmp( token, "Register:" ) == 0 )
        {
            BLD_Reg_t   reg;

            if ( !ParseRegister( &reg ))
            {
                return false;
            }

            if ( m_devType->numRegs >= m_allocRegs )
            {
                m_allocRegs += 8;
                m_devType->reg = (BLD_Reg_t *)realloc( m_devType->reg, m_allocRegs * sizeof( m_devType->reg[0] ));
            }
            m_devType->reg[ m_devType->numRegs ] = reg;
            m_devType->numRegs++;

            return true;
        }

        if ( stricmp( token, "EndDeviceType" ) == 0 )
        {
            LogVerbose( "Parsed device type: '%s' %d registers\n", 
                        m_devType->devTypeStr, m_devType->numRegs );

            m_addDevTypeFunc( m_devType );
            m_devType = NULL;

            return true;
        }

        LogError( "Unrecognized keyword: '%s' %s:%d\n", token, m_fileName, m_lineNum );
        return false;
    }

    if ( stricmp( token, "DeviceType:" ) == 0 )
    {
        if ( m_devType != NULL )
        {
            LogError( "Duplicate DeviceType or missing EndDeviceType %s:%d\n", m_fileName, m_lineNum );
            return false;
        }

        m_devType = new BLD_DevType_t;
        memset( m_devType, 0, sizeof( *m_devType ));

        const char *devTypeStr = m_line.NextToken();

        if ( devTypeStr == NULL )
        {
            LogError( "DeviceType: requires a device type string: %s:%d\n", m_fileName, m_lineNum );
            return false;
        }

        StrMaxCpy( m_devType->devTypeStr, devTypeStr, sizeof( m_devType->devTypeStr ));
        return true;
    }

    LogError( "Unrecognized keyword: '%s' %s:%d\n", token, m_fileName, m_lineNum );
    return false;
}

//***************************************************************************
/**
*   Parses A register line
*/

bool DevTypeParser::ParseRegister( BLD_Reg_t *reg )
{
    char   *s;
    uint8_t numBytes;

    memset( reg, 0, sizeof( *reg ));

    // offset name numBytes ro/rw min max type

    if ( !m_line.NextNum( &reg->address))
    {
        LogError( "Invalid register address %s:%d\n", m_fileName, m_lineNum );
        return false;
    }

    if (( s = m_line.NextToken()) == NULL )
    {
        LogError( "No register name specified %s:%d\n", m_fileName, m_lineNum );
        return false;
    }
    StrMaxCpy( reg->name, s, sizeof( reg->name ));
    
    if ( !m_line.NextNum( &numBytes ))
    {
        LogError( "Register %s: Invalid numBytes %s:%d\n", reg->name, m_fileName, m_lineNum );
        return false;
    }
    if (( numBytes < 1 ) || ( numBytes > 2 ))
    {
        LogError( "Register %s: numBytes must be 1 or 2, found: %d %s:%d\n", reg->name, numBytes, m_fileName, m_lineNum );
        return false;
    }
    reg->flags = 0;
    if ( numBytes == 2 )
    {
        reg->flags |= BLD_REG_FLAG_16BIT;
    }

    if ( reg->address + numBytes > m_devType->numRegBytes )
    {
        m_devType->numRegBytes = reg->address + numBytes;
    }

    if (( s = m_line.NextToken()) == NULL )
    {
        LogError( "Register %s: No mode specified (ro/rw) %s:%d\n", reg->name, m_fileName, m_lineNum );
        return false;
    }

    if ( stricmp( s, "ro" ) == 0 )
    {
        reg->flags |= BLD_REG_FLAG_RD;
    }
    else
    if ( stricmp( s, "rw" ) == 0 )
    {
        reg->flags |= BLD_REG_FLAG_RW;
    }
    else
    {
        LogError( "Register %s: Invalid mode specified. Expecting ro/rw, found: '%s' %s:%d\n", reg->name, s, m_fileName, m_lineNum );
        return false;
    }

    // min/max fields are optional for read-only files

    uint16_t    minVal;
    uint16_t    maxVal;

    if ( !m_line.NextNum( &minVal ))
    {
        if (( reg->flags & BLD_REG_FLAG_WR ) != 0 )
        {
            LogError( "Register %s: Min and Max required for writable registers %s:%d\n", reg->name, m_fileName, m_lineNum );
            return false;
        }

        // No more tokens - we're done successfully for Read-Only

        return true;
    }

    if ( !m_line.NextNum( &maxVal ))
    {
        LogError( "Register %s: must specify max val when using min val %s:%d\n", reg->name, m_fileName, m_lineNum );
        return false;
    }
    if ( maxVal < minVal )
    {
        LogError( "Register %s: maxVal (%u) must be >= minVal (%u) %s:%d\n", reg->name, maxVal, minVal, m_fileName, m_lineNum );
        return false;
    }
    if ((( reg->flags & BLD_REG_FLAG_16BIT ) == 0 ) && ( maxVal > 255 ))
    {
        LogError( "Register %s: maxVal (%u) must be <= 255 for 1 byte fields %s:%d\n", reg->name, maxVal, m_fileName, m_lineNum );
        return false;
    }
    reg->minVal = minVal;
    reg->maxVal = maxVal;

    if (( s = m_line.NextToken()) != NULL )
    {
        FieldType_t *fieldType;

        // Check for support formats.

        for ( fieldType = gFieldType; fieldType->name != NULL; fieldType++ )
        {
            if ( stricmp( s, fieldType->name ) == 0 )
            {
                reg->fmtFunc   = fieldType->fmtFunc;
                reg->parseFunc = fieldType->parseFunc;

                return true;
            }
        }

        LogError( "Register %s: unrecognized field type: '%s' %s:%d\n", reg->name, s, m_fileName, m_lineNum );
        return false;
    }

    return true;
}

