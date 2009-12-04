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

#include "Str.h"
#include "Bioloid.h"
#include "bioloid-reg.h"


void BLD_RegFmtBaud( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x (%d baud)", val, 2000000uL / ( val + 1 ));
}

void BLD_RegFmtRDT( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x (%d usec)", val, 2 * val );
}

void BLD_RegFmtAngle( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%03x (%d degrees)", val, (int)( val * 300ul + 511ul ) / 1023ul );
}

void BLD_RegFmtTemp( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x (%dC)", val, val );
}

void BLD_RegFmtVolt( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x (%d.%d volts)", val, val / 10, val % 10 );
}

void BLD_RegFmtStatusRet( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x %s", val, ( val == 0 ) ? "none" :
                                                 ( val == 1 ) ? "read" :
                                                 ( val == 2 ) ? "all" : "???" );
}

void BLD_RegFmtAlarm( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%02x%s%s%s%s%s%s%s%s", val,
               (( val & Bioloid::ERROR_INSTRUCTION ) != 0 )   ? " Inst"   : "",
               (( val & Bioloid::ERROR_OVERLOAD ) != 0 )      ? " OvLoad" : "",
               (( val & Bioloid::ERROR_CHECKSUM ) != 0 )      ? " ChkSum" : "",
               (( val & Bioloid::ERROR_RANGE ) != 0 )         ? " Range"  : "",
               (( val & Bioloid::ERROR_OVERHEATING ) != 0 )   ? " OvHeat" : "",
               (( val & Bioloid::ERROR_ANGLE_LIMIT ) != 0 )   ? " AngLim" : "",
               (( val & Bioloid::ERROR_INPUT_VOLTAGE ) != 0 ) ? " InVolt" : "",
               ( val == Bioloid::ERROR_NONE )                 ? " None"   : "" );
}

void BLD_RegFmtOnOff( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "%d %s", val, val ? "on" : "off" );
}

void BLD_RegFmtVelocity( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%04x %s %d", val, 
               ( val & ( 1 << 10 )) ? "CW" : "CCW",
               val & (( 1 << 10 ) - 1 ));
}

void BLD_RegFmtLoad( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen )
{
    StrPrintf( outStr, maxLen, "0x%04x %s %d", val, 
               ( val & ( 1 << 10 )) ? "CW" : "CCW",
               val & (( 1 << 10 ) - 1 ));
}

