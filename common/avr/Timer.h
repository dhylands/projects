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
*   @file   Timer.h
*
*   @brief  Defines all of the hardware definitions for the chip.
*
****************************************************************************/

#if !defined( TIMER_H )
#define TIMER_H             /**< Include Guard                             */

#include "Config.h"
#include <avr/io.h>

/* ---- Include Files ---------------------------------------------------- */

/* ---- Constants and Types ---------------------------------------------- */

#define TIMER0_CLOCK_SEL_MASK       (( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))
#define TIMER1_CLOCK_SEL_MASK       (( 1 << CS12 ) | ( 1 << CS11 ) | ( 1 << CS10 ))
#define TIMER2_CLOCK_SEL_MASK       (( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ))
#define TIMER3_CLOCK_SEL_MASK       (( 1 << CS32 ) | ( 1 << CS31 ) | ( 1 << CS30 ))

#define TIMER1_CLOCK_SEL_NONE       (( 0 << CS12 ) | ( 0 << CS11 ) | ( 0 << CS10 ))
#define TIMER1_CLOCK_SEL_DIV_1      (( 0 << CS12 ) | ( 0 << CS11 ) | ( 1 << CS10 ))
#define TIMER1_CLOCK_SEL_DIV_8      (( 0 << CS12 ) | ( 1 << CS11 ) | ( 0 << CS10 ))
#define TIMER1_CLOCK_SEL_DIV_64     (( 0 << CS12 ) | ( 1 << CS11 ) | ( 1 << CS10 ))
#define TIMER1_CLOCK_SEL_DIV_256    (( 1 << CS12 ) | ( 0 << CS11 ) | ( 0 << CS10 ))
#define TIMER1_CLOCK_SEL_DIV_1024   (( 1 << CS12 ) | ( 0 << CS11 ) | ( 1 << CS10 ))
#define TIMER1_CLOCK_SEL_T1_FALLING (( 1 << CS12 ) | ( 1 << CS11 ) | ( 0 << CS10 ))
#define TIMER1_CLOCK_SEL_T1_RISING  (( 1 << CS12 ) | ( 1 << CS11 ) | ( 1 << CS10 ))

#if defined (__AVR_ATmega8__) \
 || defined (__AVR_ATmega16__) \
 || defined (__AVR_ATmega32__) \
 || defined (__AVR_ATmega48__) \
 || defined (__AVR_ATmega88__) \
 || defined (__AVR_ATmega168__) \

#define TIMER0_CLOCK_SEL_NONE       (( 0 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1      (( 0 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_8      (( 0 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_64     (( 0 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_256    (( 1 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1024   (( 1 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_T0_FALLING (( 1 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_T0_RISING  (( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))

#elif defined (__AVR_ATmega644__)

#define TIMER0_CLOCK_SEL_NONE       (( 0 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1      (( 0 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_8      (( 0 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_64     (( 0 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_256    (( 1 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1024   (( 1 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_T0_FALLING (( 1 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_T0_RISING  (( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))

#define TIMER2_CLOCK_SEL_NONE       (( 0 << CS22 ) | ( 0 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_1      (( 0 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_8      (( 0 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_32     (( 0 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_64     (( 1 << CS22 ) | ( 0 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_128    (( 1 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_256    (( 1 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_1024   (( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ))

#elif defined (__AVR_ATmega64__) \
   || defined (__AVR_ATmega128__)

#define TIMER0_CLOCK_SEL_NONE       (( 0 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1      (( 0 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_8      (( 0 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_32     (( 0 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_64     (( 1 << CS02 ) | ( 0 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_128    (( 1 << CS02 ) | ( 0 << CS01 ) | ( 1 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_256    (( 1 << CS02 ) | ( 1 << CS01 ) | ( 0 << CS00 ))
#define TIMER0_CLOCK_SEL_DIV_1024   (( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ))

#define TIMER2_CLOCK_SEL_NONE       (( 0 << CS22 ) | ( 0 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_1      (( 0 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_8      (( 0 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_64     (( 0 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_256    (( 1 << CS22 ) | ( 0 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_DIV_1024   (( 1 << CS22 ) | ( 0 << CS21 ) | ( 1 << CS20 ))
#define TIMER2_CLOCK_SEL_T2_FALLING (( 1 << CS22 ) | ( 1 << CS21 ) | ( 0 << CS20 ))
#define TIMER2_CLOCK_SEL_T2_RISING  (( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ))

#define TIMER3_CLOCK_SEL_NONE       (( 0 << CS32 ) | ( 0 << CS31 ) | ( 0 << CS30 ))
#define TIMER3_CLOCK_SEL_DIV_1      (( 0 << CS32 ) | ( 0 << CS31 ) | ( 1 << CS30 ))
#define TIMER3_CLOCK_SEL_DIV_8      (( 0 << CS32 ) | ( 1 << CS31 ) | ( 0 << CS30 ))
#define TIMER3_CLOCK_SEL_DIV_64     (( 0 << CS32 ) | ( 1 << CS31 ) | ( 1 << CS30 ))
#define TIMER3_CLOCK_SEL_DIV_256    (( 1 << CS32 ) | ( 0 << CS31 ) | ( 0 << CS30 ))
#define TIMER3_CLOCK_SEL_DIV_1024   (( 1 << CS32 ) | ( 0 << CS31 ) | ( 1 << CS30 ))
#define TIMER3_CLOCK_SEL_T3_FALLING (( 1 << CS32 ) | ( 1 << CS31 ) | ( 0 << CS30 ))
#define TIMER3_CLOCK_SEL_T3_RISING  (( 1 << CS32 ) | ( 1 << CS31 ) | ( 1 << CS30 ))

#define TIMER_1A_PIN     5
#define TIMER_1A_MASK    ( 1 << 5 )
#define TIMER_1A_PORT    PORTB
#define TIMER_1A_DDR     DDRB
#define TIMER_1A_OCR     OCR1A
#define TIMER_1A_COM_1   COM1A1
#define TIMER_1A_COM0    COM1A0
#define TIMER_1A_TCCRA   TCCR1A

#define TIMER_1B_PIN     6
#define TIMER_1B_MASK    ( 1 << 6 )
#define TIMER_1B_PORT    PORTB
#define TIMER_1B_DDR     DDRB
#define TIMER_1B_OCR     OCR1B
#define TIMER_1B_COM_1   COM1B1
#define TIMER_1B_COM0    COM1B0
#define TIMER_1B_TCCRA   TCCR1A

#define TIMER_1C_PIN     7
#define TIMER_1C_MASK    ( 1 << 7 )
#define TIMER_1C_PORT    PORTB
#define TIMER_1C_DDR     DDRB
#define TIMER_1C_OCR     OCR1C
#define TIMER_1C_COM_1   COM1C1
#define TIMER_1C_COM0    COM1C0
#define TIMER_1C_TCCRA   TCCR1A

#define TIMER_3A_PIN     3
#define TIMER_3A_MASK    ( 1 << 3 )
#define TIMER_3A_PORT    PORTE
#define TIMER_3A_DDR     DDRE
#define TIMER_3A_OCR     OCR3A
#define TIMER_3A_COM_1   COM3A1
#define TIMER_3A_COM0    COM3A0
#define TIMER_3A_TCCRA   TCCR3A

#define TIMER_3B_PIN     4
#define TIMER_3B_MASK    ( 1 << 4 )
#define TIMER_3B_PORT    PORTE
#define TIMER_3B_DDR     DDRE
#define TIMER_3B_OCR     OCR3B
#define TIMER_3B_COM_1   COM3B1
#define TIMER_3B_COM0    COM3B0
#define TIMER_3B_TCCRA   TCCR3A

#define TIMER_3C_PIN     5
#define TIMER_3C_MASK    ( 1 << 5 )
#define TIMER_3C_PORT    PORTE
#define TIMER_3C_DDR     DDRE
#define TIMER_3C_OCR     OCR3C
#define TIMER_3C_COM_1   COM3C1
#define TIMER_3C_COM0    COM3C0
#define TIMER_3C_TCCRA   TCCR3A

#else
#   error   Common/avr/Timer.h Processor not supported
#endif

/* ---- Variable Externs ------------------------------------------------- */

#if CFG_TIMER_8_BIT_TICK
typedef uint8_t     tick_t;     // Counts once per 10 msec
#else
typedef uint16_t    tick_t;     // Counts once per 10 msec
#endif

extern volatile tick_t      gTickCount;

#if CFG_TIMER_MS_TICK

typedef uint32_t    msTick_t;   // Counts once per millisecond

extern volatile msTick_t    gMsTickCount;

#endif

/* ---- Function Prototypes ---------------------------------------------- */

void InitTimer( void );
void WaitForTimer0Rollover( void );
                  
#endif  // TIMER_H
