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
*  Str.c
*
*  PURPOSE:
*
*   This implements a sample program for accessing the serial port.
*
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "Str.h"

#include <string.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/
/**
*  Concatenates source to the destination, but makes sure that the 
*  destination string (including terminating null), doesn't exceed maxLen.
*
*  @param   dst      (mod) String to concatnate onto.
*  @param   src      (in)  String to being added to the end of @a dst.
*  @param   maxLen   (in)  Maximum length that @a dst is allowed to be.
*
*  @return  A pointer to the destination string.
*/

char *StrMaxCat( char *dst, const char *src, size_t maxLen )
{
	size_t	dstLen = strlen( dst );

	if ( dstLen < maxLen )
	{
		StrMaxCpy( &dst[ dstLen ], src, maxLen - dstLen );
	}

	return dst;

} /* StrMaxCat */

/***************************************************************************/
/**
*   Copies the source to the destination, but makes sure that the 
*   destination string (including terminating null), doesn't exceed 
*   maxLen.
*
*   @param  dst     (out) Place to store the string copy.
*   @param  src     (in)  String to copy.
*   @param  maxLen  (in)  Maximum number of characters to copy into @a dst.
*
*   @return A pointer to the destination string.
*/

char *StrMaxCpy( char *dst, const char *src, size_t maxLen )
{
	if ( maxLen < 1 )
	{
		/*
		 * There's no room in the buffer?
		 */

		return "";
	}

	if ( maxLen == 1 )
	{
		/*
		 * There's only room for the terminating null character
		 */

		dst[ 0 ] = '\0';
		return dst;
	}

	/*
	 * The Visual C++ version of strncpy writes to every single character
	 * of the destination buffer, so we use a length one character smaller
	 * and write in our own null (if required).
     *
     * This allows the caller to store a sentinel in the last byte of the
     * buffer to detect overflows (if desired).
	 */

	strncpy( dst, src, maxLen - 1 );
	if (( strlen( src ) + 1 ) >= maxLen )
	{
		/*
		 * The string exactly fits, or probably overflows the buffer.
		 * Write in the terminating null character since strncpy doesn't in
		 * this particular case.
		 *
		 * We don't do this arbitrarily so that the caller can use a sentinel 
		 * in the very end of the buffer to detect buffer overflows.
		 */

		dst[ maxLen - 1 ] = '\0';
	}

	return dst;

} /* StrMaxCpy */

