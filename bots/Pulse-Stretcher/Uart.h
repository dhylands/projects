/****************************************************************************
*
*  uart.h
*
*  Code for abstracting the UART interface (uses polled I/O)
*
****************************************************************************/

#if !defined( UART_H )
#define UART_H

#if !defined( TYPES_H )
#   include "Types.h"
#endif

void UART_Init( void );
uns8 UART_GetChar( void );
void UART_PutChar( uns8 ch );
void UART_PutStr( const char *str );
int  UART_IsCharAvailable( void );
int  UART_IsTxBufferEmpty( void );

#endif  // UART_H
