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

#include <stdint.h>
#include <stdlib.h>

/**
 * @addtogroup Error
 * @{
 */


//---------------------------------------------------------------------------

#if defined( __cplusplus )
extern "C"
{
#endif

char *GetErrorStr( uint32_t errNum, char *errStr, size_t maxLen );
char *GetLastErrorStr( char *errStr, size_t maxLen );

#if !defined( WIN32 )

// Define some routines for compatability with Win32

uint32_t    GetLastError();
uint32_t    WSAGetLastError();

#endif

#if defined( __cplusplus )
}
#endif

/** @} */

#endif // ERROR_H

