/****************************************************************************
*
*   Copyright (c) 2007 Jon Hylands	 <jon@huv.com>
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

/* ---- Include Files ---------------------------------------------------- */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <math.h>
#include <avr/sleep.h>

#include "Delay.h"
#include "adc.h"
#include "Timer.h"
#include "UART.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define BATTERY_GATE_DDR		DDRB
#define BATTERY_GATE_PORT	   PORTB
#define BATTERY_GATE_MASK	   ( 1 << 0 )

#define WALL_GATE_DDR		DDRD
#define WALL_GATE_PORT	   PORTD
#define WALL_GATE_MASK	   ( 1 << 7 )

#define BATTERY_CHANNEL 5
#define WALL_CHANNEL 3

#define GREEN_LED_DDR DDRB
#define GREEN_LED_PORT PORTB
#define GREEN_LED_MASK (1 << 6)

#define ORANGE_LED_DDR DDRB
#define ORANGE_LED_PORT PORTB
#define ORANGE_LED_MASK (1 << 7)

#define RED_LED_DDR DDRD
#define RED_LED_PORT PORTD
#define RED_LED_MASK (1 << 5)

// modes and other defines

#define MODE_NONE 0
#define MODE_WALL 1
#define MODE_BATTERY 2

#define THRESHOLD_VOLTAGE 500

// LOW_BATTERY_VOLTAGE = 9.6 volts / 3 / 5 * 1023 = 655
#define LOW_BATTERY_VOLTAGE 655

/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

static void turnOffWallPower (void)
{
	WALL_GATE_PORT &= ~WALL_GATE_MASK;
}

static void turnOnWallPower (void)
{
	WALL_GATE_PORT |= WALL_GATE_MASK;
}

static void turnOffBatteryPower (void)
{
	BATTERY_GATE_PORT &= ~BATTERY_GATE_MASK;
}

static void turnOnBatteryPower (void)
{
	BATTERY_GATE_PORT |= BATTERY_GATE_MASK;
}


ISR (INT1_vect)
{
	// LED_PORT |= LED_MASK;
}


//***************************************************************************
/**
*   aux power controller
*/

int main( void )
{
	uint16_t batteryVoltage;
	uint16_t wallVoltage;
	uint8_t mode;

	InitTimer ();
	ADC_Init (ADC_PRESCALAR_AUTO);
	InitUART();
	fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

	// Initialize the two GATE pins
	BATTERY_GATE_DDR |= BATTERY_GATE_MASK;
	WALL_GATE_DDR |= WALL_GATE_MASK;

	GREEN_LED_DDR |= GREEN_LED_MASK;
	ORANGE_LED_DDR |= ORANGE_LED_MASK;
	RED_LED_DDR |= RED_LED_MASK;

	GREEN_LED_PORT &= ~GREEN_LED_MASK;
	ORANGE_LED_PORT &= ~ORANGE_LED_MASK;
	RED_LED_PORT &= ~RED_LED_MASK;

	turnOffBatteryPower ();
	turnOffWallPower ();

	printf ("\nBioloid Aux Power\n");

	// Flash the LEDs for 200ms each...
	GREEN_LED_PORT |= GREEN_LED_MASK;
	ms_spin (200);
	GREEN_LED_PORT &= ~GREEN_LED_MASK;
	ORANGE_LED_PORT |= ORANGE_LED_MASK;
	ms_spin (200);
	ORANGE_LED_PORT &= ~ORANGE_LED_MASK;
	RED_LED_PORT |= RED_LED_MASK;
	ms_spin (200);
	RED_LED_PORT &= ~RED_LED_MASK;
	ms_spin (500);

	mode = MODE_NONE;

	DDRD &= ~(1 << PD3);   //INT1 inputs 

	// EICRA |= (1 << ISC11) | (1 << ISC10);   //INT1 rising edge
	EICRA |= (0 << ISC11) | (1 << ISC10);   //INT1 both edges
	EIMSK |= (1 << INT1);   //INT1 Enable 
	sei ();

	while ( 1 )
	{
		uint16_t junk = ADC_Read (BATTERY_CHANNEL);
		batteryVoltage = ADC_Read (BATTERY_CHANNEL);

		junk = ADC_Read (WALL_CHANNEL);
		wallVoltage = ADC_Read (WALL_CHANNEL);

		if ((mode != MODE_WALL) && (wallVoltage > THRESHOLD_VOLTAGE))
		{
			turnOnWallPower ();
			turnOffBatteryPower ();
			printf ("\nSwitching to wall power\n");
			printf ("Wall Voltage: %4d\n", wallVoltage);
			printf ("Battery Voltage: %4d\n", batteryVoltage);
			GREEN_LED_PORT |= GREEN_LED_MASK;
			ms_spin (1000);
			GREEN_LED_PORT &= ~GREEN_LED_MASK;
			mode = MODE_WALL;
		}
		else if ((mode != MODE_BATTERY) && (wallVoltage <= THRESHOLD_VOLTAGE))
		{
			turnOnBatteryPower ();
			turnOffWallPower ();
			printf ("\nSwitching to battery power\n");
			printf ("Wall Voltage: %4d\n", wallVoltage);
			printf ("Battery Voltage: %4d\n", batteryVoltage);
			GREEN_LED_PORT |= GREEN_LED_MASK;
			ms_spin (1000);
			GREEN_LED_PORT &= ~GREEN_LED_MASK;
			mode = MODE_BATTERY;
		}
		if ((mode == MODE_BATTERY) && (batteryVoltage <= LOW_BATTERY_VOLTAGE))
		{
			turnOffBatteryPower ();
			turnOffWallPower ();
			printf ("Battery Low: %4d\n", batteryVoltage);
			RED_LED_PORT |= RED_LED_MASK;
			ms_spin (100);
			RED_LED_PORT &= ~RED_LED_MASK;
			ms_spin (900);
		}
	}
}
