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
*   @file   Hardware.h
*
*   @brief  Defines all of the hardware definitions for the chip.
*
****************************************************************************/

#if !defined( HARDWARE_H )
#define HARDWARE_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include "Config.h"

#include <avr/io.h>

//--------------------------------------------------------------------------
// LED Constants

#define RED_LED_PIN     1
#define RED_LED_MASK    ( 1 << RED_LED_PIN )
#define RED_LED_DDR     DDRD
#define RED_LED_PORT    PORTD

#define BLUE_LED_PIN    5
#define BLUE_LED_MASK   ( 1 << BLUE_LED_PIN )
#define BLUE_LED_DDR    DDRC
#define BLUE_LED_PORT   PORTC

//--------------------------------------------------------------------------
// Some convenience macros to turn the LEDs on/off.
//
//  Usage:  LED_ON( BLUE );
//
// to turn on the blue LED.
//
// Note: Setting the pin to 0 turns the LED on.

#define LED_ON( color )     do { color ## _LED_PORT |=  color ## _LED_MASK; } while (0)
#define LED_OFF( color )    do { color ## _LED_PORT &= ~color ## _LED_MASK; } while (0)
#define LED_TOGGLE( color ) do { color ## _LED_PORT ^=  color ## _LED_MASK; } while (0)

//--------------------------------------------------------------------------
// Port registers.
//
// Note: For input ports, writing a 1 to the PORT register casues an internal
//       pullup resistor to be activated. This feature also requires the 
//       PUD bit to be set in the SFIOR register.
//
// For the DDR pins, 0 = input, 1 = output

// Port B
//
//  7 - PCINT7 XTAL2 TOSC2  - connected to 20 MHz crystal
//  6 - PCINT6 XTAL1 TOSC1  - connected to 20 MHz crystal
//  5 - PCINT5 SCK
//  4 - PCINT4 MISO
//  3 - PCINT3 MOSI OC2A
//  2 - PCINT2 SS   OC1B    - IN3 on LB1836M
//  1 - PCINT1      OC1A    - IN1 on LB1836M
//  0 - PCINT0 CLKO ICP1

#define PORTB_INIT      0xFF    // Enable pullups for inputs
#define DDRB_INIT       0x00    // all inputs

// Port C
//
//  7 - N/A
//  6 - PCINT14 RESET
//  5 - PCINT13 ADC5    SCL
//  4 - PCINT12 ADC4    SDA
//  3 - PCINT11 ADC3
//  2 - PCINT10 ADC2
//  1 - PCINT9  ADC1
//  0 - PCINT8  ADC1

#define PORTC_INIT      0xFF    // enable pullups for all inputs
#define DDRC_INIT       BLUE_LED_MASK

// Port D
//
//  7 - PCINT23 AIN1
//  6 - PCINT22 AIN0    OC0A
//  5 - PCINT21 T1      OC0B
//  4 - PCINT20 T0      XCK
//  3 - PCINT19 INT1    OC2B
//  2 - PCINT18 INT0
//  1 - PCINT17 TXD             - Red LED
//  0 - PCINT16 RXD

#define PORTD_INIT      0xFF    // enable pullups for all inputs
#define DDRD_INIT       RED_LED_MASK

//--------------------------------------------------------------------------
// ASSR - Asynchronous Status Register

#define ASSR_INIT   0x00

//--------------------------------------------------------------------------
// ADC Settings

#define ADC_PRESCALAR_2     (( 0 << ADPS2 ) | ( 0 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_4     (( 0 << ADPS2 ) | ( 1 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_8     (( 0 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_16    (( 1 << ADPS2 ) | ( 0 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_32    (( 1 << ADPS2 ) | ( 0 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_64    (( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_128   (( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ))

#define ADCSR_INIT  (( 1 << ADEN ) | ( 1 << ADSC ) | ADC_PRESCALAR_128 )

#define ADMUX_REF_AREF      (( 0 << REFS1 ) | ( 0 << REFS0 ))
#define ADMUX_REF_AVCC      (( 0 << REFS1 ) | ( 1 << REFS0 ))
#define ADMUX_REF_INTERNAL  (( 1 << REFS1 ) | ( 1 << REFS0 ))

#define ADMUX_INIT  ADMUX_REF_AVCC

//--------------------------------------------------------------------------
// UART settings

#define UART0_BAUD_RATE     38400

#define UART_DATA_BIT_8     (( 1 << UCSZ01 ) | ( 1 << UCSZ00 ))
#define UART_PARITY_NONE    (( 0 << UPM01 )  | ( 0 << UPM00 ))
#define UART_STOP_BIT_1     ( 0 << USBS0 )

#define UBRR0_INIT      (( CFG_CPU_CLOCK / 16 / UART0_BAUD_RATE ) - 1 )

#define UCSR0A_INIT     0
#define UCSR0B_INIT     (( 1 << RXCIE0 ) | ( 1 << RXEN0 ) | ( 1 << TXEN0 ))
#define UCSR0C_INIT     ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 )

/* ---- Variable Externs ------------------------------------------------- */

/**
 *  Description of variable.
 */

/* ---- Function Prototypes ---------------------------------------------- */

void InitHardware( void );
 

/** @} */

#endif // HARDWARE_H

