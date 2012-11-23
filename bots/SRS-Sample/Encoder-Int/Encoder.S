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
*   @file    Encoder.S 
*
*   @brief   Performs quadrature decoding.
*
*   This is taken from Larry Barello's site at this URL:
*   http://www.barello.net/Papers/Motion_Control/index.htm
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#define __SFR_OFFSET 0

#include <avr/io.h>

#define Right_Enc_B     PINC, 5
#define Left_Enc_B      PINC, 2

.comm   gEncoderCountR,1,1
.comm   gEncoderCountL,1,1

/* ---- Functions --------------------------------------------------------- */

/****************************************************************************
*
* SIG_INTERRUPT0
*
* Fields the A line for an encoder. Increments/Decrements a
* register counter per the state of the B line. Flips sense of 
* external interrupt in preparation for the next state transition.
*
* Because we dont interrupt on the B line, this only does 
* 1/2 quadurature decoding.
*
* ~40 cycles/interrupt (including call and return)
*
****************************************************************************/

    .func SIG_INTERRUPT0
    .global SIG_INTERRUPT0
SIG_INTERRUPT0:
    push R31
    push R30
    push R29
    in R31, SREG
    in R30, MCUCR
    lds R29, gEncoderCountR

    sbrc R30, ISC00 ; Was this an up or down event?
    rjmp INT0_up

    subi r29, lo8(-1)
    sbis Right_Enc_B
    subi R29, 2

    sbr R30, 1<<ISC00 ; Switch Trigger to rising edge
    rjmp INT0_done
INT0_up:
    subi R29, 1
    sbis Right_Enc_B
    subi r29, lo8(-2)

    cbr R30, 1<<ISC00 ; Switch Trigger to falling edge
INT0_done:
    sts gEncoderCountR, R29

    out MCUCR, R30 
    out SREG, R31
    pop R29
    pop R30
    pop R31
    reti
.endfunc

/****************************************************************************
*
* SIG_INTERRUPT1
*
****************************************************************************/

    .func SIG_INTERRUPT1
    .global SIG_INTERRUPT1
SIG_INTERRUPT1:
    push R31
    push R30
    push R29
    in R31, SREG
    in R30, MCUCR
    lds R29, gEncoderCountL

    sbrc R30, ISC10 ; Was this an up or down event?
    rjmp INT1_up

    subi R29, 1
    sbis Left_Enc_B
    subi r29, lo8(-2)

    sbr R30, 1<<ISC10 ; Switch Trigger to rising edge
    rjmp INT1_done
INT1_up:
    subi r29, lo8(-1)
    sbis Left_Enc_B
    subi R29, 2

    cbr R30, 1<<ISC10 ; Switch Trigger to falling edge
INT1_done:
    sts gEncoderCountL, R29

    out MCUCR, R30 
    out SREG, R31
    pop R29
    pop R30
    pop R31
    reti
.endfunc

