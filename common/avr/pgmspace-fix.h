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

#if !defined( PGMSPACE_FIX_H )
#define PGMSPACE_FIX_H

#include <avr/pgmspace.h>

#define PROGMEM_SECTION __attribute__(( section(".progmem.data") ))

#undef PSTR
#define PSTR(s) (__extension__({static char __c[] PROGMEM_SECTION = (s); &__c[0];}))

#endif  // PGMSPACE_FIX_H

