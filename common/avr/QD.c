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
*   @file   FILE_Data.c
*
*   @brief  This file provides the implementation for reading hex/srecord files.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "QD.h"
#include "Config.h"

// ---- Public Variables ----------------------------------------------------

// Quadrature decoding using a A and B channel. The sequence goes like this:
//
//  00 01 11 10 in one direction
//  00 10 11 01 in the other direction
//
// The left column contains the previous state, and the values across indicate
// the new state. The value in the table is added to the counter when 
// transitioning from the previous state to the new state.

#if ( CFG_QD_COUNT_A_ONLY )

// This increment table uses two channels, but only incrememts and decrements
// on the A channel transitions.
//

QD_Increment_t QD_gIncrement[ 4 ] =
{
    /*          00  01  10  11 */
    /*          --  --  --  -- */
    /* 00 */ {{  0,  0, -1,  0 }},
    /* 01 */ {{  0,  0,  0,  1 }},
    /* 10 */ {{  1,  0,  0,  0 }},
    /* 11 */ {{  0, -1,  0,  0 }}
};

#elif ( CFG_QD_COUNT_B_ONLY )

// This increment table uses two channels, but only incrememts and decrements
// on the B channel transitions.
//

QD_Increment_t QD_gIncrement[ 4 ] =
{
    /*          00  01  10  11 */
    /*          --  --  --  -- */
    /* 00 */ {{  0,  1,  0,  0 }},
    /* 01 */ {{ -1,  0,  0,  0 }},
    /* 10 */ {{  0,  0,  0, -1 }},
    /* 11 */ {{  0,  0,  1,  0 }}
};

#else

// Array of increments.

QD_Increment_t QD_gIncrement[ 4 ] =
{
    /*          00  01  10  11 */
    /*          --  --  --  -- */
    /* 00 */ {{  0,  1, -1,  0 }},
    /* 01 */ {{ -1,  0,  0,  1 }},
    /* 10 */ {{  1,  0,  0, -1 }},
    /* 11 */ {{  0, -1,  1,  0 }}
};

#endif

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------


