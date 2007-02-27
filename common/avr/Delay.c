/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
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
*   @file    Delay.c
*
*   @brief   Routines for waiting various amounts of time.
*
*****************************************************************************/

#include "Config.h"
#include "Delay.h"

//***************************************************************************
/**
*   Spin for ms milliseconds
*/

#define LOOPS_PER_MS ( CFG_CPU_CLOCK / 1000 / 4)

void ms_spin( unsigned short ms )
{
   if (!ms)
           return;

   /* the inner loop takes 4 cycles per iteration */
   __asm__ __volatile__ (
           "1:                     \n"
           "       ldi r26, %3     \n"
           "       ldi r27, %2     \n"
           "2:     sbiw r26, 1     \n"
           "       brne 2b         \n"
           "       sbiw %1, 1      \n"
           "       brne 1b         \n"
           : "+w" (ms)
           : "w" (ms), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
           : "r26", "r27"
           );

} // ms_spin

//***************************************************************************
/**
*   Spin for us microseconds
*/

#define LOOPS_PER_US (CFG_CPU_CLOCK / 1000000 / 4)

void us_spin(unsigned short us)
{
   if (!us)
          return;
   
   /* the inner loop takes 4 cycles per iteration */
   __asm__ __volatile__ (
          "1:                     \n"
          "       ldi r26, %3     \n"
          "       ldi r27, %2     \n"
          "2:     sbiw r26, 1     \n"
          "       brne 2b         \n"
          "       sbiw %0, 1      \n"
          "       brne 1b         \n"
          : "+w" (us)
          : "w" (us), "i" (LOOPS_PER_US >> 8), "i" (0xff & LOOPS_PER_US)
          : "r26", "r27"
          );

} // us_spin

/***************************************************************************/
/**
*  Delays for a multiple of 100 microseconds.
*/

void Delay100uSec
(
   uint8_t num100uSec   /**< Number of 100 microsecond units to delay       */
)
{
    while ( num100uSec > 0 )
    {
        us_spin( 100 );
        num100uSec--;
    }

} // Delay100uSec

/***************************************************************************/
/**
*  Delays for a multiple of 10 microseconds.
*/

void Delay10uSec
(
   uint8_t num10uSec    /**< Number of 10 microsecond units to delay         */
)
{
    while ( num10uSec > 0 )
    {
        us_spin( 10 );
        num10uSec--;
    }

} // Delay10uSec

/***************************************************************************/
/**
*  Delays for a multiple of 100 milliseconds.
*/

void Delay100mSec
(
   uint8_t num100mSec   /**< Number of 100 microsecond units to delay       */
)
{
    while ( num100mSec > 0 )
    {
        ms_spin( 100 );
        num100mSec--;
    }

} // Delay100mSec

/***************************************************************************/
/**
*  Delays for a multiple of 10 milliseconds.
*/

void Delay10mSec
(
   uint8_t num10mSec    /**< Number of 10 microsecond units to delay         */
)
{
    while ( num10mSec > 0 )
    {
        ms_spin( 10 );
        num10mSec--;
    }

} // Delay10mSec

/***************************************************************************/
/**
*  Delays for a multiple of 1 milliseconds.
*/

void Delay1mSec
(
   uint8_t num1mSec     /**< Number of 10 microsecond units to delay         */
)
{
    while ( num1mSec > 0 )
    {
        ms_spin( 1 );
        num1mSec--;
    }

} // Delay1mSec

