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
*   @file   bioloid-reg.h
*
*   @brief  Contains definitions for lookup tables used to define registers
*           on bioloid devices.
*
****************************************************************************/

#if !defined( BIOLOID_REG_H )
#define BIOLOID_REG_H       /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>

#include "StrToken.h"

/**
 * @addtogroup bioloid
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

#define BLD_REG_FLAG_RD     0x01
#define BLD_REG_FLAG_WR     0x02
#define BLD_REG_FLAG_RW     ( BLD_REG_FLAG_RD | BLD_REG_FLAG_WR )

#define BLD_REG_FLAG_16BIT  0x04

#define BLD_REG_FLAG_8_RD   BLD_REG_FLAG_RD
#define BLD_REG_FLAG_8_WR   BLD_REG_FLAG_WR
#define BLD_REG_FLAG_8_RW   BLD_REG_FLAG_RW

#define BLD_REG_FLAG_16_RD  ( BLD_REG_FLAG_RD | BLD_REG_FLAG_16BIT )
#define BLD_REG_FLAG_16_WR  ( BLD_REG_FLAG_WR | BLD_REG_FLAG_16BIT )
#define BLD_REG_FLAG_16_RW  ( BLD_REG_FLAG_RW | BLD_REG_FLAG_16BIT )

struct BLD_Reg_s;
typedef struct BLD_Reg_s BLD_Reg_t;

typedef void (*BLD_RegFmtFunc)( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
typedef bool (*BLD_RegParseFunc)( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );

struct BLD_Reg_s
{
    uint8_t         address;
    char            name[ 20 ];
    uint8_t         flags;
    uint16_t        minVal;
    uint16_t        maxVal;

    BLD_RegFmtFunc      fmtFunc;
    BLD_RegParseFunc    parseFunc;
};

typedef struct
{
    char        devTypeStr[ 16 ];
    uint16_t    model;
    unsigned    numRegs;
    BLD_Reg_t  *reg;

    uint8_t     numRegBytes;

} BLD_DevType_t;

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

#if defined( __cplusplus )
extern "C"
{
#endif

void BLD_RegFmtBaud( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtRDT( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtAngle( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtTemp( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtVolt( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtStatusRet( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtAlarm( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtOnOff( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtVelocity( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );
void BLD_RegFmtLoad( struct BLD_Reg_s *reg, uint16_t val, char *outStr, size_t maxLen );

bool BLD_RegParseBaud( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseRDT( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseAngle( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseTemp( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseVolt( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseStatusRet( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseOnOff( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );
bool BLD_RegParseVelocity( struct BLD_Reg_s *reg, StrTokenizer &line, uint16_t *valp );

#if defined( __cplusplus )
}
#endif

/** @} */

#endif /* BIOLOID_REG_H */

