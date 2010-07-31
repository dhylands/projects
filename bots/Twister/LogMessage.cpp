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
*   @file   LogMessage.cpp
*
*   @brief  Implements the message logging infrastructure.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "LogMessage.h"
#include <sstream>

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Log
 * @{
 */

/**
*	Variable which contains the handler that performs the real logging.	
*/
LogMessage::Handler LogMessage::m_handler = NULL;

//***************************************************************************
/**
*  Function that causes the actual message to be logged.
*/

void LogMessage::Log
(
    LogMessage::Severity sev,       ///< Message severity
    const char          *file,      ///< Filename where log occurs
    int                  lineNum    ///< Line number where log occurs
)
{
    if ( m_handler != NULL )
    {
        std::string str = m_stream.str();

        m_handler( sev, str.c_str(), file, lineNum );
    }

} // Log

//***************************************************************************
/**
*  Printf function that accumulates data until a newline is encountered.
*
* static
*/

void LogMessage::PrintStr
(
    LogMessage::Severity sev,   ///< Message severity
    const char          *str    ///< String to print
)
{
    if ( m_handler == NULL )
    {
        return;
    }

    static  char    line[ 512 ] = { 0 };
    char *endLine = &line[ sizeof( line ) - 1];
    const char *src = str;
    
    do
    {
        int lineLen = strlen( line );

        char *dst = &line[ lineLen ];

        while (( *src != '\0' ) && ( *src != '\n' ) && ( dst < endLine ))
        {
            *dst++ = *src++;
        }
        *dst = '\0';

        if (( *src == '\n' ) || ( dst >= endLine ))
        {
            // Don't use the LOG macro, since it may cause memory allocation
            // to occur, and this routine is often called as part of the
            // process of dumping memory leaks

            m_handler( sev, line, __FILE__, __LINE__ );

            line[ 0 ] = '\0';

            if ( *src == '\n' )
            {
                src++;
            }
        }

    } while ( *src != '\0' );

} // Printf

/** @} */

