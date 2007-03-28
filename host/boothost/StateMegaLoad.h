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

#if !defined( STATEMEGALOAD_H )
#define STATEMEGALOAD_H

// ---- Include Files -------------------------------------------------------

#if !defined( STATE_H )
#   include "State.h"
#endif

// ---- Constants and Types -------------------------------------------------

// ---- Variable Externs ----------------------------------------------------

// ---- Function Prototypes -------------------------------------------------

StatePtrProxy   StateMegaDeviceId( char ch );
StatePtrProxy   StateMegaDeviceInfoAck( char ch );
StatePtrProxy   StateMegaFlashSize( char ch );
StatePtrProxy   StateMegaBootSize( char ch );
StatePtrProxy   StateMegaPageSize( char ch );
StatePtrProxy   StateMegaAckNack( char ch );

#endif  // STATEMEGALOAD_H

