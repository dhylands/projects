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
 *  @file   BioloidControlTable.cpp
 *
 *  @brief  Base class for implementing a bioloid control table.
 *
 *  @details   This class takes care of managing the control table. Each
 *             time a control table entry is changed, the ControlTableModified
 *  virtual function is called.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "BioloidControlTable.h"

#include "Config.h"

#include "Bioloid.h"
#include "Delay.h"
#include "Log.h"

#include <memory.h>

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

namespace Bioloid {

//! Constructor.
ControlTable::ControlTable(
    size_t values_size_,  //!< Size of the final values struct (in bytes)
    Values* values_       //!< Pointer to struct containing control table values
    )
    : values_size{values_size_}, values{values_} {}

//! @brief Initializes the control table for use.
void ControlTable::init() {
    this->read_eeprom_table();
}

#if 0
// virtual
void ControlTable::ReadEEPROMTable() {
#if CFG_EMULATE_EEPROM
    char filename[32];
    snprintf(filename, sizeof(filename), "eeprom-%03u", Bioloid::as_uint8_t(this->m_id));
    FILE* fs;
    uint8_t buf[256];
    memset(buf, 0, sizeof(buf));
    if ((fs = fopen(filename, "rb")) != NULL) {
        size_t bytesRead = fread(buf, 1, sizeof(buf), fs);
        fclose(fs);
        if (bytesRead == this->m_numEEPROMEntries) {
            // Everything looks good - use the file contents for our EEPROM values.
            memcpy(this->m_controlTable, buf, this->m_numEEPROMEntries);
        } else {
            // The size doesn't match - can't rely on the contents.
            this->ResetEEPROMValue();
        }
    } else {
        // No EEPROM file - reset to defaults.
        this->ResetEEPROMValue();
    }
#else
#error Not implemented yet
#endif
}

// virtual
void ControlTable::ResetEEPROMValue() {
#if CFG_EMULATE_EEPROM
    memset(this->m_controlTable, 0, this->m_numEEPROMEntries);
    this->m_controlTable[as_uint8_t(Control::ID)] = Bioloid::as_uint8_t(ID::DEFAULT);
    this->m_controlTable[as_uint8_t(Control::BAUD_RATE)] = 1;            // 1 Mbit/s
    this->m_controlTable[as_uint8_t(Control::RETURN_DELAY_TIME)] = 250;  // 500 uSec
    this->m_controlTable[as_uint8_t(Control::STATUS_RETURN_LEVEL)] =
        as_uint8_t(StatusReturnLevel::ALL);
#else
#error Not implemented yet
#endif
}

// virtual
void ControlTable::WriteEEPROMTable() {
#if CFG_EMULATE_EEPROM
    char filename[32];
    snprintf(filename, sizeof(filename), "eeprom-%03d", Bioloid::as_uint8_t(this->m_id));
    FILE* fs;
    uint8_t buf[256];
    memset(buf, 0, sizeof(buf));
    if ((fs = fopen(filename, "wb")) != NULL) {
        fwrite(this->m_controlTable, 1, this->m_numEEPROMEntries, fs);
        fclose(fs);
    }
#else
#error Not implemented yet
#endif
}
#endif

}  // namespace Bioloid
