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
 *  @file   EmuGadget.cpp
 *
 *  @brief  Base class for implementing a bioloid gadget.
 *
 *  @details   This class takes care of managing the control table. Each
 *              time a control table entry is changed, the ControlTableModified
 *  virtual function is called.
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "EmuGadget.h"

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

void Emu::Gadget::delay_us(uint32_t usec) {}

void Emu::Gadget::SendByte(uint8_t byte) {}
