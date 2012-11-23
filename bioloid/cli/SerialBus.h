/****************************************************************************
*
*   Copyright (c) 2009 Dave Hylands     <dhylands@gmail.com>
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
*   @file   SerialBus.h
*
*   @brief  Implements a bioloid bus using posix serial. This typically
*           assumes that an FTDI USB-to-serial adapter is being used
*   to do the RS-485 management.
*
****************************************************************************/

#if !defined( SERIALBUS_H )
#define SERIALBUS_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include "BioloidBus.h"
#include "SerialPort.h"

/**
 * @addtogroup bioloid
 * @{
 */

class SerialBus : public BioloidBus
{
public:
    //------------------------------------------------------------------------
    // Default constructor

    SerialBus();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~SerialBus();

    //------------------------------------------------------------------------
    // Sets the serial port which will be used for communications

    void SetSerialPort( SerialPort *serPort );

    //------------------------------------------------------------------------
    // Reads a byte.

    virtual bool ReadByte( uint8_t *ch );

protected:

    //------------------------------------------------------------------------
    // Core portion of the WriteBuffer routine, which writes the data
    // out the serial port.

    virtual void WriteBufferedData( void *data, size_t numBytes );

private:

    //------------------------------------------------------------------------

    SerialPort *m_serialPort;

    int         m_fd;
};

/** @} */

#endif /* SERIALBUS_H */

