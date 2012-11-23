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
*   @file   LogMessage.h
*
*   @brief  Implements a mechanism for logging messages
*
****************************************************************************/
/**
*   @defgroup   Log Message Logging
*
*   @brief      Provides classes and macros to encapsulate logging.
*
****************************************************************************/

#if !defined( LOGMESSAGE_H )
#define LOGMESSAGE_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <sstream>
#include <iomanip>

/**
 * @addtogroup Log
 * @{
 */

/** @def LOG( severity, expr )  Logs a  message.                            */
/** @def LOG_INFO( expr )       Logs an informational message.              */
/** @def LOG_DEBUG( expr )      Logs a debugging message.                   */
/** @def LOG_WARNING( expr )    Logs a warning message.                     */
/** @def LOG_ERROR( expr )      Logs an error message.                      */
/** @def LOG_ASSERT( expr )     Logs an assertion failure.                  */
/** @def LOG_FATAL( expr )      Logs a fatal error.                         */

#define LOG( sev, expr )    do { LogMessage m; m.Stream() << expr; m.Log( sev, __FILE__, __LINE__ ); } while ( 0 )
#define LOG_INFO( expr )    LOG( LogMessage::Info,    expr )
#define LOG_DEBUG( expr )   LOG( LogMessage::Debug,   expr )
#define LOG_WARNING( expr ) LOG( LogMessage::Warning, expr )
#define LOG_ERROR( expr )   LOG( LogMessage::Error,   expr )
#define LOG_ASSERT( expr )  LOG( LogMessage::Assert,  expr )
#define LOG_FATAL( expr )   LOG( LogMessage::Fatal,   expr )

#define HEX(x)  std::setfill( '0' ) << std::hex << (x) << std::dec

//---------------------------------------------------------------------------
/**
*   The LogMessage class ...
*/

class LogMessage
{
public:

    /** 
     * The severity describes the severity of the message being logged.
     *
     * Fatal errors cause the program to be aborted.
     */

    enum Severity
    {                               
        Info,       ///< Informational.
        Debug,      ///< Debug.
        Warning,    ///< Warning.
        Error,      ///< Error.
        Assert,     ///< Assertion failure
        Fatal       ///< Fatal error. Causes program termination.
    };

    /**
     * Pointer to a function which does the real logging.
     */

    typedef void (*Handler)( Severity sev, const char *msg, const char *file, int lineNum );

    //------------------------------------------------------------------------
    // Default Constructor

    LogMessage();

    //------------------------------------------------------------------------
    // The compiler generated constructor and destructor are fine.

    //------------------------------------------------------------------------
    // Logs a previously collected message

    void Log( Severity sev, const char *file, int lineNum );

    //------------------------------------------------------------------------
    // Sets the handler used for logging messages

    static void SetHandler( Handler handler );

    //------------------------------------------------------------------------
    // Unlike Log, this function requires the newline to be provided.

    static void PrintStr( Severity sev, const char *msg );

    //------------------------------------------------------------------------
    // Returns the stream object that will be used to collect the message.

    std::ostringstream &Stream();

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    LogMessage( const LogMessage & copy );
    LogMessage &operator =( const LogMessage &rhs );

    //------------------------------------------------------------------------

    std::ostringstream  m_stream;   ///< Place where the message gets collected
    static  Handler     m_handler;  ///< Handler that does the real logging.
};

// ---- Inline Functions ----------------------------------------------------

//***************************************************************************
/**
*   Default Contstructor
*/

inline LogMessage::LogMessage()
{
    // Currently nothing to do
}

//***************************************************************************
/**
*   Sets the handler used for logging messages.
*/

inline void LogMessage::SetHandler
(
    LogMessage::Handler handler     ///< Pointer to a function that will do the real logging.
)
{
    m_handler = handler;
}

//***************************************************************************
/**
*   Returns the stream object that will be used to collect the message.
*/

inline std::ostringstream &LogMessage::Stream()
{
    return m_stream;
}

/** @} */

#endif // LOGMESSAGE_H

