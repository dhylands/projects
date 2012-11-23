;
; RCpulse.s
;
; This file was taken from the OSMC project for the MOB board.
;
; STACK: 4
; INTERRUPT Latency: ~16 cycles
;
; Assembly code for the RC pulse measuring interrupts.  These run
; outside of AvrX and are carefully coded to use the minimum stack
; space and minimum interrupt latency.  Each can interrupt the other.
;
; The GCC coded versions are about twice as slow and use about twice 
; as much stack (v3.0).  This is an issue since each task stack needs
; to have enough room to accomodate any usage by these routines.
;

#include <avr/io.h>

;
; Offsets into the RC data structure (RCPulse.h)
;

        .global RC0
        .global RC1

#define RC_START 0	// These are all bytes in the struct.
#define RC_END	 1
#define RC_DONE  2


	.global	SIG_INTERRUPT0
	.func	SIG_INTERRUPT0
SIG_INTERRUPT0:
	push	R16
	in	R16, _SFR_IO_ADDR( SREG )       ; Stash context
	push	R16

	in	R16, _SFR_IO_ADDR( MCUCR )	; Determine rising/falling edge
	sbrs	R16, ISC00
	rjmp	1f

	cbr	R16, BV( ISC00 )                ; Rising, switch to fall
	out	_SFR_IO_ADDR( MCUCR ), R16
	in	R16, _SFR_IO_ADDR( TCNT2 )
	sts	RC0+RC_START, R16               ; Stash the start time.
	rjmp	2f
1:
	sbr	R16, BV(ISC00)	                ; Falling, switch to rising
	out	_SFR_IO_ADDR( MCUCR ), R16
	in	R16, _SFR_IO_ADDR( TCNT2 )
	sts	RC0+RC_END, R16	                ; Stash the end time
	ldi	R16, 1
	sts	RC0+RC_DONE, R16
2:
	pop	R16		                ; Restore context
	out	_SFR_IO_ADDR( SREG ), R16
	pop	R16
	reti
	.endfunc

	.global	SIG_INTERRUPT1
	.func	SIG_INTERRUPT1
SIG_INTERRUPT1:
	push	R16
	in	R16, _SFR_IO_ADDR( SREG )
	push	R16

	in	R16, _SFR_IO_ADDR( MCUCR )
	sbrs	R16, ISC10
	rjmp	1f

	cbr	R16, BV(ISC10)
	out	_SFR_IO_ADDR( MCUCR ), R16
	in	R16, _SFR_IO_ADDR( TCNT2 )
	sts	RC1+RC_START, R16
	rjmp	2f
1:
	sbr	R16, BV(ISC10)
	out	_SFR_IO_ADDR( MCUCR ), R16
	in	R16, _SFR_IO_ADDR( TCNT2 )
	sts	RC1+RC_END, R16
	ldi	R16, 1
	sts	RC1+RC_DONE, R16
2:
	pop	R16
	out	_SFR_IO_ADDR( SREG ), R16
	pop	R16
	reti
	.endfunc


