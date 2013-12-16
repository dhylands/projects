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
*   @file   Error.cpp
*
*   @brief  Implements the error helper functions.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Error.h"
#include "Str.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Error
 * @{
 */


//***************************************************************************
/**
*   Translates error number @a errNum into a string.
*
*   @returns  A pointer to errStr.
*/

char *GetErrorStr
(
    uint32_t    errNum,   ///< Error number to get the string version for.
    char       *errStr,   ///< Place to store the error string.
    size_t      maxLen   ///< Max length that can be written into errStr.
)
{
    if ( FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, // dwFlags
                        0,                          // lpSource
                        errNum,                     // dwMessageId
                        GetUserDefaultLangID(),     // dwlanguageId
                        errStr,                     // lpBuffer
                        maxLen,                     // nSize
                        NULL ) == 0 )               // arguments
    {
        StrPrintf( errStr, maxLen, "*** Unknown Error: %d ***", errNum );
    }

    return errStr;

} // GetErrorStr

//***************************************************************************
/**
*   Translates the result of calling @a GetLastError() into a string.
*
*   @returns  A pointer to errStr.
*/

char *GetLastErrorStr
(
    char *errStr,   ///< Place to store the error string.
    size_t maxLen   ///< Max length that can be written into errStr.
)
{
    return GetErrorStr( GetLastError(), errStr, maxLen );

} // GetLastErrorStr

/** @} */


