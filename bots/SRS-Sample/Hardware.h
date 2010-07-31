/****************************************************************************
*
*   Copyright (c) 2005 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
#include "Timer.h"

#include <avr/io.h>

//--------------------------------------------------------------------------
// LED Constants

#define GREEN_LED_PIN     4
#define GREEN_LED_MASK    ( 1 << GREEN_LED_PIN )
#define GREEN_LED_DDR     DDRB
#define GREEN_LED_PORT    PORTB

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
#define LED_TOGGLE( color ) do { color ## _LED_PORT ^= color ## _LED_MASK; } while (0)

//--------------------------------------------------------------------------
//  Encoder Inputs
//

#define ENCODER_R_A_PIN     PIND   // INT0 = Port D pin 2
#define ENCODER_R_A_BIT     2
#define ENCODER_R_A_MASK    ( 1 << ENCODER_R_A_BIT )

#define ENCODER_L_A_PIN     PIND   // INT1 = Port D pin 3
#define ENCODER_L_A_BIT     3
#define ENCODER_L_A_MASK    ( 1 << ENCODER_L_A_BIT )

#define ENCODER_R_B_PIN     PINC
#define ENCODER_R_B_BIT     5
#define ENCODER_R_B_MASK    ( 1 << ENCODER_R_B_BIT )

#define ENCODER_L_B_PIN     PINC   
#define ENCODER_L_B_BIT     2
#define ENCODER_L_B_MASK    ( 1 << ENCODER_L_B_BIT )

//--------------------------------------------------------------------------
// Port registers.
//
// Note: For input ports, writing a 1 to the PORT register casues an internal
//       pullup resistor to be activated. This feature also requires the 
//       PUD bit to be set in the SFIOR register.
//
// For the DDR pins, 0 = input, 1 = output
//
// Port A corresponds to the ADC pins
//
//  7 - ADC7            Right Eye
//  6 - ADC6            Expansion Board - Battery Voltage
//  5 - ADC5
//  4 - ADC4            
//  3 - ADC3            Right Floor
//  2 - ADC2            Center Floor
//  1 - ADC1            Left Floor
//  0 - ADC0            Left Eye

#define PORTA_INIT      0x00
#define DDRA_INIT       0x00

// Port B
//
//  7 - SCK
//  6 - MISO
//  5 - MOSI
//  4 - SS              Main Board - Green LED
//  3 - OC0/AIN1        Expansion Board - LCD Enable
//  2 - INT2/AIN0
//  1 - T1              Expansion Board - Load (for outputs) (+ve edge)
//  0 - XCK/TO          Expansion Board - SHIFT /LOAD (for inputs)

#define EXP_INIT        0xab    // B0, B1, B3, B5, and B7 are outputs

#define PORTB_INIT      0xFF & ~EXP_INIT   // Enable pullups for inputs.
#define DDRB_INIT       GREEN_LED_MASK  | EXP_INIT

// Port C
//
//  7 - SCL
//  6 - SDA
//  5 - TCK             Right Wheel Encoder - ChB
//  4 - TMS             Right Motor Dir
//  3 - TDO             Left  Motor Dir
//  2 - TDI             Left  Wheel Encoder - ChB
//  1 - TOSC1
//  0 - TOSC2

#define MOTOR_R_DIR     4
#define MOTOR_L_DIR     3

#define PORTC_INIT      0xC3    // pullup unused input
#define DDRC_INIT       ( 1 << MOTOR_R_DIR ) | ( 1 << MOTOR_L_DIR )

// Port D
//
//  7 - OC2             Expansion Board - Speaker
//  6 - ICP1            Expansion Board - SW3
//  5 - OC1A            Right Motor PWM
//  4 - OC1B            Left  Motor PWM
//  3 - INT1            Left  Wheel Encoder - ChA
//  2 - INT0            Right Wheel Encoder - ChA
//  1 - TXD             Serial Port - TxD
//  0 - RXD             Serial Port - RxD

#define MOTOR_R_PWM     5
#define MOTOR_L_PWM     4

#define PORTD_INIT      ( 1 << 6 )    // Pullup SW3
#define DDRD_INIT       ( 1 << MOTOR_R_PWM ) | ( 1 << MOTOR_L_PWM )

//--------------------------------------------------------------------------
// MCUCR - MCU Control Register
//

#define MCUCR_INT0_LOW_LEVEL    ( 0 << ISC01 ) | ( 0 << ISC00 )
#define MCUCR_INT0_ANY_CHANGE   ( 0 << ISC01 ) | ( 1 << ISC00 )
#define MCUCR_INT0_FALLING_EDGE ( 1 << ISC01 ) | ( 0 << ISC00 )
#define MCUCR_INT0_RISING_EDGE  ( 1 << ISC01 ) | ( 1 << ISC00 )

#define MCUCR_INT1_LOW_LEVEL    ( 0 << ISC11 ) | ( 0 << ISC10 )
#define MCUCR_INT1_ANY_CHANGE   ( 0 << ISC11 ) | ( 1 << ISC10 )
#define MCUCR_INT1_FALLING_EDGE ( 1 << ISC11 ) | ( 0 << ISC10 )
#define MCUCR_INT1_RISING_EDGE  ( 1 << ISC11 ) | ( 1 << ISC10 )

#define MCUCR_INIT  MCUCR_INT0_RISING_EDGE | MCUCR_INT1_RISING_EDGE

//--------------------------------------------------------------------------
// MCUCSR - MCU Control and Status Register
//

// In order to use PortC pins 2, 3, 4, and 5 the JTAG needs to be disabled.
// So, we set the JTD bit in MCUCSR to disable the JTAG.

#define MCUCSR_INIT     ( 1 << JTD )

//--------------------------------------------------------------------------
// GICR - Global Interrupt Control Register
//

#define GICR_INIT   0

//--------------------------------------------------------------------------
// Timer 1
//

#define CHAN_A_NON_INVERTING_PWM    ( 1 << COM1A1 ) | ( 0 << COM1A0 )
#define CHAN_B_NON_INVERTING_PWM    ( 1 << COM1B1 ) | ( 0 << COM1B0 )
#define PWM_8_BIT_MODE              ( 0 << WGM11 )  | ( 1 << WGM10 )

#define TCCR1A_INIT     CHAN_A_NON_INVERTING_PWM | CHAN_B_NON_INVERTING_PWM | PWM_8_BIT_MODE
#define TCCR1B_INIT     TIMER1_CLOCK_SEL_DIV_8

#define OCR1A_INIT      0
#define OCR1B_INIT      0

//--------------------------------------------------------------------------
// ASSR - Asynchronous Status Register
//

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

//--------------------------------------------------------------------------
// UART settings

#define UART0_BAUD_RATE     38400

#define UART_DATA_BIT_8  (( 1 << UCSZ1 ) | ( 1 << UCSZ0 ))
#define UART_PARITY_NONE (( 0 << UPM1 )  | ( 0 << UPM0 ))
#define UART_STOP_BIT_1  ( 1 << USBS )

#define UBRR0_INIT   (( CFG_CPU_CLOCK / 16 / UART0_BAUD_RATE ) - 1 )

#define UCSR0A_INIT  0
#define UCSR0B_INIT  (( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ))
#define UCSR0C_INIT  (( 1 << URSEL ) | UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 )

//--------------------------------------------------------------------------
// SPI settings
//
//  SPCR - SPI Control Register (0101 0001)
//
//		bit 7	SPIE (interrupt enable)	0 = not enabled/
//		bit 6	SPE (enable)		    1 = enable
//		bit 5	DORD (data order)	    0 = MSB transmitted first
//		bit 4	MSTR (master sel.)	    1 = master
//		bit 3	CPOL (clk polarity)	    0 = idle low (pulse goes high, then low)
//		bit 2	CPHA (clock phase)	    0 = sample data on leading edge of pulse
//		bit 1-0	SPR1/0 (clock rate)	    01 = fOSC/16

#define SPCR_INIT   0x51

/* ---- Variable Externs ------------------------------------------------- */

/**
 *  Description of variable.
 */

/* ---- Function Prototypes ---------------------------------------------- */

void InitHardware( void );
 

/** @} */

#endif // HARDWARE_H

