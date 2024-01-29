/****************************************************************************
 *
 *   Copyright (c) 2022 Dave Hylands     <dhylands@gmail.com>
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
 *   @file   BioloidControlTable.h
 *
 *   @brief  Base class for implementing a bioloid control table, which holds
 *           the persisted and non-persisted register values.
 *
 *****************************************************************************/

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "BioloidPacket.h"

// ---- Include Files -------------------------------------------------------

namespace Bioloid {

using BaudRateByte = uint8_t;  //!< Type for baud rate stored in the Control Table
using BaudRate = uint32_t;     //!< Type for baud rate (in bits per second)

//! @brief Returns the baud rate in the single byte value stored in the control table.
static constexpr BaudRateByte baud_rate_byte(BaudRate baud  //!< [in] The baud rate (in bits/second)
) {
    return (2'000'000 / baud) - 1;
}

//! @brief Returns the baud rate represented by the baud rate byte from the control table.
static constexpr BaudRate baud_rate(
    BaudRateByte brb  //!< [in] The baud rate byte from the control table.
) {
    return 2'000'000 / (brb + 1);
}

using ReturnDelayByte = uint8_t;    //!< Type for return delay time stored in the control table.
using ReturnDelayUSecs = uint16_t;  //!< Type for return delay time (in microseconds)

static constexpr ReturnDelayByte return_delay_byte(
    ReturnDelayUSecs usecs  //!< [in] Time to delay response (in microseconds)
) {
    return usecs / 2;
}

static constexpr ReturnDelayUSecs return_delay_usecs(
    ReturnDelayByte rdb  //!< [in] Time to delay response represented as a byte.
) {
    return rdb * 2;
}

//! Class which encapsulates the Control Table
class ControlTable {
 public:
    //! Describes the status_return_level field.
    enum class StatusReturnLevel : uint8_t {
        NONE = 0,       //!< Don't respond.
        READ_DATA = 1,  //!< Respond only to READ_DATA.
        ALL = 2,        //!< Respond to everything.
    };

    //! The default baud rate used by a device.
    static constexpr BaudRate DEFAULT_BAUD_RATE = 1'000'000;

    //! The default baud rate encoded into a byte,
    static constexpr BaudRateByte DEFAULT_BAUD_RATE_BYTE = baud_rate_byte(DEFAULT_BAUD_RATE);

    //! The default return delay in microseconds.
    static constexpr ReturnDelayUSecs DEFAULT_RETURN_DELAY_USECS = 500;

    //! The default return delay encoded in a byte.
    static constexpr ReturnDelayByte DEFAULT_RETURN_DELAY_BYTE =
        return_delay_byte(DEFAULT_RETURN_DELAY_USECS);

    //! Common control table fields
    struct Values {
        uint16_t model = 0;                          //!< Model number of this device.
        uint8_t firmware_version = 0;                //!< Firmware version.
        uint8_t id = 1;                              //!< ID of this device.
        uint8_t baud_rate = DEFAULT_BAUD_RATE_BYTE;  //!< Baud Rate to use.
        ReturnDelayByte return_delay_time =
            DEFAULT_RETURN_DELAY_BYTE;          //!< Time to wait before sending response.
        uint8_t _reserved_1[10];                //!< Reserved area 1
        StatusReturnLevel status_return_level;  //!< Response we should return.
        uint8_t _reserved_2[8];                 //!< Reserved area 2
        uint8_t led;                            //!< 0=off non-zero=on
    };

    ControlTable(size_t values_size, Values* values);
    virtual ~ControlTable() = default;

    virtual void init();

 protected:
    //! Reads the EEPROM table into RAM.
    virtual void read_eeprom_table() = 0;

    //! Writes the RAM values to EEPROM.
    virtual void write_eeprom_table() = 0;

    //! Resets the EEPROM table to default values.
    virtual void reset_eeprom_values() = 0;

 private:
    size_t const values_size;  //!< Size of final values struct (in bytes)
    Values* const values;      //!< Pointer to memory allocated for values.
};

}  // namespace Bioloid
