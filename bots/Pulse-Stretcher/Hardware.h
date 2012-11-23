/****************************************************************************
*
*  Hardware.h
*
*   This header file contains all of the hardware definitions for the chip.
*   By centralizing everything in one header file, it allows us to keep
*   track of what's being used by whom.
*
****************************************************************************/

#if !defined( HARDWARE_H )
#define HARDWARE_H

#define ENABLE_BIT_DEFINITIONS
#include <avr/io.h>

#define CPU_CLOCK   16000000

//--------------------------------------------------------------------------
// MCUCR - Control Register
//
//  Bit 7   SE      Sleep Enable
//  Bit 4-6 SM0-2   Sleep Mode 
//  Bit 2,3 ISC10,1 Interrupt 1 Sense Control
//  Bit 0,1 ISC00,1 Interrupt 0 Sense Control
//

#define MCUCR_IRQ0_RISING_EDGE  ( BV( ISC01 ) | BV( ISC00 ))
#define MCUCR_IRQ1_RISING_EDGE  ( BV( ISC11 ) | BV( ISC10 ))

#define MCUCR_INIT  ( MCUCR_IRQ0_RISING_EDGE | MCUCR_IRQ1_RISING_EDGE )

//--------------------------------------------------------------------------
// GICR - Global Interrupt Control Register

#define GICR_INIT   ( BV( INT1 ) | BV( INT0 ))

//--------------------------------------------------------------------------
// Timer/Counter 2
//
// We run Timer 2 in free running mode, 8us/tick

#define TCCR2_CLK_DIV_128   ( BV( CS22 ) | BV( CS20 ))

#define TCCR2_INIT  TCCR2_CLK_DIV_128

//--------------------------------------------------------------------------
// UART settings

#define BAUD_RATE   38400

#define DATA_BIT_8  ( 1 << UCSZ1 ) | ( 1 << UCSZ0 )
#define PARITY_NONE ( 0 << UPM1 )  | ( 0 << UPM0 )
#define STOP_BIT_1  ( 1 << USBS )

#define UBRR_INIT   (( CPU_CLOCK / 16 / BAUD_RATE ) - 1 )
#define UCSRA_INIT  0
#define UCSRB_INIT  ( 1 << RXEN ) | ( 1 << TXEN )                                                                          
#define UCSRC_INIT  ( 1 << URSEL ) | DATA_BIT_8 | PARITY_NONE | STOP_BIT_1

//--------------------------------------------------------------------------
// Port B

enum
{
    MotorA_Dir  = 0,    //
    MotorA_PWM  = 1,    //

    MotorB_PWM  = 2,    //
    MotorB_Dir  = 5,    //

    GreenLEDPin = 3,    // out
    BlueLEDPin  = 4,    // out

    // 6 & 7 are used for the 16 MHz Oscillator
};

#define DDRB_INIT   ( BV( BlueLEDPin ) | BV( GreenLEDPin ) )
#define PORTB_INIT  0

//--------------------------------------------------------------------------
// Port C - ADC Channels are just numbers (passed to a2d)

enum
{
    ADC_0       = 0,

//    EyeL      = 6,
//    EyeR      = 7,


};

#define DDRC_INIT   0
#define PORTC_INIT  0

//--------------------------------------------------------------------------
// Port D

enum
{
    UartRxD         = 0,    // in  RXD  - no pullup 
    UartTxD         = 1,    // out TXD

    RC_Pulse_In_0   = 2,    // in  INT0 - pullup enabled
    RC_Pulse_In_1   = 3,    // in  INT1 - pullup enabled
};

#define DDRD_INIT   ( BV( UartTxD ) )
#define PORTD_INIT  ( BV( RC_Pulse_In_0 ) | BV( RC_Pulse_In_1 ) )

//--------------------------------------------------------------------------

extern inline void BeginCritical(void) { asm volatile("cli\n"); }
extern inline void EndCritical(void) { asm volatile("sei\n"); }

void InitHardware( void );

#endif  // HARDWARE_H

