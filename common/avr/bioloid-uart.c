/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   bioloid-uart.c
*
*   @brief  Code which deals with UART0 or UART1 on an ATMega in a manner
*           compatible with the bioloid servos.
*
*           The bioloid servos use a half-duplex protocol over a single
*           wire. This code take advantage of the fact that disabling
*           the Transmitter cause the pin to revert to it's GPIO function
*           (i.e. controlled by DDR), so we need to ensure that the Tx
*           pin is configured as an input.
*
*           This code also assumes that the Tx and Rx lines are wired
*           together.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Config.h"

#include "bioloid-uart.h"
#include "CBUF.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#if defined (__AVR_ATmega8__) \
 || defined (__AVR_ATmega48__) \
 || defined (__AVR_ATmega88__) \
 || defined (__AVR_ATmega168__)

#define BLD_UART0_DDR       DDRD
#define BLD_UART0_PORT      PORTD
#define BLD_UART0_RX_MASK   ( 1 << 0 )
#define BLD_UART0_TX_MASK   ( 1 << 1 )

#define USART0_RX_vect      USART_RX_vect
#define USART0_UDRE_vect    USART_UDRE_vect
#define USART0_TX_vect      USART_TX_vect

#elif defined( __AVR_ATmega128__ )

#define BLD_UART0_DDR       DDRE
#define BLD_UART0_PORT      PORTE
#define BLD_UART0_RX_MASK   ( 1 << 0 )
#define BLD_UART0_TX_MASK   ( 1 << 1 )

#define BLD_UART1_DDR       DDRD
#define BLD_UART1_PORT      PORTD
#define BLD_UART1_RX_MASK   ( 1 << 2 )
#define BLD_UART1_TX_MASK   ( 1 << 3 )

#elif defined( __AVR_ATmega164P__ )

#define BLD_UART0_DDR       DDRD
#define BLD_UART0_PORT      PORTD
#define BLD_UART0_RX_MASK   ( 1 << 0 )
#define BLD_UART0_TX_MASK   ( 1 << 1 )

#define BLD_UART1_DDR       DDRD
#define BLD_UART1_PORT      PORTD
#define BLD_UART1_RX_MASK   ( 1 << 2 )
#define BLD_UART1_TX_MASK   ( 1 << 3 )

#else
#   error   Processor not yet supported
#endif

/* ---- Private Variables ------------------------------------------------ */

#if CFG_BLD_USE_UART0

volatile BLD_UART0_RxBuffer_t BLD_gUart0RxBuf;
volatile BLD_UART0_TxBuffer_t BLD_gUart0TxBuf;

#endif

#if CFG_BLD_USE_UART1

volatile BLD_UART1_RxBuffer_t BLD_gUart1RxBuf;
volatile BLD_UART1_TxBuffer_t BLD_gUart1TxBuf;

#endif


/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

//***************************************************************************
/**
*   Interrupt handler for characters from the servo.
*/

#if CFG_BLD_USE_UART0

ISR( USART0_RX_vect )
{
    uint8_t ch = UDR0;   // Read the character from the UART

	if ( !CBUF_IsFull( BLD_gUart0RxBuf ))
	{
		CBUF_Push( BLD_gUart0RxBuf, ch );
	}

} // USART0_RX_vect

#endif // CFG_BLD_USE_UART0

//***************************************************************************
/**
*   Interrupt handler for characters from the servo.
*/

#if CFG_BLD_USE_UART1

ISR( USART1_RX_vect )
{
    uint8_t ch = UDR1;   // Read the character from the UART

	if ( !CBUF_IsFull( BLD_gUart1RxBuf ))
	{
		CBUF_Push( BLD_gUart1RxBuf, ch );
	}

} // USART1_RX_vect

#endif // CFG_BLD_USE_UART1

//***************************************************************************
/**
*   Interrupt handler for Uart Data Register Empty (Space available in
*   the uart to the servo).
*/

#if CFG_BLD_USE_UART0

ISR( USART0_UDRE_vect )
{
    if ( CBUF_IsEmpty( BLD_gUart0TxBuf ))
    {
        // Nothing left to transmit, disable the transmit interrupt.
		// The transmitter will be disabled when the Tx Complete interrupt 
		// occurs.

        UCSR0B &= ~( 1 << UDRIE0 );
    }
    else
    {
        // Write the next character from the TX Buffer

        UDR0 = CBUF_Pop( BLD_gUart0TxBuf );

		// Enable the Tx Complete interrupt. This is used to turn off the
		// transmitter and enable the receiver.

		UCSR0B |= ( 1 << TXCIE0 );
    }

}  // USART0_UDRE_vect

#endif // CFG_BLD_USE_UART0

//***************************************************************************
/**
*   Interrupt handler for Uart Data Register Empty (Space available in
*   the uart to the servo).
*/

#if CFG_BLD_USE_UART1

ISR( USART1_UDRE_vect )
{
    if ( CBUF_IsEmpty( BLD_gUart1TxBuf ))
    {
        // Nothing left to transmit, disable the transmit interrupt.
		// The transmitter will be disabled when the Tx Complete interrupt 
		// occurs.

        UCSR1B &= ~( 1 << UDRIE1 );
    }
    else
    {
        // Write the next character from the TX Buffer

        UDR1 = CBUF_Pop( BLD_gUart1TxBuf );

		// Enable the Tx Complete interrupt. This is used to turn off the
		// transmitter and enable the receiver.

		UCSR1B |= ( 1 << TXCIE1 );
    }

}  // USART1_UDRE_vect

#endif // CFG_BLD_USE_UART1

//***************************************************************************
/**
*   Interrupt handler for when the data has completely left the Tx shift
*	register.
*
*	We use this to disable the transmitter and re-enable the receiver.
*/

#if CFG_BLD_USE_UART0

ISR( USART0_TX_vect )
{
	// Since we were called, the data register is also empty. This means
	// that we can go ahead and disable the transmitter and re-enable the
	// receiver.

	UCSR0B &= ~(( 1 << TXEN0 ) | ( 1 << TXCIE0 ));
	UCSR0B |=  (( 1 << RXEN0 ) | ( 1 << RXCIE0 ));

} // USART0_TX_vect

#endif  // CFG_BLD_USE_UART0

//***************************************************************************
/**
*   Interrupt handler for when the data has completely left the Tx shift
*	register.
*
*	We use this to disable the transmitter and re-enable the receiver.
*/

#if CFG_BLD_USE_UART1

ISR( USART1_TX_vect )
{
	// Since we were called, the data register is also empty. This means
	// that we can go ahead and disable the transmitter and re-enable the
	// receiver.

	UCSR1B &= ~(( 1 << TXEN1 ) | ( 1 << TXCIE1 ));
	UCSR1B |=  (( 1 << RXEN1 ) | ( 1 << RXCIE1 ));

} // USART1_TX_vect

#endif  // CFG_BLD_USE_UART1

//***************************************************************************
/**
*   Initialize the UART.
*/

void BLD_InitUART( void )
{
#if CFG_BLD_USE_UART0

#if !defined( CFG_BLD_UART0_BAUD_RATE )
#   error   CFG_BLD_UART0_BAUD_RATE is undefined
#endif

#define UART0_DATA_BIT_8  (( 1 << UCSZ01 ) | ( 1 << UCSZ00 ))
#define UART0_PARITY_NONE (( 0 << UPM01 )  | ( 0 << UPM00 ))
#define UART0_STOP_BIT_1  ( 0 << USBS0 )

    // Configure TxD and RxD pins as inputs and turn off the pullups

    BLD_UART0_DDR  &= ~( BLD_UART0_RX_MASK | BLD_UART0_TX_MASK );

#if CFG_BLD_UART0_ENABLE_TX_PULLUP
    BLD_UART0_PORT &= ~BLD_UART0_RX_MASK;
    BLD_UART0_PORT |=  BLD_UART0_TX_MASK;
#else
    BLD_UART0_PORT &= ~( BLD_UART0_RX_MASK | BLD_UART0_TX_MASK );
#endif

    // Initialize the UART

    UART0_SetBaudRate( CFG_BLD_UART0_BAUD_RATE );

    UCSR0A = ( 1 << U2X0 );
    UCSR0B = (( 1 << RXCIE0 ) | ( 1 << RXEN0 ));
    UCSR0C = ( UART0_DATA_BIT_8 | UART0_PARITY_NONE | UART0_STOP_BIT_1 );

#endif

#if CFG_BLD_USE_UART1

#if !defined( CFG_BLD_UART1_BAUD_RATE )
#   error   CFG_BLD_UART1_BAUD_RATE is undefined
#endif

#define UART1_DATA_BIT_8  (( 1 << UCSZ11 ) | ( 1 << UCSZ10 ))
#define UART1_PARITY_NONE (( 0 << UPM11 )  | ( 0 << UPM10 ))
#define UART1_STOP_BIT_1  ( 0 << USBS1 )

    // Configure TxD and RxD pins as inputs and turn off the pullups

    BLD_UART1_DDR  &= ~( BLD_UART1_RX_MASK | BLD_UART1_TX_MASK );

#if CFG_BLD_UART1_ENABLE_TX_PULLUP
    BLD_UART1_PORT &= ~BLD_UART1_RX_MASK;
    BLD_UART1_PORT |=  BLD_UART1_TX_MASK;
#else
    BLD_UART1_PORT &= ~( BLD_UART1_RX_MASK | BLD_UART1_TX_MASK );
#endif

    // Initialize the UART

    UART1_SetBaudRate( CFG_BLD_UART1_BAUD_RATE );

    UCSR1A = ( 1 << U2X1 );
    UCSR1B = (( 1 << RXCIE1 ) | ( 1 << RXEN1 ));
    UCSR1C = ( UART1_DATA_BIT_8 | UART1_PARITY_NONE | UART1_STOP_BIT_1 );

#endif

} // BLD_InitUART

//***************************************************************************
/**
*   Read a character from the UART 
*/

#if CFG_BLD_USE_UART0

int  UART0_GetChar( void )
{
    while ( CBUF_IsEmpty( BLD_gUart0RxBuf ))
    {
        ;
    }

    return CBUF_Pop( BLD_gUart0RxBuf );

} // UART0_GetChar

#endif // CFG_BLD_USE_UART0

#if CFG_BLD_USE_UART1

int  UART1_GetChar( void )
{
    while ( CBUF_IsEmpty( BLD_gUart1RxBuf ))
    {
        ;
    }

    return CBUF_Pop( BLD_gUart1RxBuf );

} // UART1_GetChar

#endif // CFG_BLD_USE_UART1

//***************************************************************************
/**
*   Write a character to the UART 
*/

#if CFG_BLD_USE_UART0

int  UART0_PutChar( char ch )
{
    while ( CBUF_IsFull( BLD_gUart0TxBuf ))
    {
        ;
    }

    CBUF_Push( BLD_gUart0TxBuf, ch );

    // We want to write a character, so we need to enable the transmitter
    // and disable the receiver.

#if !CFG_BLD_UART0_RCV_TX
    UCSR0B &= ~(( 1 << RXEN0 ) | ( 1 << RXCIE0 ));
#endif
    UCSR0B |= (( 1 << TXEN0 ) | ( 1 << UDRIE0 ));

    return 0;

} // UART0_PutChar

#endif // CFG_BLD_USE_UART0

#if CFG_BLD_USE_UART1

int  UART1_PutChar( char ch )
{
    while ( CBUF_IsFull( BLD_gUart1TxBuf ))
    {
        ;
    }

    CBUF_Push( BLD_gUart1TxBuf, ch );

    // We want to write a character, so we need to enable the transmitter
    // and disable the receiver.

#if !CFG_BLD_UART1_RCV_TX
    UCSR1B &= ~(( 1 << RXEN1 ) | ( 1 << RXCIE1 ));
#endif
    UCSR1B |= (( 1 << TXEN1 ) | ( 1 << UDRIE1 ));

    return 0;

} // UART1_PutChar

#endif // CFG_BLD_USE_UART1

//***************************************************************************
/**
*   Set the baud rate.
*/

#if defined( CFG_BLD_USE_UART0 )

void UART0_SetBaudRate( uint32_t baudRate )
{
    uint32_t    divisor;

    divisor = (( CFG_CPU_CLOCK / 8 / baudRate ) - 1 );

    // Set the baud rate

    UBRR0H = ( divisor >> 8 ) & 0xFF;
    UBRR0L = divisor & 0xFF;

} // UART0_SetBaudRate

#endif  // CFG_BLD_USE_UART0

#if defined( CFG_BLD_USE_UART1 )

void UART1_SetBaudRate( uint32_t baudRate )
{
    uint32_t    divisor;

    divisor = (( CFG_CPU_CLOCK / 8 / baudRate ) - 1 );

    // Set the baud rate

    UBRR1H = ( divisor >> 8 ) & 0xFF;
    UBRR1L = divisor & 0xFF;

} // UART1_SetBaudRate

#endif  // CFG_BLD_USE_UART1



