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
*  @file    Str.h
*
*  @brief   String Manipulation routines.
*
*  Definitions for a variety of string manipulation routine. These routines
*  are all bounded, to ensure that memory overwrites don't occur.
*
****************************************************************************/
/**
*  @defgroup   Str   String Manipulation
*
*  @brief   Provides a variety of string manipulation routines.
*/

// ---- Include Files -------------------------------------------------------

#if !defined( STR_H )
#define STR_H                 ///< Include Guard

#include <stddef.h>
#include <stdarg.h>

/**
 * @addtogroup Str
 * @{
 */

// ---- Constants and Types -------------------------------------------------

/**
 * Pointer to a function which outputs a single character. This function
 * is called by the strXPrintf()/vStrXPrintf() functions to output a
 * character.
 */

typedef int (*StrXPrintfFunc)( void *outParm, int ch );

/**
 * Maintains data used by StrTok
 */

// ---- Variable Externs ----------------------------------------------------
// ---- Function Prototypes -------------------------------------------------

char *StrMaxCpy( char *dst, const char *src, size_t maxLen );
char *StrMaxCat( char *dst, const char *src, size_t maxLen );

int StrPrintf( char *outStr, int maxLen, const char *fmt, ... );
int vStrPrintf( char *outStr, int maxLen, const char *fmt, va_list args );

int StrXPrintf( StrXPrintfFunc func, void *userParm, const char *fmt, ... );
int vStrXPrintf( StrXPrintfFunc func, void *userParm, const char *fmt, va_list args );

#endif // STR_H

