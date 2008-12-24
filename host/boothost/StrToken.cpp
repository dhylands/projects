/****************************************************************************
*
*   Copyright (c) 2003 - 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   StrToken.cpp
*
*   @brief  Implements the string tokenizer 
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "StrToken.h"

#include <string.h>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Str
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

StrTokenizer::StrTokenizer()
    : m_str( NULL ), m_outToken( NULL ), m_maxLen( 0 )

{
}

//***************************************************************************
/**
*   Constructor which specifies string to be tokenized, along with location
*   to store tokens as they're parsed.
*/

StrTokenizer::StrTokenizer
(
    const char *str,    ///< String to be parsed.
    char *outToken,     ///< Place to store the result.
    size_t maxLen       ///< Maximum lenght of @a outToken.
)
    : m_str( str ), m_outToken( outToken ), m_maxLen( maxLen )
{
}

//***************************************************************************
/**
*   Destructor
*/

StrTokenizer::~StrTokenizer()
{
    // Nothing to do.
}

//***************************************************************************
/**
*  Initializes the string tokenizer object.
*/

void StrTokenizer::Init
(
    const char *str,    ///< String to be parsed.
    char *outToken,     ///< Place to store the result.
    size_t maxLen       ///< Maximum lenght of @a outToken.
)
{
    m_str       = str;
    m_outToken  = outToken;
    m_maxLen    = maxLen;

} // Init

//***************************************************************************
/**
*   Returns the next token from the previously stored string.
*
*   @returns    A pointer to the next parsed token, or NULL if not more
*               tokens could be found.
*/
char *StrTokenizer::NextToken
(
    const char *delim   ///< Set of delimiter characters which can terminate the token.
)
{
    // Skip over delimiter characters
    
    while (( *m_str != '\0' ) && ( strchr( delim, *m_str ) != NULL ))
    {
        m_str++;
    }

    if ( *m_str == '\0' )
    {
        // We've run out of data - no more tokens

        return NULL;
    }

    // Copy characters until we fill up the token, we run out, or hit a 
    // delimiter.

    size_t i;

    for ( i = 0; i < ( m_maxLen - 1 ); i++ ) 
    {
        if ( *m_str == '\0' )
        {
            break;
        }
        if ( strchr( delim, *m_str ) == NULL )
        {
            m_outToken[ i ] = *m_str++;
        }
        else
        {
            // Delimiter found

            m_str++;
            break;
        }
    }
    m_outToken[ i ] = '\0';
    
    return m_outToken;

} // NextToken

/** @} */

