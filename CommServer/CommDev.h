/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*	Permission is granted to any individual or institution to use, copy, or
*	redistribute this software so long as it is not sold for profit, and that
*	this copyright notice is retained.
*
****************************************************************************/
/**
*
*  @file    CommDev.h
*
*  @brief   Abstract interface for a communications device.
*
****************************************************************************/
/**
*  @defgroup   CommDev   Communications Device
*
*  @brief   Provides an abstract interface for a communications device.
*/

#if !defined( COMMDEV_H )
#define COMMDEV_H               ///< Include Guard

// ---- Include Files -------------------------------------------------------

#if !defined( _WINDOWS_ )
#   include <windows.h>
#endif

#if !defined( _STRING_ )
#	include <string>
#endif

#if !defined( _INC_STDARG )
#   include <stdarg.h>
#endif

/**
 * @addtogroup CommDev
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The CommDev class provides an abstract interface for a communications device.
*/

class CommDev
{
public:

    //------------------------------------------------------------------------
    // Default constructor
    
    CommDev();
    
    //------------------------------------------------------------------------
    // Destructor
    
    virtual ~CommDev();
    
    //------------------------------------------------------------------------
    /**
    * Aborts a read, if one is pending.
    */
    
    virtual void AbortRead() = 0;
    
    //------------------------------------------------------------------------
    /**
    * Close a previsouly opened device.
    */
    
    virtual void Close() = 0;
    
    //------------------------------------------------------------------------
    // Returns connection information (for display purposes)

    virtual const char *ConnectionInfo() const;

    //------------------------------------------------------------------------
    // Enable line input/editing mode, if supported by the device.

    virtual void EnableLineMode( bool enable = true );

    //------------------------------------------------------------------------
    // Returns true if the device is in an EOF condition. Not all devices 
    // are capable of detecting this condition.

    virtual bool Eof() const;

    //------------------------------------------------------------------------
    // Return the name of this device (only valid when open)

    virtual const char *Name() const;

    //------------------------------------------------------------------------
    /**
    * Open a communication device. This provides a generic interface,
    * but a derived class may choose to provide a more specialized version.
    */
    
    virtual bool Open
    (
        const char *openStr ///< (in) Parameters to determine device to open.
    ) = 0;
    
    //------------------------------------------------------------------------
    // Returns true if the device has detected a quit indication. This is
    // typically only implemented by a console device.

    virtual bool QuitDetected() const;

    //------------------------------------------------------------------------
    // Called to print a string to the communication device

    virtual void Printf( const char *fmt, ... );
    virtual void vPrintf( const char *fmt, va_list args );

    //------------------------------------------------------------------------
    /**
    *
    * Read data from the communication device.
    *
    * @returns    The number of bytes actually read.
    */
    
    virtual bool Read
    (
        void   *buf,        ///< (out) Place to store the read data.
        size_t  bufSize,    ///< (in)  Size of the data buffer.
        size_t *bytesRead   ///< (out) Number of bytes written.
    ) = 0;

    //------------------------------------------------------------------------
    /**
    * Write data to the communication device.
    */

    virtual bool Write
    (
        const void *buf,            ///< (in) Buffer containing data to write
        size_t      bufSize,        ///< (in) Number of bytes of data to write
        size_t     *bytesWritten    ///< (out) Number of bytes written.
    ) = 0;

protected:

    //------------------------------------------------------------------------

    std::string m_name; ///< Name of this device

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.
    
    CommDev( const CommDev & copy );
    CommDev &operator =( const CommDev &rhs );
    
    //------------------------------------------------------------------------
};

// ---- Inline Functions ----------------------------------------------------

#endif // COMMDEV_H

