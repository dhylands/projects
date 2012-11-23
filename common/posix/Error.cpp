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

#include <errno.h>
#include <string.h>

#include "Error.h"
#include "Str.h"

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
*   Returns the error number corresponding to the last error. This function
*   is provided for compatability with Win32.
*/

uint32_t GetLastError()
{
    return errno;
}

//***************************************************************************
/**
*   Returns the error number corresponding to the last error. This function
*   is provided for compatability with Win32.
*/

uint32_t WSAGetLastError()
{
    return errno;
}

//***************************************************************************
/**
*   Translates error number @a errNum into a string.
*
*   @returns  A pointer to errStr.
*/

char *GetErrorStr
(
    uint32_t    errNum, ///< Error number to get the string version for.
    char       *errStr, ///< Place to store the error string.
    size_t      maxLen  ///< Max length that can be written into errStr.
)
{
    strerror_r( errNum, errStr, maxLen );

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

