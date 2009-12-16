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
*   @file   SerialComm.h
*
*   @brief  Implements the abstract communications device using a serial port.
*
****************************************************************************/
/**
*  @defgroup    SerialComm  Serial Communications Device
*  @ingroup     CommDev
*
*  @brief   Implements the abstract communications device using a serial port.
*/

#if !defined( SERIALCOMM_H )
#define SERIALCOMM_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include "CommDev.h"

#if !defined( _WINDOWS_ )
#   include <windows.h>
#endif

/**
 * @addtogroup SerialComm
 * @{
 */

//---------------------------------------------------------------------------
/**
*  The SerialComm class implements the serial communications device.
*/

class SerialComm : public CommDev
{
public:

    //------------------------------------------------------------------------
    // Default constructor
    
    SerialComm();
    
    //------------------------------------------------------------------------
    // Destructor
    
    virtual ~SerialComm();
    
    //------------------------------------------------------------------------
    // Aborts a read, if one is pending.
    
    virtual void AbortRead();
    
    //------------------------------------------------------------------------
    // Close a previsouly opened device.
    
    virtual void Close();
    
    //------------------------------------------------------------------------
    // Returns connection information (for display purposes)

    virtual const char *ConnectionInfo() const;

    //------------------------------------------------------------------------
    // Open a communication device. This provides a generic interface,
    // but a derived class may choose to provide a more specialized version.
    
    virtual bool Open( const char *openStr );
    
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
    
    SerialComm( const SerialComm & copy );
    SerialComm &operator =( const SerialComm &rhs );
    
    //------------------------------------------------------------------------

    HANDLE      m_serialHndl;       ///< Handle to open serial port
    OVERLAPPED  m_readOverlapped;   ///< Overlapped data for reading.
    OVERLAPPED  m_writeOverlapped;  ///< Overlapped data for writing.
    HANDLE      m_abortEvent;       ///< Set to get the read to break

    char        m_connectionInfo[ 20 ]; ///< Describes speed etc.
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // SERIALCOMM_H

