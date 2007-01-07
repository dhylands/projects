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
*   @file   tiny45-tach.c
*
*   @brief  Simple Tachometer using a Tiny45 and an HC4LED display module
*           purchased from SparkFun.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "CBUF.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define MOSI_DDR        DDRB
#define MOSI_PORT       PORTB
#define MOSI_MASK       ( 1 << 0 )

#define SCK_DDR         DDRB
#define SCK_PORT        PORTB
#define SCK_MASK        ( 1 << 2 )

#define LED_DDR         DDRB
#define LED_PORT        PORTB
#define LED_MASK        ( 1 << 3 )

#define TACH_LED_DDR    DDRB
#define TACH_LED_PORT   PORTB
#define TACH_LED_MASK   ( 1 << 1 )

#define TACH_DDR        DDRB
#define TACH_PIN        PINB
#define TACH_PORT       PORTB
#define TACH_MASK       ( 1 << 4 )

#define TACH_NUM_ENTRIES    16  // Must be power of 2

typedef struct
{
    uint8_t     m_getIdx;
    uint8_t     m_putIdx;
    uint32_t    m_entry[ TACH_NUM_ENTRIES ];

} TACH_Buffer_t;

#define TACH_gBuffer_SIZE   TACH_NUM_ENTRIES

/* ---- Private Variables ------------------------------------------------ */

// This is the layout for each digit
//
//  --5--
// |     |
// 6     4
// |     |
//  --7--
// |     |
// 1     3
// |     |
//  --2--
//
// where the number is the bit position within the byte.

#define S1  0x02
#define S2  0x04
#define S3  0x08
#define S4  0x10
#define S5  0x20
#define S6  0x40
#define S7  0x80

uint8_t gNumBits[ 10 ] =
{
    S1 | S2 | S3 | S4 | S5 | S6     ,   // 0
              S3 | S4               ,   // 1
    S1 | S2      | S4 | S5      | S7,   // 2
         S2 | S3 | S4 | S5      | S7,   // 3
              S3 | S4      | S6 | S7,   // 4
         S2 | S3      | S5 | S6 | S7,   // 5
    S1 | S2 | S3      | S5 | S6 | S7,   // 6
              S3 | S4 | S5          ,   // 7
    S1 | S2 | S3 | S4 | S5 | S6 | S7,   // 8
         S2 | S3 | S4 | S5 | S6 | S7,   // 9
};

volatile    TACH_Buffer_t   TACH_gBuffer;

volatile    uint16_t    gOverflowCount;

/* ---- Functions -------------------------------------------------------- */

#define LOOPS_PER_MS ( CFG_CPU_CLOCK / 1000 / 4 )

//***************************************************************************
/**
*   Spin for ms milliseconds
*/

static void ms_spin( unsigned short ms )
{
    if ( ms == 0 )
    {
        return;
    }

    // the inner loop takes 4 cycles per iteration

    __asm__ __volatile__ (
            "1:                     \n"
            "       ldi r26, %3     \n"
            "       ldi r27, %2     \n"
            "2:     sbiw r26, 1     \n"
            "       brne 2b         \n"
            "       sbiw %0, 1      \n"
            "       brne 1b         \n"
            : "=w" (ms)
            : "w" (ms), "i" (LOOPS_PER_MS >> 8), "i" (0xff & LOOPS_PER_MS)
            );
} // ms_spin

//***************************************************************************
/**
*   Interrupt handler for Timer 0 overflow.
*/

ISR( TIM0_OVF_vect )
{
    gOverflowCount++;

} // TIM0_OVF_vect

//***************************************************************************
/**
*   Retrieves the value of the current timer. It is assumed that this 
*   function is called with interrupts disabled.
*/

static inline uint32_t GetTimer( void )
{
    uint8_t     tcnt;
    uint16_t    overflowCount;
    uint32_t    timerCount;

    tcnt = TCNT0;
    overflowCount = gOverflowCount;
    if (( TIFR & ( 1 << TOV0 )) != 0 )
    {
        // Timer 0 overflowed between us disabling interrupts and our
        // check

        tcnt = TCNT0;
        overflowCount++;
    }
    timerCount = ( overflowCount << 8 ) + tcnt;

    return timerCount;

} // GetTimer

//***************************************************************************
/**
*   Pin change interrupt handler
*/

ISR( PCINT0_vect )
{
    if (( TACH_PIN & TACH_MASK ) != 0 )
    {
        uint32_t    t = GetTimer();

        // Rising edge on the tach

        TACH_LED_PORT |= TACH_LED_MASK;


        if ( CBUF_Len( TACH_gBuffer ) >= TACH_NUM_ENTRIES )
        {
            CBUF_Pop( TACH_gBuffer );
        }
        CBUF_Push( TACH_gBuffer, t );
    }
    else
    {
        TACH_LED_PORT &= ~TACH_LED_MASK;
    }

} // PCINT0_vect

//***************************************************************************
/**
*   Write a byte out to the LED display
*/

static void PutByte( uint8_t b )
{
    uint8_t i;
    uint8_t mask;

    mask = 1;
    for ( i = 0; i < 8; i++ ) 
    {
        SCK_PORT &= ~SCK_MASK;    // Clock low

        if (( b & mask ) == 0 )
        {
            MOSI_PORT &= ~MOSI_MASK;
        }
        else
        {
            MOSI_PORT |= MOSI_MASK;
        }
        SCK_PORT |= SCK_MASK;    // Clock high

        mask <<= 1;
    }

} // PutByte

//***************************************************************************
/**
*   Writes a number as 4 digits
*/

static void PutNum( uint16_t num )
{
    // We need to write the 1's first

    PutByte( gNumBits[   num          % 10 ]);

    if ( num > 9 )
    {
        PutByte( gNumBits[ ( num /   10 ) % 10 ]);
    }
    else
    {
        PutByte( 0 );
    }

    if ( num > 99 )
    {
        PutByte( gNumBits[ ( num /  100 ) % 10 ]);
    }
    else
    {
        PutByte( 0 );
    }

    if ( num > 999 )
    {
        PutByte( gNumBits[ ( num / 1000 ) % 10 ]);
    }
    else
    {
        PutByte( 0 );
    }

} // PutNum

//***************************************************************************
/**
*   Tachometer.
*/

int main( void )
{
    uint16_t    num;
    uint32_t    t1;
    uint32_t    t2;
    uint8_t     numSamples;

    // Setup the USI module to be an SPI Master. The HC4LED display only has
    // a clock and data-in line, so it's a write-only interface

    MOSI_DDR    |= MOSI_MASK;
    SCK_DDR     |= SCK_MASK;

    // Initialize the hearbeat LED pin

    LED_DDR      |= LED_MASK;
    TACH_LED_DDR |= TACH_LED_MASK;

    // Initialize the Tach input pin

    TACH_DDR  &= ~TACH_MASK;
    TACH_PORT |=  TACH_MASK;

    // Initialize Timer 0 to count clock ticks

    TCCR0A = 0; // COM = 0, WGM = 0
    TCCR0B = ( 0 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ); // Divide by 1 prescalar

    GIMSK |= ( 1 << PCIE );
    PCMSK |= TACH_MASK;

    sei();

    while ( 1 )
    {
        ms_spin( 500 );

        LED_PORT ^= LED_MASK;

        cli();
        if (( numSamples = CBUF_Len( TACH_gBuffer )) >= 2 )
        {
            uint8_t     i;
            uint32_t    old_time = t1 - CFG_CPU_CLOCK;

            t1 = CBUF_GetEnd( TACH_gBuffer, 0 );
            t2 = CBUF_GetEnd( TACH_gBuffer, 1 );
            
            for ( i = 2; i < numSamples; i++ ) 
            {
                uint32_t    t = CBUF_GetEnd( TACH_gBuffer, i );

                if ( t < old_time )
                {
                    break;
                }
                t2 = t;
            }
        }
        sei();

        if ( numSamples >= 2 )
        {
            
        }

        else
        {
            PutByte( S7 );
            PutByte( S7 );
            PutByte( S7 );
            PutByte( S7 );
        }
    }

} // main

