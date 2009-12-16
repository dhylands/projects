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
*   @file   CommDev.cpp
*
*   @brief  Implements the abstract communications device class.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "CommDev.h"
#include "Str.h"

/**
 * @addtogroup CommDev
 * @{
 */

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
*   Default Constructor.
*/

CommDev::CommDev()
{
}

//***************************************************************************
/**
*   Destructor.
*
* virtual
*/

CommDev::~CommDev()
{
}

//***************************************************************************
/**
*   Returns connection information (for display purposes)
*
* virtual
*/

const char *CommDev::ConnectionInfo() const
{
    return "";
}

//***************************************************************************
/**
*   Enable line input/editing mode, if supported by the device.
*
* virtual
*/

void CommDev::EnableLineMode( bool enable )
{
    // Nothing to do

} // EnableLineMode

//***************************************************************************
/**
*   Returns true if the device is in an EOF condition. Not all devices 
*   are capable of detecting this condition.
*
* virtual
*/

bool CommDev::Eof() const
{
   return false;

} // Eof

//***************************************************************************
/**
*   Return the name of this device (only valid when open)
*
*   Derived classes can override this, if desired.
*
*   @return A pointer to a string containing the name of this device.
*
* virtual
*/

const char *CommDev::Name() const
{
    return m_name.c_str();

} // Name

//***************************************************************************
/**
*   Prints a string to the communication device.
*
* virtual
*/
    
void CommDev::Printf( const char *fmt, ... )
{
    va_list args;

    va_start( args, fmt );
    vPrintf( fmt, args );
    va_end( args );
    
} // Printf

void CommDev::vPrintf( const char *fmt, va_list args )
{
    char    buf[ 512 ];

    vStrPrintf( buf, sizeof( buf ), fmt, args );

    size_t  bytesWritten;

    Write( buf, strlen( buf ), &bytesWritten );

} // vPrintf

//***************************************************************************
/**
*   Returns true if the device has detected a quit indication. This is
*   typically only implemented by a console device.
*
* virtual
*/
    
bool CommDev::QuitDetected() const
{
    return false;

} // QuitDetected


