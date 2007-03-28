/****************************************************************************
*
*     Copyright (c) 2005 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
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

