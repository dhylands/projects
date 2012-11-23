/****************************************************************************
*
*   Copyright (c) 2005 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
/**
*
*   @file    Encoder.c
*
*   @brief   Performs quadrature decoding.
*
*****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <inttypes.h>

#include "Encoder.h"
#include "Hardware.h"

// ---- Public Variables ----------------------------------------------------

volatile encoder_t   gEncoderCountR;
volatile encoder_t   gEncoderCountL;

// ---- Private Constants and Types -----------------------------------------

// Quadrature decoding using a A and B channel. The sequence goes like this:
//
//  00 01 11 10 in one direction
//  00 10 11 01 in the other direction

// Array of increments. 

EncoderState_t gEncoderIncr[ 4 ] =
{
    /*          00  01  10  11 */
    /*          --  --  --  -- */
    /* 00 */ {{  0,  1, -1,  0 }},
    /* 01 */ {{ -1,  0,  0,  1 }},
    /* 10 */ {{  1,  0,  0, -1 }},
    /* 11 */ {{  0, -1,  1,  0 }}
};

// ---- Private Variables ---------------------------------------------------

uint8_t gEncoderStateL;
uint8_t gEncoderStateR;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------


