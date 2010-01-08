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
*   @file   Bioloid.h
*
*   @brief  Contains some constants which are used by various bioloid classes.
*
****************************************************************************/

#if !defined( BIOLOID_H )
#define BIOLOID_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

namespace Bioloid
{
    typedef uint8_t ID_t;

    enum
    {
        BROADCAST_ID    = 0xFE,
        INVALID_ID      = 0xFF,
    };

    enum Command
    {
        CMD_PING        = 0x01, // Used to obatin a status packet
        CMD_READ        = 0x02, // Read values from the control table
        CMD_WRITE       = 0x03, // Write values to control table
        CMD_REG_WRITE   = 0x04, // Prime values to write when ACTION sent
        CMD_ACTION      = 0x05, // Triggers REG_WRITE
        CMD_RESET       = 0x06, // Changes control values back to factory defaults

        CMD_SYNC_WRITE  = 0x83  // Writes values to many devices
    };

    enum Error
    {
        ERROR_RESERVED      = 0x80, ///< Reserved - set to zero
        ERROR_INSTRUCTION   = 0x40, ///< Undefined instruction
        ERROR_OVERLOAD      = 0x20, ///< Max torque can't control applied load
        ERROR_CHECKSUM      = 0x10, ///< Checksum of instruction packet incorrect
        ERROR_RANGE         = 0x08, ///< Instruction is out of range
        ERROR_OVERHEATING   = 0x04, ///< Internal temperature is too high
        ERROR_ANGLE_LIMIT   = 0x02, ///< Goal position is outside of limit range
        ERROR_INPUT_VOLTAGE = 0x01, ///< Input voltage out of range
        ERROR_NONE          = 0x00, ///< No Error

        ERROR_NOT_DONE      = 0x100,///< Special error code used by BioloidPacket::ProcessChar
        ERROR_TIMEOUT       = 0x101,///< Indicates that a timeout occurred whilw waiting for a reply
        ERROR_TOO_MUCH_DATA = 0x102,///< Packet storage isn't big enough
    };
};

#endif  // BIOLOID_H
