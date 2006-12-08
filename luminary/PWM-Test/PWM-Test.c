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
#include "hw_ints.h"
#include "hw_memmap.h"

#include "adc.h"
#include "gpio.h"
#include "interrupt.h"
#include "pwm.h"
#include "sysctl.h"
#include "timer.h"
#include "uart.h"

#include "lcd.h"
#include "Str.h"
#include "Switch.h"
                 
/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define PUSH_BUTTON             GPIO_PIN_4
#define USER_LED                GPIO_PIN_5

/* ---- Private Variables ------------------------------------------------ */

unsigned	long	gADC[ 4 ];

unsigned    long    gFlags;

int                 gRunMotors;

#define FLAG_ADC_SAMPLES_AVAIL  0

uint32_t    gMotorPeriod;

/* ---- Private Function Prototypes -------------------------------------- */

void PutStr( const char *str );
void ADC_isr( void );
void InitADC( void );
void InitMotors( void );

/* ---- Functions -------------------------------------------------------- */

/****************************************************************************
*
*   PutStr
*
****************************************************************************/

void PutStr( const char *str )
{
    while ( *str != '\0' )
    {
        if ( *str == '\n' )
        {
            UARTCharPut( UART0_BASE, '\r' );
        }
        UARTCharPut( UART0_BASE, *str );

        str++;
    }
}

/****************************************************************************
*
*   Interrupt handler which fires when a sequence has been collected.
*
****************************************************************************/

void ADC_isr( void )
{
    // Clear the interrupt

    ADCIntClear( ADC_BASE, 0 );

    // Grab the data from the ADC

    ADCSequenceDataGet( ADC_BASE, 0, &gADC[ 0 ]);

    HWREGBITW( &gFlags, FLAG_ADC_SAMPLES_AVAIL ) = 1;

} // ADC_isr

/****************************************************************************
*
*   Initialize the ADC to collect sample from our 4 line sensors.
*
****************************************************************************/

void InitADC( void )
{
    // Enable the ADC peripheral

    SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC );

    // Setup the sequence

    //
    // Configure the ADC to sample the potentiometer when the timer expires.
    // After sampling, the ADC will interrupt the processor; this is used as
    // the heartbeat for the game.
    //
    ADCSequenceConfigure( ADC_BASE,             // base
                          0,                    // sequence num
                          ADC_TRIGGER_TIMER,    // trigger
                          0 );                  // priority

    ADCSequenceStepConfigure( ADC_BASE,         // base
                              0,                // sequence num
                              0,                // step num
                              ADC_CTL_CH0 );    // config

    ADCSequenceStepConfigure( ADC_BASE,         // base
                              0,                // sequence num
                              1,                // step num
                              ADC_CTL_CH1 );    // config

    ADCSequenceStepConfigure( ADC_BASE,         // base
                              0,                // sequence num
                              2,                // step num
                              ADC_CTL_CH2 );    // config

    ADCSequenceStepConfigure( ADC_BASE,         // base
                              0,                // sequence num
                              3,                // step num
                              ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END ); //config

    ADCSequenceEnable( ADC_BASE, 0 );

    ADCIntEnable( ADC_BASE, 0);
    IntEnable(INT_ADC0);

} // InitADC

/****************************************************************************
*
*   Initialize the PWM channels for driving a pair of motors
*
****************************************************************************/

void InitMotors( void )
{
    // Enable the peripherals that we need

    // PWM 0/1 is used for the left motor, and PWM 2/3 is used for the righr
    // motor.
    //
    // PWM 0/1 is found on Port D 0/1
    // PWM 2/3 is found on Port B 0/1

    SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOB );

    // Configure PWM 0/1 pins as output

    GPIOPinTypePWM( GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    // Configure PWM 2/3 pins as output

    GPIOPinTypePWM( GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    // Set the PWM period.

    gMotorPeriod = SysCtlClockGet() / 4096; // 4 kHz

    PWMGenConfigure( PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC );
    PWMGenConfigure( PWM_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC );

    PWMGenPeriodSet( PWM_BASE, PWM_GEN_0, gMotorPeriod );
    PWMGenPeriodSet( PWM_BASE, PWM_GEN_1, gMotorPeriod );

    PWMPulseWidthSet( PWM_BASE, PWM_OUT_0, 0 );
    PWMPulseWidthSet( PWM_BASE, PWM_OUT_1, 0 );

    PWMPulseWidthSet( PWM_BASE, PWM_OUT_2, 0 );
    PWMPulseWidthSet( PWM_BASE, PWM_OUT_3, 0 );

    // Enable the PWM output signals

    PWMOutputState( PWM_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, true );

    // Enable the PWM generators

    PWMGenEnable( PWM_BASE, PWM_GEN_0 );
    PWMGenEnable( PWM_BASE, PWM_GEN_1 );

} // InitMotor

/****************************************************************************
*
*   SwitchEvent
*
****************************************************************************/

void SwitchEvent( SwitchNum_t switchNum, SwitchEvent_t event )
{
    if ( event == SWITCH_EVENT_PRESSED )
    {
        gRunMotors = !gRunMotors;
    }

} // SwitchEvent

/****************************************************************************
*
*   Main loop for the test program
*
****************************************************************************/

int main( void )
{
    int         spinIdx = 0;
    const char *spinner = "-\\|/";
    char        buf[ 20 ];
    uint32_t    lastTimerVal;
    uint32_t    tenMsec;

    // Set the clocking to run at 20MHz from the PLL.

    SysCtlClockSet( SYSCTL_SYSDIV_10 
                  | SYSCTL_USE_PLL 
                  | SYSCTL_OSC_MAIN 
                  | SYSCTL_XTAL_6MHZ );

    LCD_Init( 2, 16 );
    LCD_PutStr( "PWM-Test" );

	InitADC();
    InitMotors();

    // Configure Timer0 to generate a 10 kHz PWM signal for driving
    // the user LED.

    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER0 );
    TimerConfigure( TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PWM );
    TimerLoadSet(TIMER0_BASE, TIMER_B, (SysCtlClockGet() / 10000) - 1 );
    TimerMatchSet( TIMER0_BASE, TIMER_B, 0 );
    TimerControlLevel( TIMER0_BASE, TIMER_B, true );
    TimerEnable( TIMER0_BASE, TIMER_B );

    // Confgure Timer 1 to generate triggers for the ADC

    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER1 );

    HWREG( ADC_BASE + 0x30 ) = 6;

    TimerConfigure( TIMER1_BASE, TIMER_CFG_32_BIT_PER );
    TimerLoadSet( TIMER1_BASE, TIMER_A, SysCtlClockGet() / 20 );
    TimerControlStall( TIMER1_BASE, TIMER_A, true );
    TimerControlTrigger( TIMER1_BASE, TIMER_A, true );
    TimerEnable( TIMER1_BASE, TIMER_A );

    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOA );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_UART0 );

    // Configure the first UART for 115,200, 8-N-1 operation.

    GPIODirModeSet( GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_HW );

    UARTConfigSet( UART0_BASE, 
                   115200,
                   ( UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE ));
    UARTEnable( UART0_BASE );

    // Setup the User LED & pushbutton

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIODirModeSet( GPIO_PORTC_BASE, USER_LED, GPIO_DIR_MODE_OUT );
    GPIOPinWrite( GPIO_PORTC_BASE, USER_LED, 0 );

    EnableSwitch( 0 );

    PutStr( "*****\n" );
    PutStr( "***** PWM-Test program\n" );
    PutStr( "*****\n" );

    TimerMatchSet( TIMER0_BASE, TIMER_B, 0 );
    GPIODirModeSet( GPIO_PORTC_BASE, USER_LED, GPIO_DIR_MODE_HW );

    tenMsec = SysCtlClockGet() / 100;
    lastTimerVal = TimerValueGet( TIMER1_BASE, TIMER_A );

    while ( 1 )
    {
        uint32_t    pulseWidth;
        uint32_t    pulsePct;

        while ( HWREGBITW( &gFlags, FLAG_ADC_SAMPLES_AVAIL ) == 0 )
        {
            uint32_t    currTimerVal = TimerValueGet( TIMER1_BASE, TIMER_A );

            if (( currTimerVal - lastTimerVal ) > tenMsec )
            {
                SetRawSwitch( 0, GPIOPinRead( GPIO_PORTC_BASE, PUSH_BUTTON ) != 0 );
                CheckSwitches();

                lastTimerVal = currTimerVal;
            }
        }
        HWREGBITW( &gFlags, FLAG_ADC_SAMPLES_AVAIL ) = 0;

        StrPrintf( buf, sizeof( buf ), "%c PWM-Test", spinner[ spinIdx ]);

        LCD_MoveTo( 0, 0 );
        LCD_PutStr( buf );

        PutStr( "\r" );
        PutStr(  buf );

        if ( gRunMotors )
        {
            pulseWidth = gADC[ 0 ] * gMotorPeriod / 1023;
            pulsePct   = gADC[ 0 ] * 100 / 1023;

            StrPrintf( buf, sizeof( buf ), "%3lx %3d%%    ", gADC[0], pulsePct );
        }
        else
        {
            pulseWidth = 0;

            StrPrintf( buf, sizeof( buf ), "Motors Off" );
        }

        PutStr( " " );
        PutStr( buf );

        LCD_MoveTo( 0, 1 );
        LCD_PutStr( buf );

        PWMPulseWidthSet( PWM_BASE, PWM_OUT_0, pulseWidth );
        PWMPulseWidthSet( PWM_BASE, PWM_OUT_1, pulseWidth );
        PWMPulseWidthSet( PWM_BASE, PWM_OUT_2, pulseWidth );
        PWMPulseWidthSet( PWM_BASE, PWM_OUT_3, pulseWidth );

        spinIdx = ( spinIdx + 1 ) % 4;
    }

} // main

