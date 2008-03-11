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
*   @file   Timer-UART.h
*
*   @brief  This file implements the Tx portion of a UART using a timer.
*
****************************************************************************/

#if !defined( TIMER_UART_H )
#define TIMER_UART_H                              ///< Include Guard

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
//  CFG_TIMER_UART_BAUD_RATE        Determines the baud rate which will be
//                                  used for transmission.
//
//  CFG_TIMER_UART_TX_BUFFER_SIZE   How many bytes to put in the Tx buffer.
//                                  Setting this to zero will cause polled 
//                                  routines to be used.
//
//  CFG_TIMER_UART_LF_TO_CRLF       Translate sent LFs to CRLFs. Useful when
//                                  using a terminal program.
//
//  API Provided:
//
//      int  TimerUART_PutChar( char ch );
//      void TimerUART_PutStr( const char *s );
//      void TimerUART_Write( const void *data, uint8_t len );
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#if !defined( CFG_TIMER_UART_TX_BUFFER_SIZE )
#   error CFG_TIMER_UART_TX_BUFFER_SIZE isnt declared.
#endif

#if (( CFG_TIMER_UART_TX_BUFFER_SIZE & ( CFG_TIMER_UART_TX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_TIMER_UART_TX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_TIMER_UART_TX_BUFFER_SIZE > 128 )
typedef uint16_t TimerUART_TxIndex_t;
#else
typedef uint8_t TimerUART_TxIndex_t;
#endif

#if ( CFG_TIMER_UART_TX_BUFFER_SIZE > 0 )

typedef struct
{
    TimerUART_TxIndex_t m_getIdx;
    TimerUART_TxIndex_t m_putIdx;
    uint8_t         m_entry[ CFG_TIMER_UART_TX_BUFFER_SIZE ];

} TimerUART_TxBuffer_t;

#define gTimerUARTTxBuf_SIZE sizeof( gTimerUARTTxBuf.m_entry )

extern  volatile TimerUART_TxBuffer_t gTimerUARTTxBuf;

#endif

//---------------------------------------------------------------------------

#if defined( __AVR_LIBC_VERSION__ )
int  TimerUART_PutCharStdio( char ch, FILE *fs );
#else
#define TimerUART_PutCharStdio TimerUART_PutChar
#endif

int  TimerUART_PutChar( char ch );
void TimerUART_PutStr( const char *s );

void TimerUART_Write( const void *data, uint8_t len );

void InitTimerUART( void );

#if defined( __cplusplus )
}
#endif

/** @} */

#endif // TIMER_UART_H

