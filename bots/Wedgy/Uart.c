/****************************************************************************
*
*  uart.c
*
*  Code for abstracting the UART interface (uses polled I/O)
*
****************************************************************************/

#include "Config.h"
#include "Uart.h"
#include <avr/io.h>

#define BAUD_RATE   38400
//#define BAUD_UBRR   (( CFG_CPU_CLOCK / ( 16 * BAUD_RATE )) - 1 )

#define BAUD_UBRR   12

/***************************************************************************/
/**
*  Initializes the UART
*/

void UART_Init( void )
{
    // Set the baud rate
    // UBRR = ( Fosc / ( 16 * baud )) - 1
    
    //UBRRH = BAUD_UBRR >> 8;
    //UBRRL = BAUD_UBRR & 0x00FF;

    UBRRH = 0;
    UBRRL = 12;

    // Enable the Transmitter and Receiver

    UCSRB = ( 1 << RXEN ) | ( 1 << TXEN );

    // Set to 8-N-1

#define DATA_BIT_8  ( 1 << UCSZ1 ) | ( 1 << UCSZ0 )
#define PARITY_NONE ( 0 << UPM1 )  | ( 0 << UPM0 )
#define STOP_BIT_1  ( 1 << USBS )

    UCSRC = ( 1 << URSEL ) | DATA_BIT_8 | PARITY_NONE | STOP_BIT_1;

} // UART_Init

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

	while (( UCSRA & ( 1 << UDRE )) == 0 )
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

