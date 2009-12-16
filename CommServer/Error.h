/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   Error.h
*
*   @brief  Functions to aid in the process of error handling.
*
****************************************************************************/
/**
*   @defgroup   Error   Error Handling function.
*
*   @brief      Contains function to aid in the error handling process.
*
****************************************************************************/

#if !defined( ERROR_H )
#define ERROR_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#if !defined( _WINDOWS_ )
#	include <windows.h>
#endif

/**
 * @addtogroup Error
 * @{
 */

//---------------------------------------------------------------------------

char *GetErrorStr( DWORD errNum, char *errStr, size_t maxLen );
char *GetLastErrorStr( char *errStr, size_t maxLen );

/** @} */

#endif // ERROR_H

