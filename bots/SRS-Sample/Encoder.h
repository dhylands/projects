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
*   @file    Encoder.h
*
*   @brief   Performs quadrature decoding.
*
*****************************************************************************/

#if !defined( ENCODER_H )
#define ENCODER_H               /**< Include Guard                          */

/* ---- Include Files ----------------------------------------------------- */

#include <inttypes.h>
#include "Hardware.h"
#include "Log.h"

/* ---- Constants and Types ----------------------------------------------- */

typedef uint16_t    encoder_t;

typedef struct
{
    int8_t  increment[ 4 ];

} EncoderState_t;

/* ---- Variable Externs -------------------------------------------------- */

extern  volatile    encoder_t   gEncoderCountR;
extern  volatile    encoder_t   gEncoderCountL;

extern  EncoderState_t gEncoderIncr[];

extern  uint8_t gEncoderStateL;
extern  uint8_t gEncoderStateR;

/* ---- Function Prototypes ----------------------------------------------- */

//***************************************************************************
/**
*   Does the encoder polling to update the wheel encoder. For the SRS bot
*   this function is called each millisecond. 
*/

static inline void PollEncoder( void )
{
    uint8_t stateL = ((( ENCODER_L_A_PIN & ENCODER_L_A_MASK ) >> ENCODER_L_A_BIT ) << 1 )
                   | ((( ENCODER_L_B_PIN & ENCODER_L_B_MASK ) >> ENCODER_L_B_BIT ));

    uint8_t stateR = ((( ENCODER_R_A_PIN & ENCODER_R_A_MASK ) >> ENCODER_R_A_BIT ) << 1 )
                   | ((( ENCODER_R_B_PIN & ENCODER_R_B_MASK ) >> ENCODER_R_B_BIT ));

    gEncoderCountL += gEncoderIncr[ gEncoderStateL ].increment[ stateL ];
    gEncoderCountR -= gEncoderIncr[ gEncoderStateR ].increment[ stateR ];

    gEncoderStateL = stateL;
    gEncoderStateR = stateR;

} // Poll Encoder

#endif  // ENCODER_H

