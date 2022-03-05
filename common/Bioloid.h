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

#pragma once

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

namespace Bioloid {
enum class ID : uint8_t {
    DEFAULT = 0x00,
    BROADCAST = 0xFE,
    INVALID = 0xFF,
};
static inline uint8_t as_uint8_t(ID id) {
    return static_cast<uint8_t>(id);
}
static inline ID as_ID(uint8_t val) {
    return static_cast<ID>(val);
}

enum class Command : uint8_t {
    PING = 0x01,       // Used to obatin a status packet
    READ = 0x02,       // Read values from the control table
    WRITE = 0x03,      // Write values to control table
    REG_WRITE = 0x04,  // Prime values to write when ACTION sent
    ACTION = 0x05,     // Triggers REG_WRITE
    RESET = 0x06,      // Changes control values back to factory defaults

    SYNC_WRITE = 0x83  // Writes values to many devices
};

static inline uint8_t as_uint8_t(Command cmd) {
    return static_cast<uint8_t>(cmd);
}
static inline Command as_Command(uint8_t val) {
    return static_cast<Command>(val);
}

enum class Error : uint16_t {
    RESERVED = 0x80,       ///< Reserved - set to zero
    INSTRUCTION = 0x40,    ///< Undefined instruction
    OVERLOAD = 0x20,       ///< Max torque can't control applied load
    CHECKSUM = 0x10,       ///< Checksum of instruction packet incorrect
    RANGE = 0x08,          ///< Instruction is out of range
    OVERHEATING = 0x04,    ///< Internal temperature is too high
    ANGLE_LIMIT = 0x02,    ///< Goal position is outside of limit range
    INPUT_VOLTAGE = 0x01,  ///< Input voltage out of range
    NONE = 0x00,           ///< No Error

    NOT_DONE = 0x100,       ///< Special error code used by BioloidPacket::ProcessChar
    TIMEOUT = 0x101,        ///< Indicates that a timeout occurred whilw waiting for a reply
    TOO_MUCH_DATA = 0x102,  ///< Packet storage isn't big enough
};
static inline uint16_t as_uint16_t(Error err) {
    return static_cast<uint16_t>(err);
}

};  // namespace Bioloid
