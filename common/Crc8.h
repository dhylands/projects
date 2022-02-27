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
 *   @file   Crc8.h
 *
 *   @brief  This file contains the definition of the CRC-8 algorithim
 *           used by SMBus
 *
 ****************************************************************************/

#pragma once

/* ---- Include Files ----------------------------------------------------- */

#include <inttypes.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

uint8_t Crc8(uint8_t crc, uint8_t data);

uint8_t Crc8Block(uint8_t crc, uint8_t* data, uint8_t len);

#if defined(__cplusplus)
}
#endif

/** @} */
