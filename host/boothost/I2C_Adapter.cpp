/****************************************************************************
*
*   Copyright (c) 2005 - 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   I2C_Adapter.cpp
*
*   @brief  This file contains common code shared amongst all I2C_Adapters.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "I2C_Adapter.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup i2c
 * @{
 */

//***************************************************************************
/**
*   Constructor
*/

I2C_Adapter::I2C_Adapter()
    : m_callback( NULL )
{
    // Nothing to do
}

//***************************************************************************
/**
*   Destructor
*
*   virtual
*/

I2C_Adapter::~I2C_Adapter()
{
    // Nnothing to do
}

/** @} */

