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
*   @file   AvrInfo.c 
*
*   @brief  Contains fucntions for mapping AVR Part numbers to information
*           structures.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdlib.h> // For NULL
#include "AvrInfo.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

#define AVR_ENTRY( partNum )    \
{ \
    .m_string       = AVR_PART_STRING( partNum ),       \
    .m_signature    = AVR_PART_SIGNATURE( partNum ),    \
    .m_flashSize    = AVR_PART_FLASH_SIZE( partNum ),   \
    .m_bootSize     = AVR_PART_BOOT_SIZE( partNum ),    \
    .m_eepromSize   = AVR_PART_EEPROM_SIZE( partNum ),  \
    .m_pageSize     = AVR_PART_PAGE_SIZE( partNum ),    \
    .m_pageShift    = AVR_PART_PAGE_SHIFT( partNum )    \
}

/* ---- Private Variables ------------------------------------------------- */

static const AvrInfo_t gAvrInfo[] =
{
    AVR_ENTRY( ATMEGA_48 ),
    AVR_ENTRY( ATMEGA_8 ),
    AVR_ENTRY( ATMEGA_88 ),
    AVR_ENTRY( ATMEGA_16 ),
    AVR_ENTRY( ATMEGA_168 ),
    AVR_ENTRY( ATMEGA_169 ),
    AVR_ENTRY( ATMEGA_32 ),
    AVR_ENTRY( ATMEGA_64 ),
    AVR_ENTRY( ATMEGA_644 ),
    AVR_ENTRY( ATMEGA_128 ),
};

static const unsigned gNumAvrInfoEntries = sizeof( gAvrInfo ) / sizeof( gAvrInfo[ 0 ]);

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Searches through the AVR Info table, looking for a matching part.
*/
const AvrInfo_t *FindAvrInfoBySignature( uint16_t signature )
{
    int i;

    for ( i = 0; i < gNumAvrInfoEntries; i++ ) 
    {
        if ( gAvrInfo[ i ].m_signature == signature )
        {
            return &gAvrInfo[ i ];
        }
    }

    return NULL;

} // FindAvrInfoBySignature

