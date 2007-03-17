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

#include "Delay.h"
#include "adc.h"
#include "Log.h"
#include "Timer.h"
#include "Timer-UART.h"
#include "bioloid-uart.h"
#include "bioloid-pkt.h"
#include "bioloidIMU.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

//	Our LED is connected to PD3

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_MASK (1 << 5)

/* ---- Private Variables ------------------------------------------------ */

uint8_t gMyId;
uint8_t gControlTable [NUM_IMU_CONTROL_ENTRIES];


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
	gControlTable [CONTROL_MODEL_NUMBER_LOW] = 10;
	gControlTable [CONTROL_MODEL_NUMBER_HIGH] = 67;
	gControlTable [CONTROL_FIRMWARE_VERSION] = 1;
	gControlTable [CONTROL_ID] = 120;
	gControlTable [CONTROL_BAUD_RATE] = 1;
	gControlTable [CONTROL_RETURN_DELAY_TIME] = 250;
	gControlTable [CONTROL_STATUS_RETURN_LEVEL] = BLD_STATUS_RETURN_LEVEL_ALL;

	// if the magic constant is there, the rest of the values are probably okay.
	// if its not there, or we're doing a hard reset, ignore what is in the EEPROM,
	// and over-write it with the hardware default values...
	eepromSize = eeprom_read_byte ((uint8_t*)(EEPROM_BASE_ADDRESS - 1));
	if (!forceOverwrite && (eepromSize == EEPROM_ELEMENT_COUNT))
	{
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
	if (address > CONTROL_LED)
		return (FALSE);

	// Handle read-only areas...
	if (address <= CONTROL_FIRMWARE_VERSION)
		return (FALSE);

	// Handle reserved areas...
	if ((address >= 6) && (address <= 15))
		return (FALSE);
	if ((address >= 17) && (address <= 23))
		return (FALSE);
	if (address == 24)
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
	if (address > MAX_IMU_CONTROL_ENTRY)
		return (FALSE);

	return (TRUE);
}


//***************************************************************************
/**
*   Read a byte from the Control Table
*/

static uint8_t ReadControlTableByte (uint8_t address)
{
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
		//else
		//	Log ("Got packet for ID: %3d\n", packet->m_id);
	}
	else
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

	value = ADC_Read (ADC_FORWARDS_ACCEL_CHANNEL);
	gControlTable [CONTROL_FORWARDS_ACCEL_LOW] = (value & 0xFF);
	gControlTable [CONTROL_FORWARDS_ACCEL_HIGH] = (value >> 8);

	value = ADC_Read (ADC_SIDEWAYS_ACCEL_CHANNEL);
	gControlTable [CONTROL_SIDEWAYS_ACCEL_LOW] = (value & 0xFF);
	gControlTable [CONTROL_SIDEWAYS_ACCEL_HIGH] = (value >> 8);

	value = ADC_Read (ADC_VERTICAL_ACCEL_CHANNEL);
	gControlTable [CONTROL_VERTICAL_ACCEL_LOW] = (value & 0xFF);
	gControlTable [CONTROL_VERTICAL_ACCEL_HIGH] = (value >> 8);

	value = ADC_Read (ADC_PITCH_RATE_CHANNEL);
	gControlTable [CONTROL_PITCH_RATE_LOW] = (value & 0xFF);
	gControlTable [CONTROL_PITCH_RATE_HIGH] = (value >> 8);

	value = ADC_Read (ADC_ROLL_RATE_CHANNEL);
	gControlTable [CONTROL_ROLL_RATE_LOW] = (value & 0xFF);
	gControlTable [CONTROL_ROLL_RATE_HIGH] = (value >> 8);

	value = ADC_Read (ADC_YAW_RATE_CHANNEL);
	gControlTable [CONTROL_YAW_RATE_LOW] = (value & 0xFF);
	gControlTable [CONTROL_YAW_RATE_HIGH] = (value >> 8);
}


//***************************************************************************
/**
*   Update the LED from the control table
*/

static void UpdateLED (void)
{
	if (gControlTable [CONTROL_LED])
        LED_PORT |=  LED_MASK;
	else
        LED_PORT &= ~LED_MASK;
}


//***************************************************************************
/**
*   Dump the IMU part of the control table over the log (DEBUG function)
*/

static void LogControlTableIMUValues (void)
{
	uint16_t value;

	Log ("Control Table IMU Values\n");

	value = gControlTable [CONTROL_FORWARDS_ACCEL_LOW];
	value += (256 * gControlTable [CONTROL_FORWARDS_ACCEL_HIGH]);
	Log (" Forwards Accel: %4d\n", value);

	value = gControlTable [CONTROL_SIDEWAYS_ACCEL_LOW];
	value += (256 * gControlTable [CONTROL_SIDEWAYS_ACCEL_HIGH]);
	Log (" Sideways Accel: %4d\n", value);

	value = gControlTable [CONTROL_VERTICAL_ACCEL_LOW];
	value += (256 * gControlTable [CONTROL_VERTICAL_ACCEL_HIGH]);
	Log (" Vertical Accel: %4d\n", value);

	value = gControlTable [CONTROL_PITCH_RATE_LOW];
	value += (256 * gControlTable [CONTROL_PITCH_RATE_HIGH]);
	Log (" Pitch: %4d\n", value);

	value = gControlTable [CONTROL_ROLL_RATE_LOW];
	value += (256 * gControlTable [CONTROL_ROLL_RATE_HIGH]);
	Log (" Roll: %4d\n", value);

	value = gControlTable [CONTROL_YAW_RATE_LOW];
	value += (256 * gControlTable [CONTROL_YAW_RATE_HIGH]);
	Log (" Yaw: %4d\n\n", value);

}


//***************************************************************************
/**
*   Dump the control table over the log (DEBUG function)
*/

static void LogControlTable (void)
{
	uint8_t index;

	Log ("Control Table IMU\n");

	for (index = 0; index <= sizeof (gControlTable); index++)
		Log ("%2d : %3d\n", index, gControlTable [index]);
	Log ("Done\n");
}


//***************************************************************************
/**
*   Bioloid IMU
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

	// initialize the packet parser
	BLD_Init (&inst);
	inst.m_pktRcvd = PacketReceived;

	// initialize the control table from EEPROM
	InitializeControlTable (FALSE);

	// set the LED port for output
	LED_DDR |= LED_MASK;

	// Flash the LED for 1/2 a second...
	LED_PORT |=  LED_MASK;
	ms_spin (500);
	LED_PORT &= ~LED_MASK;

	Log ("*****\n");
	Log ("***** Bioloid IMU Sensor\n");
	Log ("*****\n");

	while (1)	// outer loop is once every 10 ms
	{
		previousCount = gTickCount;
		while (gTickCount == previousCount)
		{
			if (UART0_IsCharAvailable ())
			{
				uint8_t ch = UART0_GetChar ();
				//Log ("Got: %3d\n", ch);
				BLD_ProcessChar (&inst, ch);
			}
		}
		// every 10 ms, we want to update the control table from the IMU,
		// and update the LED from the control table.
		UpdateSensorValues ();
		UpdateLED ();
	}

	// we'll never get here...
	return 0;
}

