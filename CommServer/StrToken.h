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
*   @file   StrToken.h
*
*   @brief  String Tokenizer.
*
****************************************************************************/

#if !defined( STRTOKEN_H )
#define STRTOKEN_H           ///< Include Guard

// ---- Include Files -------------------------------------------------------

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

    StrTokenizer( const char *str, char *outToken, size_t maxLen );

    //------------------------------------------------------------------------
    // Destructor

    ~StrTokenizer();

    //------------------------------------------------------------------------

    void Init( const char *str, char *outToken, size_t maxLen );

    //------------------------------------------------------------------------
    
    char *NextToken( const char *delim );

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

