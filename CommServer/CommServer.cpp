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
*   @file   CommServer.cpp
*
*   @brief  tcpComm server for testing the serial program.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#define _WIN32_WINNT    0x0400  // Needed for IsDebuggerPresent

#include <windows.h>

#include "Error.h"
#include "LogMessage.h"
#include "SerialComm.h"
#include "TcpComm.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------

#if 0
#define DBG(x)    LOG_DEBUG(x)
#else
#define DBG(x)
#endif

void LogHandler( LogMessage::Severity sev, const char *msg, const char *file, int lineNum );
                                                 
int RealMain( int argc, char **argv );

DWORD WINAPI TcpToSerial( LPVOID param );

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Comm
 * @{
 */
 
TcpComm		tcpComm( TcpComm::Server );
SerialComm	serComm;

//***************************************************************************
/**
*   Program entry point. This just wraps the real main for ease of use
*	in the debugger.
*
*   @returns    0 is the program exits successfully, non-zero to indicate 
*               that an error occurred.
*/

int main
(
    int     argc,   ///< Number of command line arguments.
    char  **argv    ///< Array of command line arguments.
)
{
	int rc = RealMain( argc, argv );

    if ( IsDebuggerPresent() )
    {
        printf( "\nPress RETURN to exit\n" );
        getchar();
    }

	return rc;
}

//***************************************************************************
/**
*   Main program.
*
*   @returns    0 is the program exits successfully, non-zero to indicate 
*               that an error occurred.
*/

int RealMain
(
    int     argc,   ///< Number of command line arguments.
    char  **argv    ///< Array of command line arguments.
)
{
    char *serOptions = "";
    char *tcpOptions = "";

    LogMessage::SetHandler( LogHandler );

    if ( argc > 1 )
    {
        serOptions = argv[ 1 ];

        if ( *serOptions == '-' )
        {
            LOG_ERROR( "Usage: CommServer <serial-options> [<TCP port>]" );
            LOG_ERROR( "  where <serial-options> is a single argument which is" );
            LOG_ERROR( "  compatible with the BuildCommDCB function, something" );
            LOG_ERROR( "  like \"COM1: baud=9600 parity=N data=8 stop=1\"" );
            return 1;
        }

        if ( argc > 2 )
        {
            tcpOptions = argv[ 2 ];
        }
    }


    if ( !serComm.Open( serOptions ))
    {
        return 1;
    }
    LOG_INFO( "Serial Port " << serComm.Name() << " opened " << serComm.ConnectionInfo() );

    while ( true )
    {
        if ( !tcpComm.Open( tcpOptions ))
        {
            return 1;
        }

        // Start a thread for TcpToSerial

        CreateThread( NULL, 
                      0, 
                      TcpToSerial,
                      NULL, 
                      0,
                      NULL );

        // Now perform the SerialToTcp functionality

        while ( true )
        {
            char    buf[ 100 ];
            size_t  bytesRead;
            size_t  bytesWritten;

			DBG( "SerialToTcp: Waiting for data from serial port" );

            if ( !serComm.Read( buf, sizeof( buf ), &bytesRead ))
            {
                break;
            }

	        DBG( "SerialToTcp: Writing to TCP\n" );

            if ( !tcpComm.Write( buf, bytesRead, &bytesWritten ))
            {
                break;
            }
        }
		tcpComm.AbortRead();
        tcpComm.Close();

        LOG_INFO( "" );
    }
    serComm.Close();

    DBG( "Exiting CommServer application" );
    return 0;

} // main

//***************************************************************************
/**
*   Handles the log messages.
*/
void LogHandler( LogMessage::Severity sev, const char *msg, const char *file, int lineNum )
{
	char	*sevStr;

    switch ( sev )
    {
        case LogMessage::Info:      sevStr = "INFO";    break;
        case LogMessage::Debug:     sevStr = "DEBUG";   break;
        case LogMessage::Warning:   sevStr = "WARNING"; break;
        case LogMessage::Error:     sevStr = "ERROR";   break;
        case LogMessage::Assert:    sevStr = "ASSERT";  break;
        case LogMessage::Fatal:     sevStr = "FATAL";   break;
        default:                    sevStr = "*****";   break;
    }

    fprintf( stderr, "%s: %s\n", sevStr, msg );
    fflush( stderr );

} // LogHandler

//***************************************************************************
/**
*   Read data from the tcp port and write it to the console.
*/

DWORD WINAPI TcpToSerial
(
    LPVOID param    ///< Parameter passed to CreateThread.
)
{
    DBG( "TcpToSerial started" );

    while ( true )
    {
        size_t  bytesRead;
        size_t  bytesWritten;
        char    buf[ 1024 ];

        DBG( "TcpToSerial: Waiting for data from TCP" );

        if ( !tcpComm.Read( buf, sizeof( buf ), &bytesRead ))
        {
            break;
        }

		if ( bytesRead == 0 )
		{
            LOG_INFO( "Connection terminated by " << tcpComm.ConnectionInfo() );
			break;
		}
        
        DBG( "TcpToSerial: Writing to Serial port\n" );

        if ( !serComm.Write( buf, bytesRead, &bytesWritten ))
        {
            break;
        }
    }
	serComm.AbortRead();

    DBG( "Exiting TcpToSerial" );

	return 0;

} // SerialToConsole


/** @} */
