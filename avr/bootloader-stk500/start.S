#include <avr/io.h>

#if __AVR_MEGA__
  #define XJMP jmp
  #define XCALL call
#else
  #define XJMP rjmp
  #define XCALL rcall
#endif

    .text
    .global start
    .global boot_0
    .global boot_1
    .global boot

start:
    XJMP	boot_0  ; Hardware reset entry point
    XJMP	boot_1  ; Entry point if called by application for direct com with stk500

#define __zero_reg__    r1

    .global __stack
    .set    __stack, RAMEND

// Entry point from a hardware reset

boot_0:
    clr	    __zero_reg__
    out	    _SFR_IO_ADDR(SREG), __zero_reg__
    ldi	    r28,lo8(__stack)
    ldi	    r29,hi8(__stack)
    out	    _SFR_IO_ADDR(SPH), r29
    out	    _SFR_IO_ADDR(SPL), r28

    ldi     r24,lo8(0)
    ldi     r25,hi8(0)

    XJMP    boot

// Entry pint if called directly by an application

boot_1:
    ldi	    R24, 1
    XJMP    boot

