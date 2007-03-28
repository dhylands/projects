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
*   @file   StateMegaLoad.h
*
*   @brief  Implements the state machine for dealing with the MegaLoad
*           bootloader.
*
****************************************************************************/

#if !defined( STATESTK500_H )
#define STATESTK500_H

// ---- Include Files -------------------------------------------------------

#if !defined( STATE_H )
#   include "State.h"
#endif

// ---- Constants and Types -------------------------------------------------

// ---- Variable Externs ----------------------------------------------------

// ---- Function Prototypes -------------------------------------------------

StatePtrProxy StartSTK500( void );

#endif  // STATESTK500_H

