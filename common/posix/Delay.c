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

#include "Delay.h"

//***************************************************************************
/**
 *   Spin for ms milliseconds
 */

void ms_spin(unsigned short ms) {
    if (!ms) {
        return;
    }

    usleep(ms * 1000);
}

//***************************************************************************
/**
 *   Spin for us microseconds
 */

void us_spin(unsigned short us) {
    if (!us) {
        return;
    }

    usleep(us);
}

/***************************************************************************/
/**
 *  Delays for a multiple of 100 microseconds.
 */

void Delay100uSec(uint8_t num100uSec /**< Number of 100 microsecond units to delay       */
) {
    while (num100uSec > 0) {
        us_spin(100);
        num100uSec--;
    }
}

/***************************************************************************/
/**
 *  Delays for a multiple of 10 microseconds.
 */

void Delay10uSec(uint8_t num10uSec /**< Number of 10 microsecond units to delay         */
) {
    while (num10uSec > 0) {
        us_spin(10);
        num10uSec--;
    }
}

/***************************************************************************/
/**
 *  Delays for a multiple of 100 milliseconds.
 */

void Delay100mSec(uint8_t num100mSec /**< Number of 100 microsecond units to delay       */
) {
    while (num100mSec > 0) {
        ms_spin(100);
        num100mSec--;
    }
}

/***************************************************************************/
/**
 *  Delays for a multiple of 10 milliseconds.
 */

void Delay10mSec(uint8_t num10mSec /**< Number of 10 microsecond units to delay         */
) {
    while (num10mSec > 0) {
        ms_spin(10);
        num10mSec--;
    }
}

/***************************************************************************/
/**
 *  Delays for a multiple of 1 milliseconds.
 */

void Delay1mSec(uint8_t num1mSec /**< Number of 10 microsecond units to delay         */
) {
    while (num1mSec > 0) {
        ms_spin(1);
        num1mSec--;
    }
}