/****************************************************************************
 *
 *   Copyright (c) 2023 Dave Hylands     <dhylands@gmail.com>
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

#pragma once

#include <cstdint>

namespace Bioloid {

enum class StatusReturnLevel : uint8_t {
    NONE = 0,       //!< Don't respond.
    READ_DATA = 1,  //!< Respond only to READ_DATA.
    ALL = 2,        //!< Respond to everything.
};

//! Common control table fields
struct ControlTable {
    uint16_t model;                         //!< Model number of this device.
    uint8_t firmware_version;               //!< Firmware version.
    uint8_t id;                             //!< ID of this device.
    uint8_t baud_rate;                      //!< Baud Rate to use.
    uint8_t return_delay_time;              //!< Time to wait before sending response.
    uint8_t reserved_1[10];                 //!< Reserved area 1
    StatusReturnLevel status_return_level;  //!< Response we should return.
    uint8_t reserved_2[8];                  //!< Reserved area 2
    uint8_t led;                            //!< 0=off non-zero=on
};

}  // namespace Bioloid

//! Control table fields for Wobble
struct WobbleControlTable {
    Bioloid::ControlTable common;  //!< Common control table fields
    uint8_t field_1;            //!< Field 1
    uint32_t field_2;           //!< Field 2
    uint16_t field_3;           //!< Field 3
};
