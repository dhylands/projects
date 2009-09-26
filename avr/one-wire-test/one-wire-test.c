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

#include "one-wire.h"
#include "Delay.h"
#include "Led.h"
#include "Timer.h"
#include "UART.h"

int main(void)
{
    InitTimer();
    InitUART();
    OW_Init();

    LED_Init( HEARTBEAT );
    LED_Off( HEARTBEAT );

    while( 1 )
    {
        LED_Toggle( HEARTBEAT );

        Delay1mSec( 100 );
    }

    return 0;
}

