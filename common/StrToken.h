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
*   @file   StrToken.h
*
*   @brief  String Tokenizer.
*
****************************************************************************/

#if !defined( STRTOKEN_H )
#define STRTOKEN_H           ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

/**
 * @addtogroup Str
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The StrTokenizer class basically wraps up a reentrant version of strtok.
*/

class StrTokenizer
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    StrTokenizer();

    //------------------------------------------------------------------------
    // Constructor which specifies string to be tokenized, along with location
    // to store tokens as they're parsed.

    StrTokenizer( const char *str, char *outToken, size_t maxLen, const char *delim = " \r\n\t" );

    //------------------------------------------------------------------------
    // Destructor

    ~StrTokenizer();

    //------------------------------------------------------------------------

    void Init( const char *str, char *outToken, size_t maxLen, const char *delim = " \r\n\t" );

    //------------------------------------------------------------------------
    
    char *NextToken( const char *delim = NULL );

    //------------------------------------------------------------------------

    char *PrevToken();

    //------------------------------------------------------------------------

    bool NextNum( uint8_t *num, const char *delim = NULL );
    bool NextNum( uint16_t *num, const char *delim = NULL );
    bool NextNum( double *num, const char *delim = NULL );

    //------------------------------------------------------------------------

    const char *Remainder() const;

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    StrTokenizer( const StrTokenizer & copy );
    StrTokenizer &operator =( const StrTokenizer &rhs );

    //------------------------------------------------------------------------

    const char *m_str;      ///< Where we are in the string
    const char *m_delim;    ///< Last delimiters used
    char       *m_outToken; ///< Place to store the parsed token.
    size_t      m_maxLen;   ///< Max size of m_outToken.
};

// ---- Inline Functions ----------------------------------------------------

/***************************************************************************/
/**
*   Returns the remainder of the untokenized string.
*/

inline const char *StrTokenizer::Remainder() const
{
    return m_str;
}

/** @} */

#endif // STRTOKEN_H

