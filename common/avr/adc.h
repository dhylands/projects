/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   adc.h
*
*   @brief  Interupt driven interface for A/D converters
*
*   Configuration Options:
*
*   CFG_ADC_8_BIT   1 = Use 8-bit samples   ADC_Sample_t will be uint8_t
*                   0 = Use 10-bit samples  ADC_Sample_t will be uint16_t
*
*   CFG_ADC_POLL    1 = Use polled sample collection
*                   0 = Use interrupt driven sample collection
*
*   CFG_ADC_AVERAGE undefined - No averaging is performed
*                   N   N samples are averaged together
*
*****************************************************************************/

#if !defined( ANALOG_H )
#define ANALOG_H

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>
#include "Config.h"

/* ---- Constants and Types ---------------------------------------------- */

#if CFG_ADC_8_BIT

typedef uint8_t     ADC_Sample_t;

#else

typedef uint16_t    ADC_Sample_t;

#endif

// The ADC uses 10-bit successive approximation. A normal conversion takes
// 13 clock samples.
//
// So, a new ADC sample for a given channel will be updated every
// CFG_CPU_CLOCK / prescalar / 13 / 8 clock cycles.
//
// The divide by 8 is because there is only one A/D and the 8 channels are
// muxed one at a time to the ADC
//
// The datasheets suggest that the clock frequency be between 50 kHz 200 kHz
// to get maximum resolution, so the prescalar should be chosen such
// that CFG_CPU_CLOCK / prescalar is between 50,000 and 200,000.

#define ADC_PRESCALAR_2     (( 0 << ADPS2 ) | ( 0 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_4     (( 0 << ADPS2 ) | ( 1 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_8     (( 0 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_16    (( 1 << ADPS2 ) | ( 0 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_32    (( 1 << ADPS2 ) | ( 0 << ADPS1 ) | ( 1 << ADPS0 ))
#define ADC_PRESCALAR_64    (( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 0 << ADPS0 ))
#define ADC_PRESCALAR_128   (( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 ))

#if   (( CFG_CPU_CLOCK / 128 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_128
#elif (( CFG_CPU_CLOCK /  64 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_64
#elif (( CFG_CPU_CLOCK /  32 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_32
#elif (( CFG_CPU_CLOCK /  16 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_16
#elif (( CFG_CPU_CLOCK /   8 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_8
#elif (( CFG_CPU_CLOCK /   4 ) > 50000 )
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_4
#else
#define ADC_PRESCALAR_AUTO  ADC_PRESCALAR_2
#endif

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

ADC_Sample_t    ADC_Read( uint8_t channel );
void            ADC_Init( uint8_t prescalar );

#endif  // ADC_H

