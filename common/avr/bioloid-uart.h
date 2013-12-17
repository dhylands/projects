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
*   @file   bioloid-uart.h
*
*   @brief  Code which deals with UART0 or UART1 on an ATMega in a manner
*           compatible with the bioloid servos.
*
****************************************************************************/

#if !defined( BIOLOID_UART_H )
#define BIOLOID_UART_H      /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include "Config.h"

//---------------------------------------------------------------------------
//
// The following configuration options control how this code gets compiled.
//
//  CFG_BLD_USE_UARTx               0 or 1  Controls which UART is used
//                                  (currently only one).
//
//  CFG_BLD_UARTx_BAUD_RATE         Baud rate to run the UART at.
//
//  CFG_BLD_UARTx_RX_BUFFER_SIZE    How many bytes is the Rx buffer. Must
//                                  be a power of 2.
//
//  CFG_BLD_UARTx_TX_BUFFER_SIZE    How many bytes is the Tx buffer. Must
//                                  be a power of 2.
//
//  CFG_BLD_UARTx_RCV_TX            Should we receive our own transmissions?
//
//  CFG_BLD_UARTx_ENABLE_TX_PULLUP  Should the GPIO pullup be enabled for the
//                                  Tx line? Normally only the master on the
//                                  bus would need to do this.
//
//  For compatability, this module provides a subset of the UART API.
//
//      int  UARTx_GetChar( void );
//      int  UARTx_IsCharAvailable();
//      int  UARTx_PutChar( char ch );
//
//---------------------------------------------------------------------------

/* ---- Constants and Types ---------------------------------------------- */

#if CFG_BLD_USE_UART0

#if !defined( CFG_BLD_UART0_RX_BUFFER_SIZE )
#   error CFG_BLD_UART0_RX_BUFFER_SIZE isnt defined.
#endif

#if (( CFG_BLD_UART0_RX_BUFFER_SIZE & ( CFG_BLD_UART0_RX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_BLD_UART0_RX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_BLD_UART0_RX_BUFFER_SIZE > 128 )
typedef uint16_t BLD_UART0_RxIndex_t;
#else
typedef uint8_t  BLD_UART0_RxIndex_t;
#endif

typedef struct
{
    BLD_UART0_RxIndex_t m_getIdx;
    BLD_UART0_RxIndex_t m_putIdx;
    uint8_t             m_entry[ CFG_BLD_UART0_RX_BUFFER_SIZE ];

} BLD_UART0_RxBuffer_t;

int  UART0_GetChar( void );

#define BLD_gUart0RxBuf_SIZE sizeof( BLD_gUart0RxBuf.m_entry )

extern  volatile BLD_UART0_RxBuffer_t BLD_gUart0RxBuf;

#define UART0_IsCharAvailable()  (!CBUF_IsEmpty( BLD_gUart0RxBuf ))

//---------------------------------------------------------------------------

#if !defined( CFG_BLD_UART0_TX_BUFFER_SIZE )
#   error CFG_BLD_UART0_TX_BUFFER_SIZE isnt declared.
#endif

#if (( CFG_BLD_UART0_TX_BUFFER_SIZE & ( CFG_BLD_UART0_TX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_BLD_UART0_TX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_BLD_UART0_TX_BUFFER_SIZE > 128 )
typedef uint16_t BLD_UART0_TxIndex_t;
#else
typedef uint8_t  BLD_UART0_TxIndex_t;
#endif

typedef struct
{
    BLD_UART0_TxIndex_t m_getIdx;
    BLD_UART0_TxIndex_t m_putIdx;
    uint8_t             m_entry[ CFG_BLD_UART0_TX_BUFFER_SIZE ];

} BLD_UART0_TxBuffer_t;

#define BLD_gUart0TxBuf_SIZE sizeof( BLD_gUart0TxBuf.m_entry )

extern  volatile BLD_UART0_TxBuffer_t BLD_gUart0TxBuf;

int  UART0_PutChar( char ch );
void UART0_SetBaudRate( uint32_t baudRate );

#endif  // CFG_BLD_USE_UART0

//===========================================================================

#if CFG_BLD_USE_UART1

#if !defined( CFG_BLD_UART1_RX_BUFFER_SIZE )
#   error CFG_BLD_UART1_RX_BUFFER_SIZE isnt defined.
#endif

#if (( CFG_BLD_UART1_RX_BUFFER_SIZE > 0 ) && (( CFG_BLD_UART1_RX_BUFFER_SIZE & ( CFG_BLD_UART1_RX_BUFFER_SIZE - 1 )) != 0 ))
#   error CFG_BLD_UART1_RX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_BLD_UART1_RX_BUFFER_SIZE > 128 )
typedef uint16_t BLD_UART1_RxIndex_t;
#else
typedef uint8_t  BLD_UART1_RxIndex_t;
#endif

typedef struct
{
    BLD_UART1_RxIndex_t m_getIdx;
    BLD_UART1_RxIndex_t m_putIdx;
    uint8_t             m_entry[ CFG_BLD_UART1_RX_BUFFER_SIZE ];

} BLD_UART1_RxBuffer_t;

int  UART1_GetChar( void );

#define BLD_gUart1RxBuf_SIZE sizeof( BLD_gUart1RxBuf.m_entry )

extern  volatile BLD_UART1_RxBuffer_t BLD_gUart1RxBuf;

#define UART1_IsCharAvailable()  (!CBUF_IsEmpty( BLD_gUart1RxBuf ))

//---------------------------------------------------------------------------

#if !defined( CFG_BLD_UART1_TX_BUFFER_SIZE )
#   error CFG_BLD_UART1_TX_BUFFER_SIZE isnt declared.
#endif

#if (( CFG_BLD_UART1_TX_BUFFER_SIZE & ( CFG_BLD_UART1_TX_BUFFER_SIZE - 1 )) != 0 )
#   error CFG_BLD_UART1_TX_BUFFER_SIZE isnt a power of 2.
#endif

#if ( CFG_BLD_UART1_TX_BUFFER_SIZE > 128 )
typedef uint16_t BLD_UART1_TxIndex_t;
#else
typedef uint8_t  BLD_UART1_TxIndex_t;
#endif

typedef struct
{
    BLD_UART1_TxIndex_t m_getIdx;
    BLD_UART1_TxIndex_t m_putIdx;
    uint8_t             m_entry[ CFG_BLD_UART1_TX_BUFFER_SIZE ];

} BLD_UART1_TxBuffer_t;

#define BLD_gUart1TxBuf_SIZE sizeof( BLD_gUart1TxBuf.m_entry )

extern  volatile BLD_UART1_TxBuffer_t BLD_gUart1TxBuf;

int  UART1_PutChar( char ch );
void UART1_SetBaudRate( uint32_t baudRate );

#endif  // CFG_BLD_USE_UART1

void BLD_InitUART( void );

/** @} */

#endif /* BIOLOID_UART_H */

