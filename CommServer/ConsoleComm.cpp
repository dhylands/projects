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
*   @file   ConsoleComm.cpp
*
*   @brief  Implements the abstract communications device using a serial port.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Error.h"
#include "LogMessage.h"
#include "ConsoleComm.h"

/**
 * @addtogroup ConsoleComm
 * @{
 */

#define USE_DBG 0
#if USE_DBG
#define DBG(x)    LOG_DEBUG(x)
#else
#define DBG(x)
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
*   Constructor
*/

ConsoleComm::ConsoleComm()
{
    m_stdin  = INVALID_HANDLE_VALUE;
    m_stdout = INVALID_HANDLE_VALUE;

    memset( &m_inputRec, 0, sizeof( m_inputRec ));

    m_abortEvent = NULL;

    m_name = "Console";

    m_quitDetected = false;
}

//***************************************************************************
/**
*   Destructor
*
* virtual
*/

ConsoleComm::~ConsoleComm()
{
    Close();
}

//***************************************************************************
/**
*   Aborts a read, if one is pending.
*
* virtual
*/

void ConsoleComm::AbortRead()
{
    DBG( "ConsoleComm: AbortRead called" );

    if ( m_abortEvent != NULL )
    {
        DBG( "ConsoleComm: AbortRead setting event" );
        SetEvent( m_abortEvent );
    }

} // AbortRead

//***************************************************************************
/**
*   Close a previsouly opened device.
*
* virtual 
*/

void ConsoleComm::Close()
{
    // We didn't open the handles, so we don't close them either.
                            
    if ( m_abortEvent != NULL )
    {
        CloseHandle( m_abortEvent );
        m_abortEvent = NULL;
    }
}

//***************************************************************************
/**
*   Enable line input/editing mode, if supported by the device.
*
* virtual
*/

void ConsoleComm::EnableLineMode( bool enable )
{
    DWORD   inMode;
    DWORD   outMode;

    GetConsoleMode( m_stdin, &inMode );
    GetConsoleMode( m_stdin, &outMode );

    DWORD inMask  = ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT;
    DWORD outMask = ENABLE_PROCESSED_OUTPUT;

    if ( enable )
    {
        inMode |= inMask;
        outMode |= outMask;
    }
    else
    {
        inMode &= ~inMask;
        outMode &= ~outMask;
    }

    SetConsoleMode( m_stdin, inMode );
    SetConsoleMode( m_stdout, outMode );

} // EnableLineMode

//***************************************************************************
/**
*   Opens the console.
*
*   @returns    true if the port was opened successfully, false otherwise.
*
* virtual
*/

bool ConsoleComm::Open
(
    const char *openStr     ///< Not use for the Console
)
{
    m_stdin  = GetStdHandle( STD_INPUT_HANDLE );
    m_stdout = GetStdHandle( STD_OUTPUT_HANDLE );

    EnableLineMode( false );

    m_abortEvent = CreateEvent( NULL,   // lpEventAttributes
                                FALSE,  // bManualReset
                                FALSE,  // bInitialState
                                NULL ); // lpName

    return true;
}

//***************************************************************************
/**
*   Returns true if the device has detected a quit indication. This is
*   typically only implemented by a console device.
*
* virtual
*/
    
bool ConsoleComm::QuitDetected() const
{
    return m_quitDetected;

} // QuitDetected

//***************************************************************************
/**
*   Read data from the communication device.
*
*   @returns    true if the read was successful, flse otherwise.
*
* virtual 
*/

bool ConsoleComm::Read
(
    void   *buf,        ///< (out) Place to store the read data.
    size_t  bufSize,    ///< (in)  Size of the data buffer.
    size_t *bytesRead   ///< (out) Number of bytes actually read.
)
{
	DWORD	err;
    char    errStr[ 200 ];

    *bytesRead = 0;

    size_t  bytesRemaining = bufSize;

    CHAR    *s = (CHAR *)buf;

    while ( true )
    {
        bool processEvent = false;

        if (( m_inputRec.EventType == KEY_EVENT )
        &&  ( m_inputRec.Event.KeyEvent.bKeyDown )
        &&  ( m_inputRec.Event.KeyEvent.wRepeatCount > 0 ))
        {
            if ( m_inputRec.Event.KeyEvent.uChar.AsciiChar != 0x00 )
            {
                processEvent = true;
            }
            else
            if (( m_inputRec.Event.KeyEvent.wVirtualKeyCode == '2' )
            &&  (( m_inputRec.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED ) != 0 )
            &&  (( m_inputRec.Event.KeyEvent.dwControlKeyState & ( LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED )) != 0 )
            &&  (( m_inputRec.Event.KeyEvent.dwControlKeyState & ( LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED )) == 0 ))
            {
                // Special case for Control-@ which should generate a null character

                processEvent = true;
            }
            else
            if (( m_inputRec.Event.KeyEvent.wVirtualKeyCode == VK_F4 )
            &&  (( m_inputRec.Event.KeyEvent.dwControlKeyState & ( LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED )) != 0 ))
            {
                // Alt-F4 sends a quit event

                m_quitDetected = true;
                return false;
            }
        }
        if ( processEvent )
        {
            size_t  bytesToCopy;
    
            bytesToCopy = m_inputRec.Event.KeyEvent.wRepeatCount;
            if ( bytesToCopy > bytesRemaining )
            {
                bytesToCopy = bytesRemaining;
            }
    
            if ( bytesToCopy > 0 )
            {
                memset( s, m_inputRec.Event.KeyEvent.uChar.AsciiChar, bytesToCopy );
                m_inputRec.Event.KeyEvent.wRepeatCount -= bytesToCopy;
                *bytesRead += bytesToCopy;
                s += bytesToCopy;
            }
        
            if ( m_inputRec.Event.KeyEvent.wRepeatCount > 0 )
            {
                // This means that we filled the buffer
    
                break;
            }
        }

        // See how many events are in the queue
    
        DWORD   numEvents;

        if ( !GetNumberOfConsoleInputEvents( m_stdin, &numEvents ))
        {
            LOG_ERROR( "ConsoleComm::Read call to GetNumberOfConsoleInputEvents failed: "
                    << GetLastErrorStr( errStr, sizeof( errStr )));
            return false;
        }
    
        if (( *bytesRead > 0 ) && ( numEvents == 0 ))
        {
            // We've read some data, and there aren't any events in the queue.

            break;
        }

        if ( numEvents == 0 )
        {
            // Wait for an event to show up in the queue.

            HANDLE  waitHandle[ 2 ];

            waitHandle[ 0 ] = m_stdin;
            waitHandle[ 1 ] = m_abortEvent;

			DBG( "ConsoleComm: Waiting for event" );
            DWORD rc = WaitForMultipleObjects( 2, waitHandle, FALSE, INFINITE );

            if ( rc == ( WAIT_OBJECT_0 + 1 ))
            {
                // We've been asked to quit. Cancel the I/O.

				DBG( "ConsoleComm: quit event received" );
                return false;
            }
            if ( rc != WAIT_OBJECT_0 )
            {
                err = GetLastError();

                LOG_ERROR( "ConsoleComm::Read Wait failed: " << GetErrorStr( err, errStr, sizeof( errStr )));
                return false;
            }
        }
        
        // Read the event

		DWORD	numEventsRead;

		DBG( "ConsoleComm: Calling ReadConsoleInput" );
        if ( !ReadConsoleInput( m_stdin, &m_inputRec, 1, &numEventsRead ))
        {
            LOG_ERROR( "ConsoleComm::Read call to ReadConsoleInput failed: "
                    << GetLastErrorStr( errStr, sizeof( errStr )));
            return false;
        }
		DBG( "ConsoleComm: ReadConsoleInput finished" );
    }

	return true;
}

//***************************************************************************
/**
*   Write data to the communication device.
*
*   @returns    true if the write was successful, flse otherwise.
*
* virtual 
*/

bool ConsoleComm::Write
(
    const void *buf,            ///< (in)  Buffer containing data to write
    size_t      bytesToWrite,   ///< (in)  Number of bytes of data to write
    size_t     *bytesWritten    ///< (out) Number of bytes actually written
)
{
	DWORD	bytesWrittenDword;

    if ( !WriteFile( m_stdout, buf, bytesToWrite, &bytesWrittenDword, NULL ))
	{
        DWORD   err = GetLastError();
        char    errStr[ 200 ];

        *bytesWritten = 0;

        LOG_ERROR( "ConsoleComm::Write failed: " 
                << GetErrorStr( err, errStr, sizeof( errStr )));
		return false;
	}

	*bytesWritten = bytesWrittenDword;

	return true;
}

/** @} */

