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

int main(void)
{
    InitTimer();

    LED_DDR |= LED_MASK;

    LCD_Init( 2, 16 );

    LCD_PutStr( "LCD Test\n" );
    LCD_PutStr( ">Line 2<\n" );

    while( 1 )
    {
        LED_PORT &= ~LED_MASK;

        ms_spin( 100 );

        LED_PORT |=  LED_MASK;

        ms_spin( 100 );
    }

    return 0;
}

