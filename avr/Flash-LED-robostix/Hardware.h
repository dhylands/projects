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

#define RED_LED_PIN     4
#define RED_LED_MASK    ( 1 << RED_LED_PIN )
#define RED_LED_DDR     DDRG
#define RED_LED_PORT    PORTG

#define BLUE_LED_PIN    3
#define BLUE_LED_MASK   ( 1 << BLUE_LED_PIN )
#define BLUE_LED_DDR    DDRG
#define BLUE_LED_PORT   PORTG

#define YELLOW_LED_PIN  4
#define YELLOW_LED_MASK ( 1 << YELLOW_LED_PIN )
#define YELLOW_LED_DDR  DDRB
#define YELLOW_LED_PORT PORTB

//--------------------------------------------------------------------------
// Some convenience macros to turn the LEDs on/off.
//
//  Usage:  LED_ON( BLUE );
//
// to turn on the blue LED.
//
// Note: Setting the pin to 0 turns the LED on.

#define LED_ON( color )     do { color ## _LED_PORT &= ~color ## _LED_MASK; } while (0)
#define LED_OFF( color )    do { color ## _LED_PORT |= color ## _LED_MASK; } while (0)

//--------------------------------------------------------------------------
// Port registers.
//
// Note: For input ports, writing a 1 to the PORT register casues an internal
//       pullup resistor to be activated. This feature also requires the 
//       PUD bit to be set in the SFIOR register.
//
// For the DDR pins, 0 = input, 1 = output

// Port A if the one that's just pads on the bottom of the board

#define PORTA_INIT      0xFF
#define DDRA_INIT       0x00

// Port B has 3 PWM lines, and SPI. Pin 4 is connected to the Yellow LED
//
//  7 - OC2     ATM_PWM1C
//  6 - OC1B    ATM_PWM1B
//  5 - OC1A    ATM_PWM1A
//  4 - OC0     PB4 (Yellow LED)
//  3 - MISO    ATM_MISO
//  2 - MOSI    ATM_MOSI
//  1 - SCK     ATM_SCK
//  0 - SS      ATM_SS

#define PORTB_INIT      0xFF    // Enable pullups for inputs, Yellow LED off
#define DDRB_INIT       YELLOW_LED_MASK

// Port C is available on the headers as 8 I/O lines. We'll assume all inputs
// for now.

#define PORTC_INIT      0xFF    // enable pullups for all inputs
#define DDRC_INIT       0x00

// Port D
//
//  7 - T2      ATM_T2
//  6 - T1      ATM_T1
//  5 - PD5     ATM_PD5
//  4 - IC1     ATM_IC1
//  3 - INT3    ATM_TX1
//  2 - INT2    ATM_RX1
//  1 - INT1    ATM_SDA
//  0 - INT0    ATM_SCL

#define PORTD_INIT      0xFF    // enable pullups for all inputs
#define DDRD_INIT       0x00

// Port E
//
//  7 - INT7    ATM_INT7
//  6 - INT6    ATM_INT6
//  5 - OC3C    ATM_PWM3C
//  4 - OC3B    ATM_PWM3B
//  3 - OC3A    ATM_PWM3A
//  2 - PE2     ATM_IRQ     Used to interrupt gumstix
//  1 - TXD     ATM_TX0
//  0 - RXD     ATM_RX0

#define PORTE_INIT      0xFF    // enable pullups for all inputs
#define DDRE_INIT       0x00

// Port F - A/D lines - could be used as GPIO
//
// We disable all pullups by default so they don't mess with the A/D

#define PORTF_INIT      0x00    // disable pullups for A/D channels
#define DDRF_INIT       0x00

// Port G
//
//  7 - N/A
//  6 - N/A
//  5 - N/A
//  4 - TOSC1   ATM_PG4 - Red LED
//  3 - TOSC2   ATM_PG3 - Blue LED
//  2 - ALE     ATM_PG2
//  1 - /RD     ATM_PG1
//  0 - /WR     ATM_PG0

#define PORTG_INIT  0xFF    // Enable pullups for inputs, LEDs off
#define DDRG_INIT   ( RED_LED_MASK | BLUE_LED_MASK )    // Make LEDs outputs

//--------------------------------------------------------------------------
// ASSR - Asynchronous Status Register
//
// TOSC1 & TOSC2 are connected to the Red/Blue LEDs so we need to set
// AS0 to 0. AS0 is the only writable bit.

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
#define UART1_BAUD_RATE     38400

#define UART_DATA_BIT_8  (( 1 << UCSZ1 ) | ( 1 << UCSZ0 ))
#define UART_PARITY_NONE (( 0 << UPM1 )  | ( 0 << UPM0 ))
#define UART_STOP_BIT_1  ( 1 << USBS )

#define UBRR0_INIT   (( CFG_CPU_CLOCK / 16 / UART0_BAUD_RATE ) - 1 )
#define UBRR1_INIT   (( CFG_CPU_CLOCK / 16 / UART1_BAUD_RATE ) - 1 )

#define UCSR0A_INIT  0
#define UCSR0B_INIT  (( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ))
#define UCSR0C_INIT  ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 )

#define UCSR1A_INIT  0
#define UCSR1B_INIT  (( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ))
#define UCSR1C_INIT  ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 )

/* ---- Variable Externs ------------------------------------------------- */

/**
 *  Description of variable.
 */

/* ---- Function Prototypes ---------------------------------------------- */

void InitHardware( void );
 

/** @} */

#endif // HARDWARE_H

