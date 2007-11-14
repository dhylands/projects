/****************************************************************************
*
*   Copyright (c) 2007 Jon Hylands     <jon@huv.com>
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

#include <avr/io.h>

#include "lcd.h"
#include "Delay.h"
#include "Timer.h"
#include "adc.h"
#include "UART.h"
#include <stdlib.h>

#define LED_DDR        DDRB
#define LED_PORT       PORTB
#define LED_MASK       ( 1 << 7  )

#define RELAY_DDR        DDRB
#define RELAY_PORT       PORTB
#define RELAY_MASK       ( 1 << 2  )

#define THERMOCOUPLE_CHANNEL 0

#define TEMPERATURE_BUFFER_COUNT 20

// Define the hysteresis parameters.
#define LOW_TRIGGER_OFFSET	2
#define HIGH_TRIGGER_OFFSET	1

// Define the profile parameters.
#define PREHEATING_TEMPERATURE	145
#define DRYING_TEMPERATURE		150
#define DRYING_TIME				90
#define HEATING_TEMPERATURE		233
#define REFLOW_TEMPERATURE		235
#define REFLOW_TIME				45

// Define the states.
#define WAITING_STATE		0
#define PREHEATING_STATE	1
#define DRYING_STATE		2
#define HEATING_STATE		3
#define REFLOW_STATE		4
#define COOLING_STATE		5

// Declare the global variables.
uint8_t gCurrentState = WAITING_STATE;
uint8_t gRemainingTime = 0;

uint16_t gCurrentTemperature = 0;
uint16_t gSetPointTemperature = 0;
uint16_t gTurnOnTemperature = 0;
uint16_t gTurnOffTemperature = 0;

uint16_t gTemperatureBuffer [TEMPERATURE_BUFFER_COUNT];

//////////////////////////////////////////////////////////////////////////

static int compareNumbers (uint16_t *num1, uint16_t *num2)
{
	if (*num1 <  *num2)
		return -1;
	if (*num1 == *num2)
		return  0;
	return  1;
}

/*
static int compareNumbers (void *void_num1, void *void_num2)
{
	uint16_t *num1 = (uint16_t *)void_num1;
	uint16_t *num2 = (uint16_t *)void_num2;
	
	if (*num1 < *num2)
		return -1;
	if (*num1 == *num2)
		return 0;
	return 1;
}
*/

//////////////////////////////////////////////////////////////////////////
//
// convertToTemperature
//
// Answer a temperature (in tenths of Celsius degrees) from the sensor input.
// The conversion is determined by breaking the 5v A/D range into the
// 1024 possible values (4.88 mV), then dividing by the 10mV per degree
// which the thermocouple amplifier outputs (5 / 1024 / 0.01 = 0.488).
//

static uint16_t convertToTemperature( uint16_t sensorInput ) {

	uint16_t	temperature;
	uint32_t	conversion;

	// Determine the temperature in thousandths of degrees (*0.488 * 1000).
	conversion = sensorInput;
	conversion = conversion * 488;

	// Scale the temperature up into tenths of degrees, and cast it into
	// 16 bits.
	conversion = conversion / 100;

	temperature = conversion;

	return( temperature );
}

//////////////////////////////////////////////////////////////////////////
//
// aquireTemperature
//
// Answer a temperature (in degrees C) taken by reading the temperature
//	probe a number of times (TEMPERATURE_BUFFER_COUNT), and taking
//	the median measurement. Since this is an oven, we discard any measurements
//	that are zero...
//

static uint16_t aquireTemperature (void) {
	uint16_t value;
	uint16_t temperature;
	uint8_t index;

	// Read the analog input for the temperature a bunch of times, and buffer them.
	for (index = 0; index < TEMPERATURE_BUFFER_COUNT; index++)
	{
		while ((value = ADC_Read (THERMOCOUPLE_CHANNEL)) == 0);
		temperature = convertToTemperature (value);
		value = (temperature + 5) / 10;
		gTemperatureBuffer [index] = value;
		printf ("Temp [%2d]: %4d\n", index, value);
		ms_spin (20);
	}

	// sort the buffer so we can take the median value
	qsort (gTemperatureBuffer, TEMPERATURE_BUFFER_COUNT, sizeof (uint16_t), (void *)compareNumbers);

	for (index = 0; index < TEMPERATURE_BUFFER_COUNT; index++)
		printf ("Sort [%2d]: %3d\n", index, gTemperatureBuffer [index]);

	value = gTemperatureBuffer [TEMPERATURE_BUFFER_COUNT * 3 / 4];

	return (value);
}

//////////////////////////////////////////////////////////////////////////

static void displayCurrentAndDesiredTemperature (void) {

	LCD_MoveTo (0, 1);
	LCD_Printf ("T: %3dC DT: %3dC", gCurrentTemperature, gSetPointTemperature);
}

//////////////////////////////////////////////////////////////////////////
//
// displayRemainingTime
//
// Update the LCD display to show the remainingTime.
//

static void displayRemainingTime (void) {

	LCD_MoveTo (8, 0);
	if (gRemainingTime == 0)
		LCD_PutStr ("...   ");
	else
		LCD_Printf ("%3d s", gRemainingTime);
}

//////////////////////////////////////////////////////////////////////////
//
// turnOnOven
//
// Turn the oven on.
//

static inline void turnOnOven (void) {

	RELAY_PORT |= RELAY_MASK;
}

//////////////////////////////////////////////////////////////////////////
//
// turnOffOven
//
// Turn the oven off.
//

static inline void turnOffOven (void) {

	RELAY_PORT &= ~RELAY_MASK;
}

//////////////////////////////////////////////////////////////////////////
//
// turnOnLED
//
// Turn the LED on.
//

static inline void turnOnLED (void) {

	LED_PORT |= LED_MASK;
}

//////////////////////////////////////////////////////////////////////////
//
// turnOffLED
//
// Turn the LED off.
//

static inline void turnOffLED (void) {

	LED_PORT &= ~LED_MASK;
}

//////////////////////////////////////////////////////////////////////////
//
// setPreheatingState
//
// Change the state-related parameters appropriately for this state.
//

static void setPreheatingState (void) {

	// Show the Reflow state message.
	LCD_MoveTo (0, 0);
	LCD_PutStr ("Preheat ");

	// Set the parameters.
	gCurrentState = PREHEATING_STATE;
	gTurnOnTemperature = PREHEATING_TEMPERATURE;
	gTurnOffTemperature = PREHEATING_TEMPERATURE;
	gSetPointTemperature = DRYING_TEMPERATURE;
	gRemainingTime = 0;
}


//////////////////////////////////////////////////////////////////////////
//
// setDryingState
//
// Change the state-related parameters appropriately for this state.
//

static void setDryingState (void) {

	// Show the Reflow state message.
	LCD_MoveTo (0, 0);
	LCD_PutStr ("Drying  ");

	// Set the parameters.
	gCurrentState = DRYING_STATE;
	gTurnOnTemperature = DRYING_TEMPERATURE - LOW_TRIGGER_OFFSET;
	gTurnOffTemperature = DRYING_TEMPERATURE + HIGH_TRIGGER_OFFSET;
	gSetPointTemperature = DRYING_TEMPERATURE;
	gRemainingTime = DRYING_TIME;
}

//////////////////////////////////////////////////////////////////////////
//
// setHeatingState
//
// Change the state-related parameters appropriately for this state.
//

static void setHeatingState (void) {

	// Show the Reflow state message.
	LCD_MoveTo (0, 0);
	LCD_PutStr ("Heating ");

	// Set the parameters.
	gCurrentState = HEATING_STATE;
	gTurnOnTemperature = HEATING_TEMPERATURE;
	gTurnOffTemperature = HEATING_TEMPERATURE;
	gSetPointTemperature = REFLOW_TEMPERATURE;
	gRemainingTime = 0;
}

//////////////////////////////////////////////////////////////////////////
//
// setReflowState
//
// Change the state-related parameters appropriately for this state.
//

static void setReflowState (void) {

	// Show the Reflow state message.
	LCD_MoveTo (0, 0);
	LCD_PutStr ("Reflow  ");

	// Set the parameters.
	gCurrentState = REFLOW_STATE;
	gTurnOnTemperature = REFLOW_TEMPERATURE - LOW_TRIGGER_OFFSET;
	gTurnOffTemperature = REFLOW_TEMPERATURE + HIGH_TRIGGER_OFFSET;
	gSetPointTemperature = REFLOW_TEMPERATURE;
	gRemainingTime = REFLOW_TIME;
}

//////////////////////////////////////////////////////////////////////////
//
// setCoolingState
//
// Change the state-related parameters appropriately for this state.
//

static void setCoolingState (void) {

	// Show the Cooling state message.
	LCD_MoveTo (0, 0);
	LCD_PutStr ("Cooling ");

	// Turn off oven, and set the parameters.
	turnOffOven();
	gTurnOnTemperature = 0;
	gTurnOffTemperature = 0;
	gSetPointTemperature = 0;
	gRemainingTime = 0;
	gCurrentState = COOLING_STATE;
}

//////////////////////////////////////////////////////////////////////////
//
// maintainSetPointTemperature
//
// Turn the oven on or off as appropriate to maintain the set point
// temperature.
//

static uint8_t maintainSetPointTemperature (void) {

	// If the set point temperature is zero, then we have no set point,
	// so we make sure the oven is off.
	if (gSetPointTemperature == 0)
	{
		turnOffOven ();
		return (0);
	}

	// If the current temperature is above the turn-off point,
	// then we turn the oven off.
	if (gCurrentTemperature >= gTurnOffTemperature)
	{
		turnOffOven ();
		return (0);
	}

	// If the current temperature is below the turn-on point,
	// then we turn the oven on.
	if (gCurrentTemperature <= gTurnOnTemperature)
	{
		turnOnOven ();
		return (1);
	}

	return (2);
}

//////////////////////////////////////////////////////////////////////////
//
// processCurrentState
//
// This routine uses the current temperature, time, and state to decide
// what needs to be done (ie. turn on or off the oven, or change states).
//

static void processCurrentState (void) {

	switch (gCurrentState)
	{
		case PREHEATING_STATE:

			// If we have reached the target temperature, move on to the drying state.
			if (gCurrentTemperature >= gSetPointTemperature)
				setDryingState ();
			else
				maintainSetPointTemperature ();
			break;

		case DRYING_STATE:

			// If the drying time has expired, move on to the heating state.
			if (gRemainingTime == 0)
				setHeatingState ();
			else
				maintainSetPointTemperature ();
			break;

		case HEATING_STATE:

			// If we have reached the target temperature, move on to the reflow state.
			if (gCurrentTemperature >= gSetPointTemperature)
				setReflowState ();
			else
				maintainSetPointTemperature ();
			break;

		case REFLOW_STATE:

			// If the reflow time has expired, move on to the cooling state.
			if (gRemainingTime == 0)
				setCoolingState();
			else
				maintainSetPointTemperature ();
			break;
	}

	displayRemainingTime ();
	displayCurrentAndDesiredTemperature ();
}

//////////////////////////////////////////////////////////////////////////

int main(void)
{

    InitTimer();
	ADC_Init (ADC_PRESCALAR_AUTO);

    LED_DDR |= LED_MASK;
    RELAY_DDR |= RELAY_MASK;

    InitUART();
    fdevopen( UART0_PutCharStdio, UART0_GetCharStdio );

    printf ("Reflow Oven\n");
    printf ("HUVrobotics.com\n");

	// Start with the Oven relay off.
	turnOffOven ();
	turnOnLED ();

    LCD_Init( 2, 16 );
	LCD_PutStr ("Reflow Oven\n");
	LCD_PutStr ("HUVrobotics.com");

	ms_spin (2000);

	turnOffLED ();
	LCD_Clear ();
	// Start the procedure by preheating the oven.
	setPreheatingState ();
	gRemainingTime = 0;

	// Perform the main loop of updating the currentTemperature and remainingTime,
	// then doing what is appropriate, depending on the currentState.
	do {
		gCurrentTemperature = aquireTemperature ();

		// If the remainingTime has not already expired, decrement it.
		if (gRemainingTime > 0)
			gRemainingTime--;

		// Perform the required actions, depending on the currentState.
		processCurrentState ();

		turnOnLED ();
		ms_spin (100);
		turnOffLED ();
		// Delay appropriately to make this loop run at 1 Hz.
		ms_spin (300);

	} while (1);

    return 0;
}
