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
/**
 *
 *   @file   BioloidControlTableMeta.h
 *
 *   @brief  Class which defines metadata for the control table fields.
 *
 *****************************************************************************/

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>

namespace Bioloid {

template <typename E>
struct EnumEntry {
   static_assert(
        std::is_arithmetic_v<E> || std::is_enum_v<E>,
        "Expecting E to be aritmetic or enum");

    E value;           //!< Value of the enumeration.
    char const* name;  //!< Name of the enumeration value.
};

template <typename E>
struct EnumTable {
    uint8_t num_entries;    //!< Number of EnumEntrys in the table.
    EnumEntry<E> *entries;  //!< Pointer to atray of EnumEntrys.

    //! Finds an enum value by value.
    //! @return nullptr if the value wasn't found.
    //! @return A pointer to the enumeration name if the value was found.
    std::optional<char const*> find_by_value(E value) const {
        auto begin = &this->entries[0];
        auto end = &this->entries[this->num_entries];
        auto it = std::find_if(begin, end, [&](auto& entry) { return entry.value == value; });
        if (it == end) {
            return std::nullopt_t;
        }
        return it->name;
    }

    //! Finds an enum value by name,
    //! @return
    std::optional<E> find_by_name(char const*name) const {
        auto begin = &this->entries[0];
        auto end = &this->entries[this->num_entries];
        auto it = std::find_if(begin, end, [&](auto& entry) { return ::strcasecmp(entry.name, name) == 0; });
        if (it == end) {
            return std::nullopt_t;
        }
        return it->value;
    }
};

class ControlTableMetadata {
    //! Type of field.
    enum class Type : uint8_t {
        UINT8,    //!< Unsigned 8-bit int
        UINT16,   //!< Unsigned 16-bit int
        UINT32,   //!< Unsigned 32-bit int
        INT8,     //!< Signed 8-bit int
        INT16,    //!< Signed 16-bit int
        INT32,    //!< Signed 32-bit int
        ENUM8,    //!< Enumeration (based on 8-bit unsigned value)
        CSTRING,  //!< C-style null terminated string
    };

    char const* name;  //!< Name of the field.
    uint8_t offset;    //!< Offset of the field within the struct.

    Type type;         //!< Type of the field.
    union {
        EnumTable<uint8_t> const* enum8_table;  //!< Pointer to EnumTable
    };
};

}  // namespace Bioloid
