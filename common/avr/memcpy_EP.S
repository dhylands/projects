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
*   @file   memcpy_EP.S
*
*   @brief  Implements a variant of memcpy_P which utilizes the ELPM
*           instruction. This allows the entire 128K of flash to be read
*           on the ATMega128
*
*****************************************************************************/

#include <avr/io.h>

// extern void *memcpy_EP( void *dst, uint32_t src, size_t len );

#define dst_hi  r25
#define dst_lo  r24
#define src_hi  r23     ; Not actually used (s/b always zero)
#define src_3   r22
#define src_2   r21
#define src_lo  r20
#define len_hi  r19                   
#define len_lo  r18

        .text
        .global memcpy_EP
        .type   memcpy_EP, @function

memcpy_EP:
        out     _SFR_IO_ADDR(RAMPZ), src_3  
        movw    r30, src_lo     ; Z
        movw    r26, dst_lo     ; X   
        rjmp    .memcpy_EP_start
        
.memcpy_EP_loop:
        elpm    r0, Z+
	    st	    X+, r0
        
.memcpy_EP_start:
	    subi	len_lo, lo8(1)
	    sbci	len_hi, hi8(1)

	    brcc	.memcpy_EP_loop
        
; return dest (unchanged)
	    ret
        
.memcpy_P_end:
	    .size   memcpy_EP, .memcpy_EP_end - memcpy_EP
        
