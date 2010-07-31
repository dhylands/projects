/****************************************************************************
*
*  uart.c
*
*  Code for abstracting the UART interface (uses polled I/O)
*
****************************************************************************/

#include "Uart.h"
#include <avr/io.h>

/***************************************************************************/
/**
*  Reads a character from the UART (waits until a character is available)
*/

uns8 UART_GetChar( void )
{
    // Wait until chcracter is available

    while ( !UART_IsCharAvailable() )
    {
        ;
    }

    // Read it and return it

    return UDR;

} // UART_GetChar

/***************************************************************************/
/**
*  Writes a character to the UART
*/

void UART_PutChar( uns8 ch )
{
    // Wait for empty transmit buffer

    while ( !UART_IsTxBufferEmpty() )
    {
        ;
    }

    // Send the character

    UDR = ch;

} // UART_PutChar

/***************************************************************************/
/**
*  Writes a string to the uart.
*/

void UART_PutStr( const char *str )
{
    while ( *str != '\0' )
    {
        UART_PutChar( *str );

        str++;
    }

} // UART_PutStr

/***************************************************************************/
/**
*  Determines if a character is available in the receive buffer.
*/

int  UART_IsCharAvailable( void )
{
    return ( UCSRA & ( 1 << RXC )) != 0;

} // UART_IsCharAvailable

/***************************************************************************/
/**
*  Determines if a character is available in the receive buffer.
*/

int  UART_IsTxBufferEmpty( void )
{
    // Wait for empty transmit buffer

    return (( UCSRA & ( 1 << UDRE )) != 0 );

} // UART_IsTxBufferEmpty

