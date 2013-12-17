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
#include <ctype.h>

#include "Delay.h"
#include "adc.h"
#include "Timer.h"
#include "UART.h"
#include "Led.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define BATTERY_GATE_DDR	 DDRB
#define BATTERY_GATE_PORT	 PORTB
#define BATTERY_GATE_MASK	 ( 1 << 2 )

#define WALL_GATE_DDR			 DDRB
#define WALL_GATE_PORT	   PORTB
#define WALL_GATE_MASK	   ( 1 << 1 )

#define BATTERY_CHANNEL 5
#define WALL_CHANNEL 3

// modes and other defines

#define MODE_NONE 0
#define MODE_WALL 1
#define MODE_BATTERY 2

static const char *gModeStr[] = {"none", "wall", "battery"};

#define THRESHOLD_VOLTAGE 500

// LOW_BATTERY_VOLTAGE = 9.6 volts / 3 / 5 * 1023 = 655
//#define LOW_BATTERY_VOLTAGE 655
#define LOW_BATTERY_VOLTAGE 400

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

static void HandleUARTChar(void)
{
	char ch = getchar();
	ch = toupper(ch);
	switch (ch)
	{
		 case 'W':
			 printf("W - Toggle Wall Power Enable (AUX_EN)\n");
			 WALL_GATE_PORT ^= WALL_GATE_MASK;
			 break; 

		 case 'B':
			 printf("B - Toggle Battery Power Enable (BATT_EN)\n");
			 BATTERY_GATE_PORT ^= BATTERY_GATE_MASK;
			 break;

		 case 'R':
			 printf("R - Toggle Red LED\n");
			 LED_Toggle(RED);
			 break;

		 case 'G':
			 printf("G - Toggle Green LED\n");
			 LED_Toggle(GREEN);
			 break;

		 case 'O':
			 printf("O - Toggle Orange LED\n");
			 LED_Toggle(ORANGE);
			 break;

		 case ' ':
			 break;

		 default:
			 printf("Unrecognized command '%c'\n", ch);
			 break;
	}


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

	LED_Init(GREEN);
	LED_Init(ORANGE);
	LED_Init(RED);

	LED_Off(GREEN);
	LED_Off(ORANGE);
	LED_Off(RED);

	turnOffBatteryPower ();
	turnOffWallPower ();

	printf("\nBioloid Aux Power\n");
	printf("W - Toggle Wall Enable\n");
	printf("B - Toggle Battery Enable\n");
	printf("G - Toggle Green LED\n");
	printf("R - Toggle Red LED\n");
	printf("O - Toggle Orange LED\n");
	printf("Space - Report\n");

	// Flash the LEDs for 200ms each...
	LED_On(GREEN);
	ms_spin (200);
	LED_Off(GREEN);

	LED_On(ORANGE);
	ms_spin (200);
	LED_Off(ORANGE);

	LED_On(RED);
	ms_spin (200);
	LED_Off(RED);
	ms_spin (500);

	mode = MODE_NONE;

	DDRD &= ~(1 << PD3);   //INT1 inputs 

	// EICRA |= (1 << ISC11) | (1 << ISC10);   //INT1 rising edge
	EICRA |= (0 << ISC11) | (1 << ISC10);   //INT1 both edges
	EIMSK |= (1 << INT1);   //INT1 Enable 
	sei ();

	while ( 1 )
	{
		(void)ADC_Read (BATTERY_CHANNEL);
		batteryVoltage = ADC_Read (BATTERY_CHANNEL);

		(void)ADC_Read (WALL_CHANNEL);
		wallVoltage = ADC_Read (WALL_CHANNEL);

		if (UART0_IsCharAvailable())
		{
			HandleUARTChar();
			printf("MODE = %d %s\n", mode, gModeStr[mode]);
			printf(" AUX_EN: %s\n", (WALL_GATE_PORT & WALL_GATE_MASK) ? "high" : "low");
			printf("BATT_EN: %s\n", (BATTERY_GATE_PORT & BATTERY_GATE_MASK) ? "high" : "low");
			printf("Wall    Voltage: %4d\n", wallVoltage);
			printf("Battery Voltage: %4d\n", batteryVoltage);
			printf("\n");
		}

		if ((mode != MODE_WALL) && (wallVoltage > THRESHOLD_VOLTAGE))
		{
			turnOnWallPower();
			turnOffBatteryPower();
			printf("\nSwitching to wall power\n");
			printf("Wall    Voltage: %4d\n", wallVoltage);
			printf("Battery Voltage: %4d\n", batteryVoltage);
			LED_On(GREEN);
			ms_spin (1000);
			LED_Off(GREEN);
			mode = MODE_WALL;
		}
		else if ((mode != MODE_BATTERY) && (wallVoltage <= THRESHOLD_VOLTAGE))
		{
			turnOnBatteryPower();
			turnOffWallPower();
			printf("\nSwitching to battery power\n");
			printf("Wall    Voltage: %4d\n", wallVoltage);
			printf("Battery Voltage: %4d\n", batteryVoltage);
			LED_On(GREEN);
			ms_spin(1000);
			LED_Off(GREEN);
			mode = MODE_BATTERY;
		}
		if ((mode == MODE_BATTERY) && (batteryVoltage <= LOW_BATTERY_VOLTAGE))
		{
			turnOffBatteryPower();
			turnOffWallPower();
			printf("Battery Low: %4d\n", batteryVoltage);
			LED_On(RED);
			ms_spin(100);
			LED_Off(RED);
			ms_spin(900);
		}
	}
}
