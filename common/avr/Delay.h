/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
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
*   @file    Delay.h
*
*   @brief   Routines for waiting various amounts of time.
*
*****************************************************************************/

#if !defined( DELAY_H )
#define DELAY_H

/* ---- Include Files ----------------------------------------------------- */

#include <inttypes.h>

/* ---- Constants and Types ----------------------------------------------- */

/* ---- Variable Externs -------------------------------------------------- */

/* ---- Function Prototypes ----------------------------------------------- */

#if defined( __cplusplus )
extern "C"
{
#endif

void us_spin( unsigned short us );
void ms_spin( unsigned short ms );

void Delay100uSec( uint8_t num100uSec );
void Delay10uSec( uint8_t num10uSec );

void Delay100mSec( uint8_t num100mSec );
void Delay10mSec( uint8_t num10mSec );
void Delay1mSec( uint8_t num1mSec );

#if defined( __cplusplus )
}
#endif

#endif  // DELAY_H

