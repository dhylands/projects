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

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <math.h>

#include "Delay.h"
#include "adc.h"
#include "Log.h"
#include "Timer.h"
#include "Timer-UART.h"
#include "bioloid-uart.h"
#include "bioloid-pkt.h"
#include "bioloidGripper.h"
#include "Motor.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

//	Our LED is connected to PD3

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_MASK (1 << 3)

/* ---- Private Variables ------------------------------------------------ */

uint8_t gMyId;
uint8_t gControlTable [NUM_CONTROL_ENTRIES];

uint8_t gWristRotationDirection = WRIST_ROTATION_STOPPED;
double gWristRotationIncrement;
double gWristRotationPosition;

// Gripper Finger Variables.
int16_t lastFingerOffset = 0;
int16_t fingerErrorAccumulation = 0;
double fPk = 0.3, fDk = 0.0, fIk = 0.0;  // PID constants
int16_t speedOffset = 512;

// Gripper Wrist Variables.
int16_t lastWristOffset = 0;
int16_t wristErrorAccumulation = 0;
double wPk = 0.2, wDk = 0.0, wIk = 0.0;  // PID constants

uint8_t gCycleCount;
uint8_t sensorCycleCount = 0;
/* ---- Functions -------------------------------------------------------- */


//***************************************************************************
/**
*   The baud rate has been changed. Update UART0
*/

static void HandleBaudRateChange (uint8_t baudRate)
{
	switch (baudRate)
	{
		case 1: UART0_SetBaudRate (1000000); break;
		case 3: UART0_SetBaudRate (500000); break;
		case 4: UART0_SetBaudRate (400000); break;
		case 7: UART0_SetBaudRate (250000); break;
		case 9: UART0_SetBaudRate (200000); break;
		case 16: UART0_SetBaudRate (115200); break;
		case 34: UART0_SetBaudRate (57600); break;
		case 103: UART0_SetBaudRate (19200); break;
		case 207: UART0_SetBaudRate (9600); break;
	}
}


//***************************************************************************
/**
*   Update the EEPROM with what's in memory...
*/

static void UpdateEEPROM (void)
{
	Log ("Writing EEPROM Control Table Entries...\n");
	// We keep this magic constant in (EEPROM_BASE_ADDRESS - 1) as a form of CRC
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS - 1), EEPROM_ELEMENT_COUNT);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_MODEL_NUMBER_LOW), gControlTable [CONTROL_MODEL_NUMBER_LOW]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_MODEL_NUMBER_HIGH), gControlTable [CONTROL_MODEL_NUMBER_HIGH]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_FIRMWARE_VERSION), gControlTable [CONTROL_FIRMWARE_VERSION]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_ID), gControlTable [CONTROL_ID]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_BAUD_RATE), gControlTable [CONTROL_BAUD_RATE]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_RETURN_DELAY_TIME), gControlTable [CONTROL_RETURN_DELAY_TIME]);
	eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_STATUS_RETURN_LEVEL), gControlTable [CONTROL_STATUS_RETURN_LEVEL]);
}


//***************************************************************************
/**
*   Initialize Control Table
*/

static void InitializeControlTable (uint8_t forceOverwrite)
{
	uint8_t eepromSize;

	memset (&gControlTable, 0, sizeof (gControlTable));

	// NOTE - These are the hardware defaults...
	gControlTable [CONTROL_MODEL_NUMBER_LOW] = 12;
	gControlTable [CONTROL_MODEL_NUMBER_HIGH] = 67;
	gControlTable [CONTROL_FIRMWARE_VERSION] = 1;
	gControlTable [CONTROL_ID] = 80;
	gControlTable [CONTROL_BAUD_RATE] = 1;
	gControlTable [CONTROL_RETURN_DELAY_TIME] = 250;
	gControlTable [CONTROL_STATUS_RETURN_LEVEL] = BLD_STATUS_RETURN_LEVEL_ALL;

	gControlTable [CONTROL_DESIRED_FINGER_POSITION_LOW] = (MAX_FINGER_POSITION & 0xFF);
	gControlTable [CONTROL_DESIRED_FINGER_POSITION_HIGH] = (MAX_FINGER_POSITION >> 8);
	gControlTable [CONTROL_DESIRED_FINGER_SPEED_LOW] = 255;
	gControlTable [CONTROL_DESIRED_FINGER_SPEED_HIGH] = 3;

	gControlTable [CONTROL_DESIRED_WRIST_POSITION_LOW] = 0;
	gControlTable [CONTROL_DESIRED_WRIST_POSITION_HIGH] = 2;
	gControlTable [CONTROL_DESIRED_WRIST_SPEED_LOW] = 255;
	gControlTable [CONTROL_DESIRED_WRIST_SPEED_HIGH] = 3;

	// if the magic constant is there, the rest of the values are probably okay.
	// if its not there, or we're doing a hard reset, ignore what is in the EEPROM,
	// and over-write it with the hardware default values...
	eepromSize = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS - 1));
	if (!forceOverwrite && (eepromSize == EEPROM_ELEMENT_COUNT))
	{
		Log ("Reading EEPROM Control Table Entries...\n");
		gControlTable [CONTROL_MODEL_NUMBER_LOW] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_MODEL_NUMBER_LOW));
		gControlTable [CONTROL_MODEL_NUMBER_HIGH] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_MODEL_NUMBER_HIGH));
		gControlTable [CONTROL_FIRMWARE_VERSION] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_FIRMWARE_VERSION));
		gControlTable [CONTROL_ID] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_ID));
		gControlTable [CONTROL_BAUD_RATE] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_BAUD_RATE));
		gControlTable [CONTROL_RETURN_DELAY_TIME] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_RETURN_DELAY_TIME));
		gControlTable [CONTROL_STATUS_RETURN_LEVEL] = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + CONTROL_STATUS_RETURN_LEVEL));
	}
	else
		UpdateEEPROM ();

	// We copy the ID into its own global since we it them a lot...
	gMyId = gControlTable [CONTROL_ID];

	// We need to handle baud rate specifically, because it has to be set when it changes
	if (gControlTable [CONTROL_BAUD_RATE] != 1)
		HandleBaudRateChange (gControlTable [CONTROL_BAUD_RATE]);
}


//***************************************************************************
/**
*   Check if given address & value are in range for writing...
*/

static uint8_t IsAddressLegalToWrite (uint8_t address, uint8_t value)
{
	// Handle past end of writable area
	if (address > CONTROL_DESIRED_WRIST_SPEED_HIGH)
		return (FALSE);

	if ((address >= CONTROL_FINGER_POSITION_LOW) && (address <= CONTROL_FINGER_PRESSURE_HIGH))
		return (FALSE);

	// Handle read-only areas...
	if (address <= CONTROL_FIRMWARE_VERSION)
		return (FALSE);

	// Handle reserved areas...
	if ((address >= 6) && (address <= 15))
		return (FALSE);
	if ((address >= 17) && (address <= 24))
		return (FALSE);

	// Special case to prevent messing up the ID
	if ((address == CONTROL_ID) && ((value > 0xFD) || (value == 0)))
		return (FALSE);

	return (TRUE);
}


//***************************************************************************
/**
*   Check if given address is in range for reading...
*/

static uint8_t IsAddressLegalToRead (uint8_t address)
{
	// Handle past end of readable area
	if (address > MAX_CONTROL_ENTRY)
		return (FALSE);

	return (TRUE);
}


//***************************************************************************
/**
*   Read a byte from the Control Table
*/

static uint8_t ReadControlTableByte (uint8_t address)
{
	// Log ("READ - [%2d] = %3d\n", address, gControlTable [address]);
	// this function doesn't do much, but it is here in case we need to do something
	// fancy in the future...
	return (gControlTable [address]);
}


//***************************************************************************
/**
*   Write a byte to the Control Table
*/

static void WriteControlTableByte (uint8_t address, uint8_t value)
{
	// Log ("WRITE - [%2d] = %3d\n", address, value);

	// first, update the control table in RAM
	gControlTable [address] = value;

	// if the address is an EEPROM address, write the byte into EEPROM
	if (address <= MAX_EEPROM_ENTRY)
		eeprom_write_byte ((uint8_t*)(EEPROM_BASE_ADDRESS + address), value);

	// if the user updated the ID, then we need to reset our id cache
	if (address == CONTROL_ID)
		gMyId = value;

	// finally, if the baud rate is updated, we need to update the UART
	if (address == CONTROL_BAUD_RATE)
		HandleBaudRateChange (value);

	// TEMPORARY...If the user is updating one of the PID constants, update the variable.
/*	if( address == CONTROL_WRIST_KP_HIGH )
		wPk = (double)((value * 256) + gControlTable[ CONTROL_WRIST_KP_LOW ]) / 1000.0;
	if( address == CONTROL_WRIST_KD_HIGH )
		wDk = (double)((value * 256) + gControlTable[ CONTROL_WRIST_KD_LOW ]) / 1000.0;
	if( address == CONTROL_WRIST_KI_HIGH )
		wIk = (double)((value * 256) + gControlTable[ CONTROL_WRIST_KI_LOW ]) / 1000.0; */
//	if( address == CONTROL_DESIRED_FINGER_SPEED_HIGH )
//		speedOffset = (value * 256) + gControlTable[ CONTROL_DESIRED_FINGER_SPEED_LOW ];
}


//***************************************************************************
/**
*   Called when we want to dump a response packet back...
*/

static void OutputResponse (uint8_t errorCode, uint8_t forceResponse)
{
	if (errorCode != BLD_ERROR_NONE)
		Log ("ERROR Response: 0x%02x\n", errorCode);

	// we don't call this routine for READ_DATA, so we don't send a response unless
	// the status return level is set to ALL... However, if forceResponse is true, then
	// we're going to return a response anyways, because that is how PING is supposed to work
	if (!forceResponse && (gControlTable [CONTROL_STATUS_RETURN_LEVEL] != BLD_STATUS_RETURN_LEVEL_ALL))
		return;

	us_spin (gControlTable [CONTROL_RETURN_DELAY_TIME] * 2);
	UART0_PutChar (0xFF);
	UART0_PutChar (0xFF);
	UART0_PutChar (gMyId);
	UART0_PutChar (0x02);	// length for blank response is always 2
	UART0_PutChar (errorCode);
	UART0_PutChar (~(gMyId + 0x02 + errorCode));	// checksum is ~(ID + LENGTH + ERROR)
}


//***************************************************************************
/**
*   Called for a READ command
*/

static void HandleReadCommand (BLD_Packet_t *packet, uint8_t isBroadcast)
{
	uint8_t length, startAddress;
	uint8_t crc, index, byte;

	// Doing a READ_DATA via the broadcast ID is a no-op...
	if (isBroadcast)
		return;

	startAddress = packet->m_param [0];
	length = packet->m_param [1];

	// We do the loop once to check, because reading should be atomic (it either works or doesn't)
    for (index = startAddress; index < (startAddress + length); index++)
	{
		if (!IsAddressLegalToRead (index))
			{
				if (!isBroadcast)
					OutputResponse (BLD_ERROR_RANGE, FALSE);
				return;
			}
	}

//		Reponse packet looks like this:
//		HEADER HEADER ID LENGTH ERROR PARAM(s) CHECKSUM

	us_spin (gControlTable [CONTROL_RETURN_DELAY_TIME] * 2);
	crc = gMyId + length + 2;
	UART0_PutChar (0xFF);
	UART0_PutChar (0xFF);
	UART0_PutChar (gMyId);
	UART0_PutChar (length + 2);	// length for READ
	UART0_PutChar (0x00);	// error is 0, since we got here...
	for (index = startAddress; index < (startAddress + length); index++)
	{
		byte = ReadControlTableByte (index);
		UART0_PutChar (byte);
		crc += byte;
	}
	UART0_PutChar (~crc);	// checksum is ~(ID + LENGTH + ERROR + PARAMs)
}


//***************************************************************************
/**
*   Called for a WRITE command
*/

//		HEADER HEADER ID LENGTH ERROR PARAM(s) CHECKSUM

static void HandleWriteCommand (BLD_Packet_t *packet, uint8_t isBroadcast)
{
	uint8_t length, startAddress;
	uint8_t index;

	startAddress = packet->m_param [0];
	length = packet->m_length - 3;

	// We do the loop once to check, because writing should be atomic (it either works or doesn't)
    for (index = 1; index <= length; index++)
	{
		if (!IsAddressLegalToWrite (startAddress + index - 1, packet->m_param [index]))
			{
				if (!isBroadcast)
					OutputResponse (BLD_ERROR_RANGE, FALSE);
				return;
			}
	}

	// Now, we know we're legal, so do the actual write...
    for (index = 1; index <= length; index++)
        WriteControlTableByte (startAddress + index - 1, packet->m_param [index]);

	// we don't send a response packet back for a broadcast WRITE_DATA
	if (!isBroadcast)
		OutputResponse (BLD_ERROR_NONE, FALSE);
}


//***************************************************************************
/**
*   Called when a packet is parsed.
*/

static void PacketReceived (BLD_Instance_t *inst, BLD_Packet_t *packet, BLD_Error_t err)
{

	if (err == BLD_ERROR_NONE)
	{
		// if we get here, we know we have a good packet...
		uint8_t isBroadcast = packet->m_id == BLD_BROADCAST_ID;

		// we really only care about packets for us...
		if ((packet->m_id == gMyId) || isBroadcast)
		{
		    switch (packet->m_cmd)
		    {
		        case BLD_CMD_PING:
		        {
					// Log ("got PING command\n");
					OutputResponse (BLD_ERROR_NONE, TRUE);
		            break;
		        }

		        case BLD_CMD_READ:
		        {
		            // Log ("got READ command\n");
					HandleReadCommand (packet, isBroadcast);
		            break;
		        }

		        case BLD_CMD_WRITE:
		        {
		            // Log ("got WRITE command\n");
					HandleWriteCommand (packet, isBroadcast);
		            break;
		        }

		        case BLD_CMD_RESET:
		        {
		            Log ("got RESET command\n");
					InitializeControlTable (TRUE);
					OutputResponse (BLD_ERROR_NONE, FALSE);
		            break;
		        }

		        default:
		        {
					// there are other commands that we don't care about....
		            Log ("ID:0x%02x Cmd: 0x%02x *** Unknown ***\n", packet->m_id, packet->m_cmd);
		            break;
		        }
			}
		}
		// else
			// Log ("Got packet for ID: %3d\n", packet->m_id);
	}
	else if (packet->m_id == gMyId)
	{
		Log ("CRC Error\n");
		OutputResponse (BLD_ERROR_CHECKSUM, FALSE);
	}

} // PacketReceived


//***************************************************************************
/**
*   Update the control table from the sensor values
*/

static void UpdateSensorValues (void)
{
	uint16_t value;

	disableMotor();
	ms_spin (10);

	value = ADC_Read (GRIPPER_PRESSURE_CHANNEL);
	gControlTable [CONTROL_FINGER_PRESSURE_LOW] = (value & 0xFF);
	gControlTable [CONTROL_FINGER_PRESSURE_HIGH] = (value >> 8);

	value = ADC_Read (GRIPPER_POSITION_CHANNEL);
	gControlTable [CONTROL_FINGER_POSITION_LOW] = (value & 0xFF);
	gControlTable [CONTROL_FINGER_POSITION_HIGH] = (value >> 8);

	value = ADC_Read (GRIPPER_ROTATION_CHANNEL);
	gControlTable [CONTROL_WRIST_POSITION_LOW] = (value & 0xFF);
	gControlTable [CONTROL_WRIST_POSITION_HIGH] = (value >> 8);

	enableMotor();
}

static int16_t max (int16_t firstValue, int16_t secondValue)
{
	if (firstValue > secondValue)
		return (firstValue);
	else
		return (secondValue);
}

static int16_t min (int16_t firstValue, int16_t secondValue)
{
	if (firstValue < secondValue)
		return (firstValue);
	else
		return (secondValue);
}


//***************************************************************************
/**
*   Update the motors from the control table
*/

static void UpdateActuators (void)
{
	int16_t desiredFingerPosition, desiredWristPosition;
	int16_t desiredFingerSpeed, desiredWristSpeed;
	int16_t fingerPosition, wristPosition;
	int16_t offset;
	int16_t offsetDelta;
	int16_t fingerMotorSpeed, wristMotorSpeed;
	int16_t rawFingerMotorSpeed, rawWristMotorSpeed;
	double scale;

	// Get the finger position and speed requested by the controller.
	desiredFingerPosition = gControlTable [CONTROL_DESIRED_FINGER_POSITION_LOW] +
		(gControlTable [CONTROL_DESIRED_FINGER_POSITION_HIGH] * 256);
	desiredFingerSpeed = gControlTable [CONTROL_DESIRED_FINGER_SPEED_LOW] +
		(gControlTable [CONTROL_DESIRED_FINGER_SPEED_HIGH] * 256);

	// Get the actual current finger position.
	fingerPosition = gControlTable [CONTROL_FINGER_POSITION_LOW] +
		(gControlTable [CONTROL_FINGER_POSITION_HIGH] * 256);

	// Now determine how far off we are.
	offset = fingerPosition - desiredFingerPosition;

	// Determine how fast we are appropaching the desired position
	// (ie. what is the change in offset from last time to this time).
	offsetDelta = offset - lastFingerOffset;
	lastFingerOffset = offset;

	// Determine how much overall error we have had (ie. keep a running
	// sum of our offsets).
	fingerErrorAccumulation += offset;

	// Use PID to determine the correct speed at which to drive the motor.
	// The further away the actual position is from the desired position,
	// the greater the speed.  We need to bracket this so the value does
	// not exceed our possible speed values (-1023 to 1023).  If the offset
	// is small enough, we just accept it's current position.
	if( abs( offset ) > FINGER_POSITION_DEADBAND ) {
		fingerMotorSpeed = (int16_t)(fPk * (double)offset + fDk * (double)offsetDelta + fIk * (double)fingerErrorAccumulation);
		fingerMotorSpeed =  min( max( fingerMotorSpeed, -1023 ), 1023 );
	}
	else
		fingerMotorSpeed = 0;

	// Get the wrist position and speed requested by the controller.
	desiredWristPosition = gControlTable [CONTROL_DESIRED_WRIST_POSITION_LOW] +
		(gControlTable [CONTROL_DESIRED_WRIST_POSITION_HIGH] * 256);
	desiredWristSpeed = gControlTable [CONTROL_DESIRED_WRIST_SPEED_LOW] +
		(gControlTable [CONTROL_DESIRED_WRIST_SPEED_HIGH] * 256);

	// Get the actual current wrist position.
	wristPosition = gControlTable [CONTROL_WRIST_POSITION_LOW] +
		(gControlTable [CONTROL_WRIST_POSITION_HIGH] * 256);

	// Now determine how far off we are.
	offset = wristPosition - desiredWristPosition;

	// Determine how fast we are appropaching the desired position
	// (ie. what is the change in offset from last time to this time).
	offsetDelta = offset - lastWristOffset;
	lastWristOffset = offset;

	// Determine how much overall error we have had (ie. keep a running
	// sum of our offsets).
	wristErrorAccumulation += offset;

	// Use PID to determine the correct speed at which to drive the motor.
	// The further away the actual position is from the desired position,
	// the greater the speed.  We need to bracket this so the value does
	// not exceed our possible speed values (-1023 to 1023).  If the offset
	// is small enough, we just accept it's current position.
	if( abs( offset ) > WRIST_POSITION_DEADBAND ) {
		wristMotorSpeed = wPk * (double)offset + wDk * (double)offsetDelta + wIk * (double)wristErrorAccumulation;
		wristMotorSpeed =  min( max( wristMotorSpeed, -1023 ), 1023 );
	}
	else
		wristMotorSpeed = 0.0;

//	fingerMotorSpeed += ((speedOffset * 2) - 1023);
//	if( abs( fingerMotorSpeed ) < 2 )
//		fingerMotorSpeed = 0;

	// We are using 8-bit anti-phase PWM, so we need a number from 0 to 255, where 127 is off.
	// To get this we need to scale our motor speed (-1023, 1023) accordingly.  We also must
	// accomodate the motor deadband (measured symmetrically about zero).
	// The usable motor speed range is (127 - deadband) either side of STOP.
	scale = (double)(127 - FINGER_MOTOR_DEADBAND) / 1023.0;
	rawFingerMotorSpeed = (int16_t)((double)fingerMotorSpeed * scale);

	scale = (double)(127 - WRIST_MOTOR_DEADBAND) / 1023.0;
	rawWristMotorSpeed = (int16_t)((double)wristMotorSpeed * scale);

	// Now we shift the motorSpeed by the deadband amount.
	if( fingerMotorSpeed != 0 ) {
		if( fingerMotorSpeed > 0 )
			rawFingerMotorSpeed += FINGER_MOTOR_DEADBAND;
		else
			rawFingerMotorSpeed -= FINGER_MOTOR_DEADBAND;
	}

	if( wristMotorSpeed != 0 ) {
		if( wristMotorSpeed > 0 )
			rawWristMotorSpeed += WRIST_MOTOR_DEADBAND;
		else
			rawWristMotorSpeed -= WRIST_MOTOR_DEADBAND;
	}

	// Now we shift the (-127, 127) speed up to (0, 254) by adding 127.
	rawFingerMotorSpeed += 127;
	rawWristMotorSpeed += 127;

	if (gCycleCount == 0)
	{
		// Log ( "FP: %4d DFP: %4d DP: %4d FMS: %4d RFMS: %4d kD: %4d\n", fingerPosition, desiredFingerPosition, offset, fingerMotorSpeed, rawFingerMotorSpeed, (int16_t)(fDk * 1000.0) );
		// Log ( "WP: %4d DWP: %4d DP: %4d WMS: %4d RWMS: %4d\n", wristPosition, desiredWristPosition, offset, wristMotorSpeed, rawWristMotorSpeed );
	}

//	SetMotorSpeed( SPEED_OFF, rawFingerMotorSpeed );
	SetMotorSpeed( rawWristMotorSpeed, rawFingerMotorSpeed );
}

//***************************************************************************
/**
*   Update the LED from the control table
*/

static void UpdateLED (void)
{
	if (gControlTable [CONTROL_LED])
        LED_PORT &= ~LED_MASK;
	else
        LED_PORT |= LED_MASK;
}


//***************************************************************************
/**
*   Bioloid Gripper
*/

int main (void)
{
	BLD_Instance_t inst;
	tick_t previousCount;

	// initialize the hardware stuff we use
	InitTimer ();
	BLD_InitUART ();
	InitTimerUART ();
	ADC_Init (ADC_PRESCALAR_AUTO);
	InitMotors ();

	// initialize the packet parser
	BLD_Init (&inst);
	inst.m_pktRcvd = PacketReceived;

	// initialize the control table from EEPROM
	InitializeControlTable (FALSE);

	// set the LED port for output
	LED_DDR |= LED_MASK;

	// Flash the LED for 1/2 a second...
	LED_PORT &= ~LED_MASK;
	ms_spin (500);
	LED_PORT |= LED_MASK;

	Log ("*****\n");
	Log ("***** Bioloid Gripper\n");
	Log ("***** Copyright 2007 HUVrobotics\n");
	Log ("*****\n");

	gCycleCount = 0;
	while (1)	// outer loop is once every 10 ms
	{
		previousCount = gTickCount;
		while (gTickCount == previousCount)
		{
			if (UART0_IsCharAvailable ())
			{
				uint8_t ch = UART0_GetChar ();
				// Log ("Got: %3d\n", ch);
				BLD_ProcessChar (&inst, ch);
			}
		}
		// every 10 ms, we want to update the control table from the IMU,
		// and update the LED and actuators from the control table.
//		UpdateSensorValues ();
		if( sensorCycleCount == 0 )
			UpdateSensorValues ();

		UpdateActuators ();
		UpdateLED ();
		gCycleCount++;
		if (gCycleCount == 100)
			gCycleCount = 0;

		sensorCycleCount++;
		if (sensorCycleCount == 10)
			sensorCycleCount = 0;
	}

	// we'll never get here...
	return 0;
}

