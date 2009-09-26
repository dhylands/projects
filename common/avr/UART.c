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
*   @file   UART.c
*
*   @brief  Implements the interrupt drive UART access
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Config.h"
#include "CBUF.h"
#include "UART.h"

#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#include <avr/signal.h>
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

#if CFG_USE_UART0

#if ( CFG_UART0_RX_BUFFER_SIZE > 0 )

volatile UART0_RxBuffer_t gUart0RxBuf;

#else

void (*gUart0RxHandler)( uint8_t ch ) = 0;

#endif

#if ( CFG_UART0_TX_BUFFER_SIZE > 0 )
volatile UART0_TxBuffer_t gUart0TxBuf;
#endif

#endif  // CFG_USE_UART0

//---------------------------------------------------------------------------

#if CFG_USE_UART1

#if ( CFG_UART1_RX_BUFFER_SIZE > 0 )

volatile UART1_RxBuffer_t gUart1RxBuf;

#else

void (*gUart1RxHandler)( uint8_t ch ) = 0;

#endif

#if ( CFG_UART1_TX_BUFFER_SIZE > 0 )
volatile UART1_TxBuffer_t gUart1TxBuf;
#endif

#endif  // CFG_USE_UART1

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/*
 * We assume that all of the appropriate registers are initialized
 * in the InitHardware function.
 */

//***************************************************************************
/**
*   Interrupt handler for Uart Rx Complete
*/

#if CFG_USE_UART0

ISR( USART0_RX_vect )
{
    uint8_t ch = UDR0;   // Read the character from the UART

#if ( CFG_UART0_RX_BUFFER_SIZE > 0 )

    if ( !CBUF_IsFull( gUart0RxBuf ))
    {
        CBUF_Push( gUart0RxBuf, ch );
    }

#else
    if ( gUart0RxHandler != 0 )
    {
        gUart0RxHandler( ch );
    }
#endif

} // USART0_RX_vect

#endif  // CFG_USE_UART0

//---------------------------------------------------------------------------

#if CFG_USE_UART1

ISR( USART1_RX_vect )
{
    uint8_t ch = UDR1;   // Read the character from the UART

#if ( CFG_UART1_RX_BUFFER_SIZE > 0 )

    if ( !CBUF_IsFull( gUart1RxBuf ))
    {
        CBUF_Push( gUart1RxBuf, ch );
    }

#else
    if ( gUart1RxHandler != 0 )
    {
        gUart1RxHandler( ch );
    }
#endif

} // USART1_RX_vect

#endif  // CFG_USE_UART1

//***************************************************************************
/**
*   Interrupt handler for Uart Data Register Empty
*/

#if ( CFG_USE_UART0 && ( CFG_UART0_TX_BUFFER_SIZE > 0 ))

ISR( USART0_UDRE_vect )
{
    if ( CBUF_IsEmpty( gUart0TxBuf ))
    {
        // Nothing left to transmit, disable the transmit interrupt

        UCSR0B &= ~( 1 << UDRIE0 );
    }
    else
    {
        // Otherwise, write the next character from the TX Buffer

        UDR0 = CBUF_Pop( gUart0TxBuf );
    }

}  // USART0_UDRE_vect

#endif  // ( CFG_USE_UART0 && ( CFG_UART0_TX_BUFFER_SIZE > 0 ))

//---------------------------------------------------------------------------

#if ( CFG_USE_UART1 && ( CFG_UART1_TX_BUFFER_SIZE > 0 ))

ISR( USART1_UDRE_vect )
{
    if ( CBUF_IsEmpty( gUart1TxBuf ))
    {
        // Nothing left to transmit, disable the transmit interrupt

        UCSR1B &= ~( 1 << UDRIE1 );
    }
    else
    {
        // Otherwise, write the next character from the TX Buffer

        UDR1 = CBUF_Pop( gUart1TxBuf );
    }

}  // SIG_USART1_DATA

#endif  // ( CFG_USE_UART1 && ( CFG_UART1_TX_BUFFER_SIZE > 0 ))

//***************************************************************************
/**
*   Extracts a character from the receive buffer. This function will block
*   until a character is available.
*
*   This function returns an int (rather than a char) to be compatible 
*   with fdevopen()
*/

#if CFG_USE_UART0
#if ( CFG_UART0_RX_BUFFER_SIZE > 0 )

#if defined( __AVR_LIBC_VERSION__ )
int UART0_GetCharStdio( FILE *fs )
{
    return UART0_GetChar();
}
#endif

int UART0_GetChar( void )
{
    int ch;

    while ( CBUF_IsEmpty( gUart0RxBuf ))
    {
        ;
    }

    ch = CBUF_Pop( gUart0RxBuf );

#if CFG_UART0_CR_TO_LF

    // The following allows this to be used with fgets

    if ( ch == '\r' )
    {
        ch = '\n';
    }
#endif
#if CFG_UART0_ECHO_INPUT
    UART0_PutChar( ch );
#endif

    return ch;

} // UART_GetChar
#endif  // CFG_UART0_RX_BUFFER_SIZE > 0
#endif  // CFG_USE_UART0

//---------------------------------------------------------------------------

#if CFG_USE_UART1
#if ( CFG_UART1_RX_BUFFER_SIZE > 0 )

#if defined( __AVR_LIBC_VERSION__ )
int UART1_GetCharStdio( FILE *fs )
{
    return UART1_GetChar();
}
#endif

int UART1_GetChar( void )
{
    int ch;

    while ( CBUF_IsEmpty( gUart1RxBuf ))
    {
        ;
    }

    ch = CBUF_Pop( gUart1RxBuf );

#if CFG_UART1_CR_TO_LF

    // The following allows this to be used with fgets

    if ( ch == '\r' )
    {
        ch = '\n';
    }
#endif
#if CFG_UART1_ECHO_INPUT
    UART1_PutChar( ch );
#endif

    return ch;

} // UART_GetChar
#endif  // CFG_UART1_RX_BUFFER_SIZE > 0
#endif  // CFG_USE_UART1

//***************************************************************************
/**
*   Write's a single character to the UART. By returning an int, this
*   function is compatible with fdevopen()
*/

#if CFG_USE_UART0

#if defined( __AVR_LIBC_VERSION__ )
int UART0_PutCharStdio( char ch, FILE *fs )
{
    return UART0_PutChar( ch );
}
#endif

int UART0_PutChar( char ch )
{
#if CFG_UART0_LF_TO_CRLF
    if ( ch == '\n'  )
    {
        UART0_PutChar( '\r' );
    }
#endif

#if ( CFG_UART0_TX_BUFFER_SIZE > 0 )

    while ( CBUF_IsFull( gUart0TxBuf ))
    {
        ;
    }

    CBUF_Push( gUart0TxBuf, ch );

    // Enable the transmit interrupt now that there's a character in the
    // buffer.

    UCSR0B |= ( 1 << UDRIE0 );

#else

    // Wait for empty transmit buffer

    while (( UCSR0A & ( 1 << UDRE0 )) == 0 )
    {
        ;
    }

    // Send the character

    UDR0 = ch;
#endif

    return 0;

} // UART0_PutChar

#endif  // CFG_USE_UART0

//---------------------------------------------------------------------------

#if CFG_USE_UART1

#if defined( __AVR_LIBC_VERSION__ )
int UART1_PutCharStdio( char ch, FILE *fs )
{
    return UART1_PutChar( ch );
}
#endif

int UART1_PutChar( char ch )
{
#if CFG_UART1_LF_TO_CRLF
    if ( ch == '\n'  )
    {
        UART1_PutChar( '\r' );
    }
#endif

#if ( CFG_UART1_TX_BUFFER_SIZE > 0 )

    while ( CBUF_IsFull( gUart1TxBuf ))
    {
        ;
    }

    CBUF_Push( gUart1TxBuf, ch );

    // Enable the transmit interrupt now that there's a character in the
    // buffer.

    UCSR1B |= ( 1 << UDRIE1 );

#else

    // Wait for empty transmit buffer

    while (( UCSR1A & ( 1 << UDRE1 )) == 0 )
    {
        ;
    }

    // Send the character

    UDR1 = ch;
#endif

    return 0;

} // UART1_PutChar

#endif  // CFG_USE_UART1

//***************************************************************************
/**
*   Write a string to the UART.
*/

#if CFG_USE_UART0

void UART0_PutStr( const char *str )
{
    while ( *str != '\0' )
    {
        if ( *str == '\n' )
        {
            UART0_PutChar( '\r' );
        }
        UART0_PutChar( *str );

        str++;
    }

} // UART0_PutStr

#endif  // CFG_USE_UART0

//---------------------------------------------------------------------------

#if CFG_USE_UART1

void UART1_PutStr( const char *str )
{
    while ( *str != '\0' )
    {
        if ( *str == '\n' )
        {
            UART1_PutChar( '\r' );
        }
        UART1_PutChar( *str );

        str++;
    }

} // UART1_PutStr

#endif  // CFG_USE_UART1

//***************************************************************************
/**
*   Sets the function that will be called to process a received character
*/

#if CFG_USE_UART0
#if ( CFG_UART0_RX_BUFFER_SIZE == 0 )
void UART0_SetRxHandler( void (*rxHandler)( uint8_t ch ))
{
    gUart0RxHandler = rxHandler;

} // UART0_SetRxHandler
#endif
#endif

//---------------------------------------------------------------------------

#if CFG_USE_UART1
#if ( CFG_UART1_RX_BUFFER_SIZE == 0 )
void UART1_SetRxHandler( void (*rxHandler)( uint8_t ch ))
{
    gUart1RxHandler = rxHandler;

} // UART1_SetRxHandler
#endif
#endif

//***************************************************************************
/**
*   Write data to the UART. This function may block if insufficient space
*   is available in the transmit buffer.
*/

#if CFG_USE_UART0

void UART0_Write( const void *data, uint8_t len )
{
    const uint8_t *p = (const uint8_t *)data;

    while ( len > 0 )
    {
        UART0_PutChar( *p++ );
        len--;
    }

} // UART0_Write

#endif

//---------------------------------------------------------------------------

#if CFG_USE_UART1

void UART1_Write( const void *data, uint8_t len )
{
    const uint8_t *p = (const uint8_t *)data;

    while ( len > 0 )
    {
        UART1_PutChar( *p++ );
        len--;
    }

} // UART1_Write

#endif

//***************************************************************************
/**
*   Initialize the UART
*/

void InitUART( void )
{
#if CFG_USE_UART0

#if !defined( CFG_UART0_BAUD_RATE )
#   error   CFG_UART0_BAUD_RATE is undefined
#endif

#define UBRR0_INIT   (( CFG_CPU_CLOCK / 8 / CFG_UART0_BAUD_RATE ) - 1 )

#define UART0_DATA_BIT_8  (( 1 << UCSZ01 ) | ( 1 << UCSZ00 ))
#define UART0_PARITY_NONE (( 0 << UPM01 )  | ( 0 << UPM00 ))
#define UART0_STOP_BIT_1  ( 1 << USBS0 )

    // Initialize the UART

    UBRR0H = UBRR0_INIT >> 8;
    UBRR0L = UBRR0_INIT & 0xFF;

    UCSR0A = ( 1 << U2X0 );
    UCSR0B = (( 1 << RXCIE0 ) | ( 1 << RXEN0 ) | ( 1 << TXEN0 ));

#if defined( URSEL )
#   define  URSEL_INIT  ( 1 << URSEL )
#elif defined( URSEL0 )
#   define  URSEL_INIT  ( 1 << URSEL0 )
#else
#   define  URSEL_INIT  0
#endif

    UCSR0C = URSEL_INIT | UART0_DATA_BIT_8 | UART0_PARITY_NONE | UART0_STOP_BIT_1;

#endif

#if CFG_USE_UART1

#if !defined( CFG_UART1_BAUD_RATE )
#   error   CFG_UART1_BAUD_RATE is undefined
#endif

#define UBRR1_INIT   (( CFG_CPU_CLOCK / 8 / CFG_UART1_BAUD_RATE ) - 1 )

#define UART1_DATA_BIT_8  (( 1 << UCSZ11 ) | ( 1 << UCSZ10 ))
#define UART1_PARITY_NONE (( 0 << UPM11 )  | ( 0 << UPM10 ))
#define UART1_STOP_BIT_1  ( 1 << USBS1 )

    // Initialize the UART

    UBRR1H = UBRR1_INIT >> 8;
    UBRR1L = UBRR1_INIT & 0xFF;

    UCSR1A = ( 1 << U2X1 );
    UCSR1B = (( 1 << RXCIE1 ) | ( 1 << RXEN1 ) | ( 1 << TXEN1 ));

#if defined( URSEL )
#   define  URSEL_INIT  ( 1 << URSEL )
#elif defined( URSEL1 )
#   define  URSEL_INIT  ( 1 << URSEL1 )
#else
#   define  URSEL_INIT  0
#endif

    UCSR1C = URSEL_INIT | UART1_DATA_BIT_8 | UART1_PARITY_NONE | UART1_STOP_BIT_1;

#endif

} // InitUART

