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

#define F_CPU 8000000UL

/* ---- Include Files ---------------------------------------------------- */

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <math.h>

#include "Delay.h"
#include "Log.h"
#include "Timer.h"
#include "UART.h"
#include "compassTest.h"
#include "i2cmaster.h"

/* ---- Public Variables ------------------------------------------------- */


/* ---- Private Constants and Types -------------------------------------- */

//	Our LED is connected to PD2

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_MASK (1 << 2)

/* ---- Private Variables ------------------------------------------------ */


/* ---- Functions -------------------------------------------------------- */



/////////////////////////////////////////////////////
//
//	Turn on the LED
//

static inline void turnOnLED (void)
{
	LED_PORT |= LED_MASK;
}

/////////////////////////////////////////////////////
//
//	Turn off the LED
//

static inline void turnOffLED (void)
{
	LED_PORT &= ~LED_MASK;
}


/////////////////////////////////////////////////////
//
//	HMC 6343 Compass Module Test
//

int main (void)
{
	uint16_t heading;
	int16_t pitch, roll;
	uint16_t headingIntegerPart, headingFractionPart;
	int16_t pitchIntegerPart, pitchFractionPart;
	int16_t rollIntegerPart, rollFractionPart;

	// initialize the hardware stuff we use
	InitTimer ();
	InitUART ();
	i2c_init ();

	fdevopen (UART0_PutCharStdio, UART0_GetCharStdio);

	// set the LED port for output
	LED_DDR |= LED_MASK;

	printf ("\nHoneywell HMC6343 I2C Compass Test\n\n");

	// Flash the LED for 1/2 a second...
	turnOnLED ();
	ms_spin (500);
	turnOffLED ();

	while (1)	// outer loop is once every 250 ms (more or less)
	{
		// send the HEADING command to the compass
		i2c_start_wait (COMPASS_ADDRESS_WRITE);
		i2c_write (COMPASS_HEADING_COMMAND);

		// now read the response
		i2c_rep_start (COMPASS_ADDRESS_READ);
		heading = (i2c_readAck () * 256) + i2c_readAck ();
		pitch = (i2c_readAck () * 256) + i2c_readAck ();
		roll = (i2c_readAck () * 256) + i2c_readNak ();
		i2c_stop ();

		headingIntegerPart = heading / 10;
		headingFractionPart = heading - (headingIntegerPart * 10);
		pitchIntegerPart = pitch / 10;
		pitchFractionPart = pitch - (pitchIntegerPart * 10);
		if (pitchFractionPart < 0)
			pitchFractionPart *= -1;
		rollIntegerPart = roll / 10;
		rollFractionPart = roll - (rollIntegerPart * 10);
		if (rollFractionPart < 0)
			rollFractionPart *= -1;

		printf ("Heading: %3d.%1d   Pitch: %3d.%1d   Roll: %3d.%1d\n", headingIntegerPart, headingFractionPart, pitchIntegerPart, pitchFractionPart, rollIntegerPart, rollFractionPart);

		turnOnLED ();
		ms_spin (100);
		turnOffLED ();
		ms_spin (150);
	}

	// we'll never get here...
	return 0;
}

