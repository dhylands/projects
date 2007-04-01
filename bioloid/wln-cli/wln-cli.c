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
*   @file   spi-master.c 
*
*   @brief  Implements the ...
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <avr/io.h>
#include <stdint.h>

#include "Delay.h"
#include "Log.h"
#include "Timer.h"
#include "UART.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_MASK    ( 1 << 5 )

#define SS_DDR      DDRB
#define SS_PORT     PORTB
#define SS_MASK     ( 1 << 2 )

#define MOSI_DDR    DDRB
#define MOSI_PORT   PORTB
#define MOSI_MASK   ( 1 << 3 )

#define MISO_DDR    DDRB
#define MISO_PORT   PORTB
#define MISO_PIN    PINB
#define MISO_MASK   ( 1 << 4 )

#define SCK_DDR     DDRB
#define SCK_PORT    PORTB
#define SCK_MASK    ( 1 << 5 )

#define INT_DDR     DDRB
#define INT_PORT    PORTB
#define INT_PIN     PINB
#define INT_MASK    ( 1 << 1 )

#define RESET_DDR   DDRD
#define RESET_PORT  PORTD
#define RESET_MASK  ( 1 << 2 )

#define TRIGGER_DDR     DDRC
#define TRIGGER_PORT    PORTC
#define TRIGGER_MASK    ( 1 << 4 )

#define WLN_CMD_READ_CONFIG     0x40
#define WLN_CMD_WRITE_DATA      0x80
#define WLN_CMD_READ_DATA       0x00

#define WLN_STATUS_TX_DATA_AVAIL    0x80
#define WLN_STATUS_RX_SPACE_AVAIL   0x40
#define WLN_STATUS_RX_IRQ           0x20
#define WLN_STATUS_TX_IRQ           0x10

#define SetDirectionIn(x)   x ## _DDR &= ~ x ## _MASK; x ## _PORT &= ~ x ##_MASK
#define SetDirectionOut(x)  x ## _DDR |=   x ## _MASK

#define SPI_BIT_BANG    1

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Activates the SS signal that goes to the slave
*/

static inline void ActivateSlave( void )
{
    // SS is active low

    SS_PORT &= ~SS_MASK;

} // ActivateSlave

//***************************************************************************
/**
*   Deactivates the SS signal that goes to the slave
*/

static inline void DeactivateSlave( void )
{
    // SS is active low

    SS_PORT |= SS_MASK;

} // DeactivateSlave

#if SPI_BIT_BANG

//***************************************************************************
/**
*   Functions for manipulating SCK, MOSI and MISO
*/

static inline void SPI_ClockLow( void )
{
    SCK_PORT &= ~SCK_MASK;
}

static inline void SPI_ClockHigh( void )
{
    SCK_PORT |= SCK_MASK;
}

static inline void SPI_DataOut( uint8_t bit )
{
    if ( bit )
    {
        MOSI_PORT |= MOSI_MASK;
    }
    else
    {
        MOSI_PORT &= ~MOSI_MASK;
    }
}

static inline uint8_t SPI_DataIn( void )
{
    return ( MISO_PIN & MISO_MASK ) != 0;  
}

#endif

//***************************************************************************
/**
*   Initialize the SPI for Master mode
*/

static void SPI_MasterInit( void )
{
#if SPI_BIT_BANG

    // SCK idles low

    SPI_ClockLow();

#else
    // The Airborne WLAN only supports mode = 1

    SPSR = ( 1 << SPI2X );  // Pick divide by 128 clock frequency

    SPCR = ( 0 << SPIE )    // Enable interrupts
         | ( 1 << SPE )     // Enable SPI
         | ( 0 << DORD )    // Send MSB first
         | ( 1 << MSTR )    // Enable master mode
         | ( 0 << CPOL )    // SPI Mode 1 has CPOL=0, CPHA=1
         | ( 1 << CPHA )
         | ( 0 << SPR1 )    // Pick Divide by 128 clock frequency
         | ( 1 << SPR0 );
#endif

    DeactivateSlave();

} // SPI_MasterInit

//***************************************************************************
/**
*   Write a byte to the slave, and return the byte that's read
*/

static uint8_t SPI_XferByte( uint8_t b )
{
    uint8_t b_in = b;

#if SPI_BIT_BANG

    // According to the photo in the DPAC datasheet, CPOL = 0, CPHA = 1,
    // the output is driven on the rising edge, and sampled on the
    // falling edge.

    uint8_t i;

    for ( i = 0; i < 8; i++ )
    {
        us_spin( 10 );

        SPI_DataOut( b & 0x80 );
        SPI_ClockHigh();

        us_spin( 10 );

        b <<= 1;
        b |= ( SPI_DataIn() & 0x01 );
        SPI_ClockLow();
    }

#else
    // Kick off the transmission

    SPDR = b;

    // Wait for the transmission to complete

    while (( SPSR & ( 1 << SPIF )) == 0 )
    {
        ;
    }

    // Read (and return) the read byte

    b = SPDR;
#endif

    Log( "Xfer sent: 0x%02x rcvd: 0x%02x\n", b_in, b );

    return b;

} // SPI_XferByte

//***************************************************************************
/**
*   WLN_PrintStatus
*/

static void WLN_PrintStatus( uint8_t status )
{
    Log( "TxBuf:%s RxBuf:%s TxIrq:%d RxIrq:%d Int:%d\n",
         (( status & WLN_STATUS_TX_DATA_AVAIL ) == 0 )  ? "Empty" : "Data ",
         (( status & WLN_STATUS_RX_SPACE_AVAIL ) == 0 ) ? "Full " : "Space",
         (( status & WLN_STATUS_TX_IRQ ) == 0 )         ? 0       : 1,
         (( status & WLN_STATUS_RX_IRQ ) == 0 )         ? 0       : 1,
         (( INT_PIN & INT_MASK ) != 0 ));

} // WLN_PrintStatus

//***************************************************************************
/**
*   WLN_ReadStatus
*/

static uint8_t WLN_ReadStatus( void )
{
    uint8_t status;

    ActivateSlave();

    SPI_XferByte( WLN_CMD_READ_CONFIG );
    status = SPI_XferByte( 0 );

    DeactivateSlave();

    return status;

} // WLN_ReadStatus

//***************************************************************************
/**
*   WLN_ReadData
*/

static void WLN_ReadData( void )
{
    uint8_t lenHi;
    uint8_t lenLo;
    uint8_t data[8];
    uint8_t i;
    uint8_t len;

    TRIGGER_PORT |= TRIGGER_MASK;
    us_spin( 10 );

    ActivateSlave();

    SPI_XferByte( WLN_CMD_READ_DATA );
    lenHi = SPI_XferByte( 0 );
    lenLo = SPI_XferByte( 0 );

    if (( lenHi == 0 ) && ( lenLo <= sizeof( data )))
    {
        len = lenLo;
    }
    else
    {
        len = sizeof( data );
    }
    for ( i = 0; i < len; i++ )
    {
        data[ i ] = SPI_XferByte( 0 );
    }

    DeactivateSlave();
    TRIGGER_PORT &= ~TRIGGER_MASK;

    Log( "ReadData: Hi:0x%02x Lo:0x%02x Data", lenHi, lenLo );
    for ( i = 0; i < len; i++ )
    {
        Log( " 0x%02x", data[ i ]);
    }
    Log( "\n" );

} // WLN_ReadData

//***************************************************************************
/**
*   WLN_WriteByte
*/

static void WLN_WriteByte( uint8_t b )
{
    uint8_t lenHi;
    uint8_t lenLo;

    ActivateSlave();

    SPI_XferByte( WLN_CMD_WRITE_DATA );
    lenHi = SPI_XferByte( 0 );
    lenLo = SPI_XferByte( 0 );
    SPI_XferByte( b );

    DeactivateSlave();

    Log( "WriteData: LenHi:0x%02x LenLo:0x%02x Data:0x%02x\n", lenHi, lenLo, b );


} // WLN_WriteByte

//***************************************************************************
/**
*   main program
*/

int main( void )
{
    uint8_t delay;
    uint8_t counter;
    uint8_t doStatus = 1;

    InitTimer();
    InitUART();

    SetDirectionOut( LED );

    SetDirectionOut( MOSI );
    SetDirectionIn(  MISO );
    SetDirectionOut( SCK );
    SetDirectionOut( SS );
    SetDirectionIn(  INT );
    SetDirectionOut( TRIGGER );

    TRIGGER_PORT &= ~TRIGGER_MASK;

    SPI_MasterInit();

    Log( "*****\n" );
    Log( "***** WLN CLI program\n" );
    Log( "*****\n" );

    sei();

    counter = 0;
    while ( 1 )
    {
        LED_PORT ^= LED_MASK;

        if ( doStatus )
        {
            if ( counter == 0 )
            {
                uint8_t status = WLN_ReadStatus();
                WLN_PrintStatus( status );
            }
            counter = ( counter + 1 ) & 0x03;
        }

        // Tick rate is 100/sec so waiting for 50 waits for 1/2 sec

        for ( delay = 0; delay < 50; delay++ ) 
        {
            tick_t prevCount = gTickCount;
            while ( gTickCount == prevCount )
            {
                LogBufDump();
            }

            if ( UART0_IsCharAvailable() )
            {
                char    ch = UART0_GetChar();

                switch ( ch )
                {
                    case '!':
                    {
                        doStatus = !doStatus;
                        Log( "Turning status %s\n", doStatus ? "on" : "off" );
                        break;
                    }

                    case '*':
                    {
                        Log( "Reset\n" );

                        SetDirectionOut( RESET );
                        RESET_PORT &= ~RESET_MASK;
                        break;
                    }

                    case ' ':
                    {
                        WLN_ReadData();
                        break;
                    }

                    default:
                    {
                        Log( "Read: '%c'\n", ch );
                        WLN_WriteByte( ch );
                        break;
                    }
                }

            }
        }
    }

} // main

