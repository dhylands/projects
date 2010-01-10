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
*   @file   bioloid-reg.c 
*
*   @brief  Provides formatting routines for formatting various registers
*           found on bioloid devices.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <string.h>

#include "Str.h"
#include "StrToken.h"
#include "Bioloid.h"
#include "bioloid-reg.h"
#include "Log.h"

void BLD_RegFmtBaud( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%d baud", 2000000uL / ( val + 1 ));
}

bool BLD_RegParseBaud( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    uint16_t    baud;

    if ( !line.NextNum( &baud ))
    {
        LogError( "Please specify a numeric baud rate\n" );
        return false;
    }
    *valp = 2000000 / baud - 1;
    if ( *valp > reg->maxVal )
    {
        LogError( "Baud value must be less than %u: got: %u\n", reg->maxVal, *valp );
        return false;
    }

    return true;
}

void BLD_RegFmtRDT( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%d usec", 2 * val );
}

bool BLD_RegParseRDT( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    uint16_t    rdt;

    if ( !line.NextNum( &rdt ))
    {
        LogError( "Please specify a numeric return delay time\n" );
        return false;
    }
    *valp = rdt / 2;
    if ( *valp > reg->maxVal )
    {
        LogError( "RDT value must be less than %u: got: %u\n", reg->maxVal, *valp );
        return false;
    }

    return true;
}

void BLD_RegFmtAngle( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%d degrees", (int)( val * 300ul + 511ul ) / 1023ul );
}

bool BLD_RegParseAngle( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    uint16_t    angle;

    if ( !line.NextNum( &angle ))
    {
        LogError( "Please specify a numeric angle\n" );
        return false;
    }
    *valp = (unsigned)angle * 0x3ffu / 300u;
    if ( angle > 300 )
    {
        LogError( "Angle must be less than 300: got: %u\n", angle );
        return false;
    }

    return true;
}

void BLD_RegFmtTemp( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%dC", val );
}

bool BLD_RegParseTemp( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    uint16_t    temp;

    if ( !line.NextNum( &temp ))
    {
        LogError( "Please specify a numeric temperature\n" );
        return false;
    }
    *valp = temp;
    if ( *valp > reg->maxVal )
    {
        LogError( "Temp value must be less than %u: got: %u\n", reg->maxVal, *valp );
        return false;
    }

    return true;
}

void BLD_RegFmtVolt( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%d.%d volts", val / 10, val % 10 );
}

bool BLD_RegParseVolt( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    double  voltage;

    if ( !line.NextNum( &voltage ))
    {
        LogError( "Please specify a numeric temperature\n" );
        return false;
    }
    *valp = (uint16_t)( voltage / 10.0 );
    if ( *valp < reg->minVal )
    {
        LogError( "Voltage value must be greater than %u: got: %u\n", reg->minVal, *valp );
        return false;
    }
    if ( *valp > reg->maxVal )
    {
        LogError( "Voltage value must be less than %u: got: %u\n", reg->maxVal, *valp );
        return false;
    }

    return true;
}

void BLD_RegFmtStatusRet( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%s", ( val == 0 ) ? "none" :
                                     ( val == 1 ) ? "read" :
                                     ( val == 2 ) ? "all" : "???" );
}

bool BLD_RegParseStatusRet( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    const char *token;

    token = line.NextToken();

    if ( stricmp( token, "none" ) == 0 )
    {
        *valp = 0;
        return true;
    }

    if ( stricmp( token, "read" ) == 0 )
    {
        *valp = 1;
        return true;
    }

    if ( stricmp( token, "all" ) == 0 )
    {
        *valp = 2;
        return true;
    }

    LogError( "Status Return must be none, read, or all, found '%s'\n", token );
    return false;
}

void BLD_RegFmtAlarm( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%s%s%s%s%s%s%s%s",
               (( val & Bioloid::ERROR_INSTRUCTION ) != 0 )   ? "Inst "   : "",
               (( val & Bioloid::ERROR_OVERLOAD ) != 0 )      ? "OvLoad " : "",
               (( val & Bioloid::ERROR_CHECKSUM ) != 0 )      ? "ChkSum " : "",
               (( val & Bioloid::ERROR_RANGE ) != 0 )         ? "Range "  : "",
               (( val & Bioloid::ERROR_OVERHEATING ) != 0 )   ? "OvHeat " : "",
               (( val & Bioloid::ERROR_ANGLE_LIMIT ) != 0 )   ? "AngLim " : "",
               (( val & Bioloid::ERROR_INPUT_VOLTAGE ) != 0 ) ? "InVolt " : "",
               ( val == Bioloid::ERROR_NONE )                 ? "None "   : "" );
}

void BLD_RegFmtOnOff( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%s", val ? "on" : "off" );
}

bool BLD_RegParseOnOff( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    const char *token;

    token = line.NextToken();

    if (( stricmp( token, "on" ) == 0 ) || ( strcmp( token, "1" ) == 0 ))
    {
        *valp = 1;
        return true;
    }
    if (( stricmp( token, "off" ) == 0 ) || ( strcmp( token, "0" ) == 0 ))
    {
        *valp = 0;
        return true;
    }

    LogError( "Expecting on or off, got: '%s'\n", token );
    return false;
}

void BLD_RegFmtVelocity( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    uint32_t    rpm;

    rpm = (( val * 1140 ) + 511 ) / 1023;

    StrPrintf( outStr, maxLen, "%d.%d RPM",
               rpm / 10, rpm % 10 );
}

bool BLD_RegParseVelocity( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp )
{
    double rpm;

    if ( !line.NextNum( &rpm ))
    {
        LogError( "Please specify a numeric rpm\n" );
        return false;
    }
    if (( rpm < 0.0 ) || ( rpm > 114.0 ))
    {
        LogError( "Expecting velocity to be between 0.0 and 114.0: found: %f\n", rpm );
        return false;
    }

    Log( "rpm = %f\n", rpm );

    *valp = (uint16_t)((( rpm / 114.0 ) * 1023.0 ) + 0.5 );

    return true;
}

void BLD_RegFmtLoad( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%s %d",
               ( val & ( 1 << 10 )) ? "CW" : "CCW",
               val & (( 1 << 10 ) - 1 ));
}

