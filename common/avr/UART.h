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
*   @file   UART.h
*
*   @brief  This file implements an interrupt driven UART access.
*
****************************************************************************/

#if !defined( UART_H )
#define UART_H                              ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include <inttypes.h>
#include <avr/io.h>
#include "CBUF.h"
#include "Config.h"

#if defined( __AVR_LIBC_VERSION__ )
#   include <stdio.h>
#endif

#if defined( __cplusplus )
extern "C"
{
#endif

//---------------------------------------------------------------------------
//
//  To use this header file the following #defines should be made in Config.h
//  Replace the x by a 0 or a 1.
//
//  CFG_USE_UARTx               0 or 1  Controls if anything relating to that 
//                              UART is generated
//
//  CFG_UARTx_BAUD_RATE         Determines the baud rate which will be used.
//
//  CFG_UARTx_RX_BUFFER_SIZE    How many bytes to put in the Rx buffer.
//                              Setting the buffer size to zero, means that
//                              no buffers will be alllocated and a callback
//                              must be provided to deal with each received
//                              character.
//
//  CFG_UARTx_TX_BUFFER_SIZE    How many bytes to put in the Tx buffer.
//                              Setting this to zero will cause polled routines
//                              to be used.
//
//  CFG_UARTx_ECHO_INPUT        causes data received by UARTx_GetChar to be
//                              echoed. This is useful if the uart is connected
//                              to s termnal program and being used to prompt
//                              the user for input.
//
//  CFG_UARTx_CR_TO_LF          Translate received CRs to LFs. This allows
//                              the input to be compatible with fgets
//
//  CFG_UARTx_LF_TO_CRLF        Translate sent LFs to CRLFs. Useful when
//                              using a terminal program.
//
//  API Provided:
//
//      int  UARTx_GetChar( void );
//      int  UARTx_IsCharAvailable();
//      void UARTx_SetRxHandler( void (*rxHandler)( uint8_t ch ));
//      int  UARTx_PutChar( char ch );
//      void UARTx_PutStr( const char *s );
//      void UARTx_Write( const void *data, uint8_t len );
//      void InitUART( void );
//
//  Note: To allow the code to be compatible with single UART processors
//        CFG_USE_UART can be defined instead of CFG_USE_UARTx and then
//        The x (the uart number) can be dropped.
//
//---------------------------------------------------------------------------

#if defined( CFG_USE_UART )
#   if defined( CFG_USE_UART0 ) || defined( CFG_USE_UART1 )
#       error CFG_USE_UART is incompatible with CFG_USE_UART0 or CFG_USE_UART1
#   endif

#   if !defined( CFG_UART_RX_BUFFER_SIZE )
#       error CFG_UART_RX_BUFFER_SIZE isnt defined.
#   endif

#   if !defined( CFG_UART_TX_BUFFER_SIZE )
#       error CFG_UART_TX_BUFFER_SIZE isnt declared.
#   endif

#   define  CFG_USE_UART0               CFG_USE_UART
#   define  CFG_UART0_BAUD_RATE         CFG_UART_BAUD_RATE
#   define  CFG_UART0_RX_BUFFER_SIZE    CFG_UART_RX_BUFFER_SIZE
#   define  CFG_UART0_TX_BUFFER_SIZE    CFG_UART_TX_BUFFER_SIZE
#   define  CFG_UART0_LF_TO_CRLF        CFG_UART_LF_TO_CRLF
#   define  CFG_UART0_ECHO_INPUT        CFG_UART_ECHO_INPUT
#   define  CFG_UART0_CR_TO_LF          CFG_UART_CR_TO_LF

#   define  UART_GetChar                UART0_GetChar
#   define  UART_IsCharAvailable        UART0_IsCharAvailable
#   define  UART_SetRxHandler           UART0_SetRxHandler
#   define  UART_PutChar                UART0_PutChar
#   define  UART_PutStr                 UART0_PutStr
#   define  UART_Write                  UART0_Write

#   define  UART_GetCharStdio           UART0_GetCharStdio
#   define  UART_PutCharStdio           UART0_PutCharStdio

#endif  // CFG_USE_UART

#if !defined( CFG_USE_UART0 ) && !defined( CFG_USE_UART1 )
#   error   Expecting one of CFG_USE_UART0 or CFG_USE_UART1 to be defined
#endif

#if CFG_USE_UART0

// These defines allow this file to work with AVRs which only have one UART
// (i.e ATMega8, 16, 32)

#if !defined( UCSR0A )

#define UCSR0A          UCSRA
#define UCSR0B          UCSRB
#define UCSR0C          UCSRC
#define UBRR0H          UBRRH
#define UBRR0L          UBRRL
#define UDR0            UDR
#define UDRIE0          UDRIE
#define UDRE0           UDRE
#define U2X0            U2X
#define RXCIE0          RXCIE
#define RXEN0           RXEN
#define TXEN0           TXEN
#define UCSZ01          UCSZ1
#define UCSZ00          UCSZ0
#define UPM01           UPM1
#define UPM00           UPM0
#define USBS0           USBS

#endif

#if defined( USART_RX_vect )
#   define  USART0_RX_vect      USART_RX_vect
#   define  USART0_UDRE_vect    USART_UDRE_vect
#endif

//---------------------------------------------------------------------------

#if !defined( CFG_UART0_RX_BUFFER_SIZE )
#   error CFG_UART0_RX_BUFFER_SIZE isnt defined.
#endif

#if (( CFG_UART0_RX_BUFFER_SIZE > 0 ) && (( CFG_UART0_RX_BUFFER_SIZE & ( CFG_UART0_RX_BUFFER_SIZE - 1 )) != 0 ))
#   error CFG_UART0_RX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_UART0_RX_BUFFER_SIZE > 128 )
typedef uint16_t UART0_RxIndex_t;
#else
typedef uint8_t  UART0_RxIndex_t;
#endif

#if ( CFG_UART0_RX_BUFFER_SIZE > 0 )
typedef struct
{
    UART0_RxIndex_t m_getIdx;
    UART0_RxIndex_t m_putIdx;
    uint8_t         m_entry[ CFG_UART0_RX_BUFFER_SIZE ];

} UART0_RxBuffer_t;

#if defined( __AVR_LIBC_VERSION__ )
int  UART0_GetCharStdio( FILE *fs );
#else
#define UART0_GetCharStdio UART0_GetChar
#endif

int  UART0_GetChar( void );

#define gUart0RxBuf_SIZE sizeof( gUart0RxBuf.m_entry )

extern  volatile UART0_RxBuffer_t gUart0RxBuf;

#define UART0_IsCharAvailable()  (!CBUF_IsEmpty( gUart0RxBuf ))

#else

void UART0_SetRxHandler( void (*rxHandler)( uint8_t ch ));

#endif

//---------------------------------------------------------------------------

#if !defined( CFG_UART0_TX_BUFFER_SIZE )
#   error CFG_UART0_TX_BUFFER_SIZE isnt declared.
#endif

#if (( CFG_UART0_TX_BUFFER_SIZE & ( CFG_UART0_TX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_UART0_TX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_UART0_TX_BUFFER_SIZE > 128 )
typedef uint16_t UART0_TxIndex_t;
#else
typedef uint8_t UART0_TxIndex_t;
#endif

#if ( CFG_UART0_TX_BUFFER_SIZE > 0 )

typedef struct
{
    UART0_TxIndex_t m_getIdx;
    UART0_TxIndex_t m_putIdx;
    uint8_t         m_entry[ CFG_UART0_TX_BUFFER_SIZE ];

} UART0_TxBuffer_t;

#define gUart0TxBuf_SIZE sizeof( gUart0TxBuf.m_entry )

extern  volatile UART0_TxBuffer_t gUart0TxBuf;

#endif

//---------------------------------------------------------------------------

#if defined( __AVR_LIBC_VERSION__ )
int  UART0_PutCharStdio( char ch, FILE *fs );
#else
#define UART0_PutCharStdio UART0_PutChar
#endif

int  UART0_PutChar( char ch );
void UART0_PutStr( const char *s );

void UART0_Write( const void *data, uint8_t len );

#endif  // CFG_USE_UART0

//===========================================================================

#if CFG_USE_UART1

//---------------------------------------------------------------------------

#if !defined( CFG_UART1_RX_BUFFER_SIZE )
#   error CFG_UART1_RX_BUFFER_SIZE isnt defined.
#endif

#if (( CFG_UART1_RX_BUFFER_SIZE > 0 ) && (( CFG_UART1_RX_BUFFER_SIZE & ( CFG_UART1_RX_BUFFER_SIZE - 1 )) != 0 ))
#   error CFG_UART1_RX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_UART1_RX_BUFFER_SIZE > 128 )
typedef uint16_t UART1_RxIndex_t;
#else
typedef uint8_t  UART1_RxIndex_t;
#endif

#if ( CFG_UART1_RX_BUFFER_SIZE > 0 )
typedef struct
{
    UART1_RxIndex_t m_getIdx;
    UART1_RxIndex_t m_putIdx;
    uint8_t         m_entry[ CFG_UART1_RX_BUFFER_SIZE ];

} UART1_RxBuffer_t;

#if defined( __AVR_LIBC_VERSION__ )
int  UART1_GetCharStdio( FILE *fs );
#else
#define UART1_GetCharStdio UART1_GetChar
#endif

int  UART1_GetChar( void );

#define gUart1RxBuf_SIZE sizeof( gUart1RxBuf.m_entry )

extern  volatile UART1_RxBuffer_t gUart1RxBuf;

#define UART1_IsCharAvailable()  (!CBUF_IsEmpty( gUart1RxBuf ))

#else

void UART1_SetRxHandler( void (*rxHandler)( uint8_t ch ));

#endif

//---------------------------------------------------------------------------

#if !defined( CFG_UART1_TX_BUFFER_SIZE )
#   error CFG_UART1_TX_BUFFER_SIZE isnt declared.
#endif

#if (( CFG_UART1_TX_BUFFER_SIZE & ( CFG_UART1_TX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_UART1_TX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_UART1_TX_BUFFER_SIZE > 128 )
typedef uint16_t UART1_TxIndex_t;
#else
typedef uint8_t UART1_TxIndex_t;
#endif

#if ( CFG_UART1_TX_BUFFER_SIZE > 0 )

typedef struct
{
    UART1_TxIndex_t m_getIdx;
    UART1_TxIndex_t m_putIdx;
    uint8_t         m_entry[ CFG_UART1_TX_BUFFER_SIZE ];

} UART1_TxBuffer_t;

#define gUart1TxBuf_SIZE sizeof( gUart1TxBuf.m_entry )

extern  volatile UART1_TxBuffer_t gUart1TxBuf;

#endif

//---------------------------------------------------------------------------

#if defined( __AVR_LIBC_VERSION__ )
int  UART1_PutCharStdio( char ch, FILE *fs );
#else
#define UART1_PutCharStdio UART1_PutChar
#endif

int  UART1_PutChar( char ch );
void UART1_PutStr( const char *s );

void UART1_Write( const void *data, uint8_t len );

#endif  // CFG_USE_UART1

void InitUART( void );

#if defined( __cplusplus )
}
#endif

/** @} */

#endif // UART_H

