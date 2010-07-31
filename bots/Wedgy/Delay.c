/****************************************************************************
*
*  Delay.c
*
*  Routines for waiting various amounts of time.
*
****************************************************************************/

#include "Config.h"
#include "Delay.h"

#include <avr/interrupt.h>
#include <avr/signal.h>

#define LOOPS_PER_US (CFG_CPU_CLOCK/4)
#define LOOPS_PER_MS (CFG_CPU_CLOCK/1000/4)

volatile uns16 gTickCount = 0;

/* spin for us microseconds */
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
                : "=w" (us)
                : "w" (us), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
                );
}

/* spin for ms milliseconds */
void ms_spin(unsigned short ms)
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
                "       sbiw %0, 1      \n"
                "       brne 1b         \n"
                : "=w" (ms)
                : "w" (ms), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
                );
}

/***************************************************************************/
/**
*  Delays for a multiple of 100 microseconds.
*
*  @note This routine assumes a 20 MHz clock.
*/

void Delay100uSec
(
   uns8 num100uSec   /**< Number of 100 microsecond units to delay         */
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
*
*  @note This routine assumes a 20 MHz clock.
*/

void Delay10uSec
(
   uns8 num10uSec    /**< Number of 10 microsecond units to delay         */
)
{
    while ( num10uSec > 0 )
    {
        us_spin( 10 );
    }

} // Delay10uSec

/***************************************************************************/
/**
*  Timer 0 interrupt handler
*/

SIGNAL(SIG_OVERFLOW0)        /* signal handler for tcnt0 overflow interrupt */
{
    static  int microTick = 0;

    if ( ++microTick >= 10 )
    {
        microTick = 0;

        gTickCount++;
    }

    // We want our timer tick to interrupt once a millisecond.
    // If we use 8 MHz/64 then we get 125000 counts/second. So setting
    // things up to overflow after 125 will give us 1000 overflows/second
    //
    // 256 - 125 = 131

    TCNT0 = 131;

} // Timer 0 Overflow

/***************************************************************************/
/**
*  InitTimer
*/

void InitTimer( void )
{
    TCCR0 = ( 0 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ); // Divide by 64
    TCNT0 = 0;

    // Enable Timer 0 interrupt

    timer_enable_int( BV( TOIE0 ));

    sei();

} // InitTimer

/***************************************************************************/
/**
*  Wait for Timer 0 to rollover
*/

void WaitForTimer0Rollover( void )
{
    uns16   prevCount = gTickCount;

    while ( gTickCount == prevCount )
    {
        ;
    }

} // WaitForTimer0Rollover


