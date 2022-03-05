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
 *  @file   BioloidGadget.cpp
 *
 *  @brief  Base class for implementing a bioloid gadget.
 *
 *  @details   This class takes care of managing the control table. Each
 *              time a control table entry is changed, the ControlTableModified
 *  virtual function is called.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "BioloidGadget.h"

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

Gadget::Gadget(Bioloid::ID id, uint8_t numControlTableEntries, uint8_t numEEPROMEntries)
    : m_id(id),
      m_numControlTableEntries(numControlTableEntries),
      m_numEEPROMEntries(numEEPROMEntries),
      m_controlTable(new uint8_t[numControlTableEntries]) {
    memset(this->m_controlTable, 0, this->m_numControlTableEntries);
}

// virtual
Gadget::~Gadget() {
    delete m_controlTable;
    m_controlTable = nullptr;
}

// virtual
void Gadget::InitializeControlTable() {
    this->ReadEEPROMTable();
}

// virtual
void Gadget::ReadEEPROMTable() {
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
void Gadget::ResetEEPROMValue() {
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
void Gadget::WriteEEPROMTable() {
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

void Gadget::ProcessBytes(uint8_t* buf, size_t numBytes) {
    for (size_t i = 0; i < numBytes; i++) {
        Error err = this->m_packet.ProcessChar(buf[i]);
        if (err == Error::NOT_DONE) {
            continue;
        }
        if (err != Error::NONE) {
            LogError("Error parsing a packet: %d\n", err);
            continue;
        }
        this->ProcessPacket();
    }
}

// virtual
void Gadget::ProcessPacket() {
    bool isBroadcast = this->m_packet.ID() == ID::BROADCAST;
    if (this->m_packet.ID() != this->m_id && !isBroadcast) {
        // packet isn't for us - ignore
        return;
    }

    switch (this->m_packet.Command()) {
        case Command::PING: {
            LogDebug("Rcvd PING command for ID %u\n", this->m_packet.ID());
            break;
        }
        case Command::READ: {
            break;
        }
        case Command::WRITE: {
            break;
        }
        case Command::REG_WRITE: {
            break;
        }
        case Command::ACTION: {
            break;
        }
        case Command::RESET: {
            break;
        }
        case Command::SYNC_WRITE: {
            break;
        }
        default: {
            LogError("Unrecognized command: 0x%02x\n", this->m_packet.Command());
            break;
        }
    }
}

void Gadget::SendResponse(Error errorCode, bool forceResponse) {
    if (errorCode != Error::NONE) {
        Log("ERROR Response: 0x%02x\n", errorCode);
    }

    // we don't call this routine for READ_DATA, so we don't send a response unless
    // the status return level is set to ALL... However, if forceResponse is true, then
    // we're going to return a response anyways, because that is how PING is supposed to work
    if (!forceResponse &&
        (this->m_controlTable[as_uint8_t(Control::STATUS_RETURN_LEVEL)] != as_uint8_t(StatusReturnLevel::ALL))) {
        return;
    }

    us_spin(this->m_controlTable[as_uint8_t(Control::RETURN_DELAY_TIME)] * 2);

    UART0_PutChar(0xFF);
    UART0_PutChar(0xFF);
    UART0_PutChar(gMyId);
    UART0_PutChar(0x02);  // length for blank response is always 2
    UART0_PutChar(errorCode);
    UART0_PutChar(~(gMyId + 0x02 + errorCode));  // checksum is ~(ID + LENGTH + ERROR)
}

}  // namespace Bioloid
