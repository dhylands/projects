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
*   @file   ConsoleComm.h
*
*   @brief  Implements the abstract communications device using a serial port.
*
****************************************************************************/
/**
*  @defgroup    ConsoleComm  Serial Communications Device
*  @ingroup     CommDev
*
*  @brief   Implements the abstract communications device using a serial port.
*/

#if !defined( CONSOLECOMM_H )
#define CONSOLECOMM_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include "CommDev.h"

#if !defined( _WINDOWS_ )
#   define WIN32_LEAN_AND_MEAN  ///< Trim it down as much as we can
#   include <windows.h>
#endif

/**
 * @addtogroup ConsoleComm
 * @{
 */

//---------------------------------------------------------------------------
/**
*  The ConsoleComm class implements the serial communications device.
*/

class ConsoleComm : public CommDev
{
public:

    //------------------------------------------------------------------------
    // Default constructor
    
    ConsoleComm();
    
    //------------------------------------------------------------------------
    // Destructor
    
    virtual ~ConsoleComm();
    
    //------------------------------------------------------------------------
    // Aborts a read, if one is pending.
    
    virtual void AbortRead();
    
    //------------------------------------------------------------------------
    // Close a previsouly opened device.
    
    virtual void Close();
    
    //------------------------------------------------------------------------
    // Enable line input/editing mode, if supported by the device.

    virtual void EnableLineMode( bool enable = true );

    //------------------------------------------------------------------------
    // Open a communication device. This provides a generic interface,
    // but a derived class may choose to provide a more specialized version.
    
    virtual bool Open( const char *openStr );
    
    //------------------------------------------------------------------------
    // Returns true if the device has detected a quit indication. This is
    // typically only implemented by a console device.

    virtual bool QuitDetected() const;

    //------------------------------------------------------------------------
    // Read data from the communication device.
    
    virtual bool Read( void *buf, size_t bufSize, size_t *bytesRead );

    //------------------------------------------------------------------------
    // Write data to the communication device.

    virtual bool Write( const void *buf, size_t bufSize, size_t *bytesWritten );

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.
    
    ConsoleComm( const ConsoleComm & copy );
    ConsoleComm &operator =( const ConsoleComm &rhs );
    
    //------------------------------------------------------------------------

    HANDLE          m_stdin;
    HANDLE          m_stdout;
    INPUT_RECORD    m_inputRec;
    HANDLE          m_abortEvent;
    bool            m_quitDetected;
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // CONSOLECOMM_H

