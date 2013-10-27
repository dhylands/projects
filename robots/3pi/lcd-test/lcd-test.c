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

#include <avr/io.h>

#include "lcd.h"
#include "Delay.h"
#include "Timer.h"
#include "UART.h"

#define SW_A  (1<<1)
#define SW_B  (1<<4)
#define SW_C  (1<<5)
#define SW_ALL  (SW_A | SW_B | SW_C)

int main(void)
{
    InitTimer();
    InitUART();

    LED_DDR |= LED_MASK;

    LCD_Init( CFG_LCD_NUM_LINES, CFG_LCD_NUM_COLUMNS );

    LCD_PutStr( "LCD Test\n" );
    LCD_PutStr( ">Line 2<\n" );

    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );
    printf( "*****\n" );
    printf( "***** LCD Test program\n" );
    printf( "*****\n" );

    DDRB &= ~SW_ALL;
    PORTB |= SW_ALL;

    while( 1 )
    {
        LED_PORT |=  LED_MASK;

        ms_spin( 100 );

        LED_PORT &= ~LED_MASK;

        ms_spin( 100 );

        LED_PORT |=  LED_MASK;

        ms_spin( 100 );

        LED_PORT &= ~LED_MASK;

        ms_spin( 700 );

        LCD_MoveTo(0,1);
        uint8_t pinb = PINB;
        if ( pinb & SW_A )
        {
            printf("A is high ");
            LCD_PutChar('a');
        }
        else
        {
            printf("A is low ");
            LCD_PutChar('A');
        }
        if ( pinb & SW_B )
        {
            printf("B is high ");
            LCD_PutChar('b');
        }
        else
        {
            printf("B is low ");
            LCD_PutChar('B');
        }
        if ( pinb & SW_C )
        {
            printf("C is high\n");
            LCD_PutChar('c');
        }
        else
        {
            printf("C is low\n");
            LCD_PutChar('C');
        }
    }

    return 0;
}

