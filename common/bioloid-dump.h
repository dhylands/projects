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
*   @file   bioloid-dump.h
*
*   @brief  Contains functions for dumping bioloid packets.
*
****************************************************************************/

#if !defined( BIOLOID_DUMP_H )
#define BIOLOID_DUMP_H      /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include "bioloid-pkt.h"

/* ---- Constants and Types ---------------------------------------------- */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void BLD_DumpCmdPacket( BLD_Packet_t *pkt );
void BLD_DumpRspPacket( BLD_Packet_t *pkt );

/** @} */

#endif /* BIOLOID_DUMP_H */

