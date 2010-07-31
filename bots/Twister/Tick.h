/****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
****************************************************************************/
/**
*
*   @file   Tick.h
*
*   @brief  Definitions for the system tick.
*
****************************************************************************/

#if !defined( TICK_H )
#define TICK_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

//---------------------------------------------------------------------------

#define TICK_HZ  512

typedef int tick_t;

extern int gTickFlag;
extern int gTickOverrun;
extern volatile tick_t gTickCounter;

static inline tick_t Milliseconds2Ticks( unsigned mSec )
{
    return mSec * TICK_HZ / 1000;
}

#endif // TICK_H

