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
****************************************************************************
*
*   Test program for monitoring the ADC lines.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>

#include "hw_types.h"
#include "hw_memmap.h"

#include "gpio.h"
#include "sysctl.h"
#include "systick.h"

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define USER_LED                GPIO_PIN_5

/* ---- Private Variables ------------------------------------------------ */

volatile    uint32_t    gTickCount;

/* ---- Private Function Prototypes -------------------------------------- */

void SysTick_isr(void) __attribute__((__interrupt__));

/* ---- Functions -------------------------------------------------------- */

/****************************************************************************
*
*   Interrupt handler which fires when the system tick
*
****************************************************************************/

void SysTick_isr( void )
{
    gTickCount++;

} // SysTick_isr

/****************************************************************************
*
*   Main loop for the test program
*
****************************************************************************/

int main( void )
{
    uint32_t    prevTick;

    // Set the clocking to run at 20MHz from the PLL.

    SysCtlClockSet( SYSCTL_SYSDIV_10 
                  | SYSCTL_USE_PLL 
                  | SYSCTL_OSC_MAIN 
                  | SYSCTL_XTAL_6MHZ );

    // Configure the LED

    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOC );
    GPIODirModeSet( GPIO_PORTC_BASE, USER_LED, GPIO_DIR_MODE_OUT );
    GPIOPinWrite( GPIO_PORTC_BASE, USER_LED, 0 );

    // Configure the system tick to go off once every millisecond

    SysTickPeriodSet( SysCtlClockGet() / 1000 );
    SysTickEnable();
    SysTickIntEnable();

    while ( 1 )
    {
        prevTick = gTickCount;

        while (( gTickCount - prevTick ) < 50 )
        {
            ;
        }

        GPIOPinWrite( GPIO_PORTC_BASE, USER_LED,
                      GPIOPinRead( GPIO_PORTC_BASE, USER_LED ) ^ USER_LED );
    }

} // main

