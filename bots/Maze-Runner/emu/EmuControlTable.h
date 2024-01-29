
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
 *   @file   EmuControlTableh
 *
 *   @brief  Class for implementing an emulated Control Table, which is
 *           persisted as a file on disk..
 *
 *****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <stddef.h>
#include <stdint.h>

#include "BioloidControlTable.h"

namespace Emu {

class ControlTable : public Bioloid::ControlTable {
 public:
    ControlTable(Bioloid::ID id, uint8_t numControlTableEntries, uint8_t numEEPROMEntries);
    virtual ~ControlTable();

    virtual void Initialize();

 protected:
    virtual void ReadEEPROMTable();
    virtual void ResetEEPROMValue();
    virtual void WriteEEPROMTable();

 private:
};

}  // namespace Emu
