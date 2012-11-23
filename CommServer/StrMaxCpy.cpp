/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*	Permission is granted to any individual or institution to use, copy, or
*	redistribute this software so long as it is not sold for profit, and that
*	this copyright notice is retained.
*
****************************************************************************/
/**
*
*  @file    StrMaxCpy.cpp
*
*  @brief   Implements StrMaxCpy, a bounded string copy routine.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include "Str.h"
#include <string.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup Str
 * @{
 */

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

/** @} */

