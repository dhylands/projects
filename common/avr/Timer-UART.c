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
*   @file   Timer-UART.c
*
*   @brief  Implements the Tx portion of a UART using a timer.
*
*   This started out using the ideas presented in Application Note AVR304
*
*   We need the reload value to fit within 8 bits.
*
*   8000000 / 38400 / 8 =  26
*   8000000 /  4800 / 8 = 208
*  16000000 / 38400 / 8 =  52
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Config.h"
#include "CBUF.h"
#include "Timer.h"
#include "Timer-UART.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------

#if !defined( CFG_TIMER_UART_BAUD_RATE )
#   error   CFG_TIMER_UART_BAUD_RATE needs to be defined
#endif

#if !defined( CFG_TIMER_UART_DDR )
#   error   CFG_TIMER_UART_DDR needs to be defined
#endif

#if !defined( CFG_TIMER_UART_PORT )
#   error   CFG_TIMER_UART_PORT needs to be defined
#endif

#if !defined( CFG_TIMER_UART_MASK )
#   error   CFG_TIMER_UART_MASK needs to be defined
#endif

// ---- Private Variables ---------------------------------------------------

#if ( CFG_TIMER_UART_TX_BUFFER_SIZE > 0 )
volatile TimerUART_TxBuffer_t gTimerUARTTxBuf;
#endif

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/*
 * We assume that all of the appropriate registers are initialized
 * in the InitHardware function.
 */

//***************************************************************************
/**
*   Interrupt handler for Timer2 Compare Match, which happens each time
*   the counter (TCNT2) reaches OCR2A.
*
*   This represents one bit time.
*/

static  uint8_t     gTxData;
static  uint8_t     gTxBitCount;

#if ( CFG_TIMER_UART_TX_BUFFER_SIZE > 0 )

ISR( TIMER2_COMPA_vect )
{
    if ( gTxBitCount > 1 )
    {
        if ( gTxData & 1 )
        {
            CFG_TIMER_UART_PORT |= CFG_TIMER_UART_MASK;
        }
        else
        {
            CFG_TIMER_UART_PORT &= ~CFG_TIMER_UART_MASK;
        }
        gTxData >>= 1;
    }
    else
    {
        CFG_TIMER_UART_PORT |= CFG_TIMER_UART_MASK;
    }

    if ( gTxBitCount > 0 )
    {
        gTxBitCount--;
    }
    else
    {
        if ( CBUF_IsEmpty( gTimerUARTTxBuf ))
        {
            // Nothing left to transmit, disable the transmit interrupt

            TIMSK2 &= ~( 1 << OCIE2A );
        }
        else
        {
            // Otherwise, write the next character from the TX Buffer,
            // and start things off with the start bit.

            CFG_TIMER_UART_PORT &= ~CFG_TIMER_UART_MASK;

            gTxData = CBUF_Pop( gTimerUARTTxBuf );
            gTxBitCount = 9;
        }
    }

}  // TIMER2_COMPA_vect

#endif

//***************************************************************************
/**
*   Write's a single character to the UART. By returning an int, this
*   function is compatible with fdevopen()
*/

int TimerUART_PutCharStdio( char ch, FILE *fs )
{
    return TimerUART_PutChar( ch );
}

int TimerUART_PutChar( char ch )
{
#if CFG_TIMER_UART_LF_TO_CRLF
    if ( ch == '\n'  )
    {
        TimerUART_PutChar( '\r' );
    }
#endif

#if ( CFG_TIMER_UART_TX_BUFFER_SIZE > 0 )

    // Wait for space to become available in the FIFO

    while ( CBUF_IsFull( gTimerUARTTxBuf ))
    {
        ;
    }

    CBUF_Push( gTimerUARTTxBuf, ch );

    if (( TIMSK2 & ( 1 << OCIE2A )) == 0 )
    {
        // The fact that interrupts are currently disabled means that the
        // character we just put into the FIFO was the first, so we need
        // to re-enable interrupts to kick things off.

        // We don't stop the timer when interrupts are disabled, so we
        // almost certainly need to clear any indication that an interrupt
        // occured.

        TCNT2 = 0;
        TIFR2  = ( 1 << OCF2A );

        // Make sure that interrupts are enabled. If this was the first
        // character placed in the FIFO, then when the timer wraps again it
        // will pickup the next character from the FIFO.

        TIMSK2 = ( 1 << OCIE2A );
    }

#else

    // Wait for empty transmit buffer

    xxxx

    while (( UCSR0A & ( 1 << UDRE0 )) == 0 )
    {
        ;
    }

    // Send the character

    UDR0 = ch;
#endif

    return 0;

} // TimerUART_PutChar

//***************************************************************************
/**
*   Write a string to the UART.
*/

void TimerUART_PutStr( const char *str )
{
    while ( *str != '\0' )
    {
        if ( *str == '\n' )
        {
            TimerUART_PutChar( '\r' );
        }
        TimerUART_PutChar( *str );

        str++;
    }

} // TimerUART_PutStr

//***************************************************************************
/**
*   Write data to the UART. This function may block if insufficient space
*   is available in the transmit buffer.
*/

void TimerUART_Write( const void *data, uint8_t len )
{
    const uint8_t *p = (const uint8_t *)data;

    while ( len > 0 )
    {
        TimerUART_PutChar( *p++ );
        len--;
    }

} // TimerUART_Write

//***************************************************************************
/**
*   Initialize the UART
*
*   8000000 / 38400 = 208
*/

void InitTimerUART( void )
{
#define PRESCALAR   8
#define RELOAD      (( CFG_CPU_CLOCK / CFG_TIMER_UART_BAUD_RATE ) / PRESCALAR )

    // We use the CTC mode (WGM = 2), which starts counting at zero and overflows
    // when TCNT2 reaches OCR2A

    TCCR2A = ( 1 << WGM21 ) | ( 0 << WGM20 );
    TCCR2B = ( 0 << WGM22 ) | TIMER2_CLOCK_SEL_DIV_8;

    // Configure the Tx pin as an output and put it in the marking state

    CFG_TIMER_UART_DDR  |= CFG_TIMER_UART_MASK;
    CFG_TIMER_UART_PORT |= CFG_TIMER_UART_MASK;

    OCR2A = RELOAD;

} // InitTimerUART

