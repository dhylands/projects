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
/**
*
*   @file   bioloidFoot.h
*
*   @brief  Contains definitions for a bioloid Foot Pressure sensor.
*
****************************************************************************/

#if !defined( BIOLOID_FOOT_H )
#define BIOLOID_FOOT_H	   /**< Include Guard							 */

#define TRUE 1
#define FALSE 0

#define BLD_STATUS_RETURN_LEVEL_NONE 0    // Don't respond
#define BLD_STATUS_RETURN_LEVEL_READ_DATA 1    // respond only to READ_DATA
#define BLD_STATUS_RETURN_LEVEL_ALL 2    // Respond to everything

#define EEPROM_BASE_ADDRESS 11
#define EEPROM_ELEMENT_COUNT 7

#define CONTROL_MODEL_NUMBER_LOW 0
#define CONTROL_MODEL_NUMBER_HIGH 1
#define CONTROL_FIRMWARE_VERSION 2
#define CONTROL_ID 3
#define CONTROL_BAUD_RATE 4
#define CONTROL_RETURN_DELAY_TIME 5
#define CONTROL_STATUS_RETURN_LEVEL 16

#define MAX_EEPROM_ENTRY 23

#define CONTROL_LED 25
#define CONTROL_FRONT_LEFT_LOW 26
#define CONTROL_FRONT_LEFT_HIGH 27
#define CONTROL_FRONT_RIGHT_LOW 28
#define CONTROL_FRONT_RIGHT_HIGH 29
#define CONTROL_REAR_LEFT_LOW 30
#define CONTROL_REAR_LEFT_HIGH 31
#define CONTROL_REAR_RIGHT_LOW 32
#define CONTROL_REAR_RIGHT_HIGH 33

#define NUM_PRESSURE_CONTROL_ENTRIES 34
#define MAX_PRESSURE_CONTROL_ENTRY 33

#define CONTROL_LED 25
#define CONTROL_FORWARDS_ACCEL_LOW 26
#define CONTROL_FORWARDS_ACCEL_HIGH 27
#define CONTROL_SIDEWAYS_ACCEL_LOW 28
#define CONTROL_SIDEWAYS_ACCEL_HIGH 29
#define CONTROL_VERTICAL_ACCEL_LOW 30
#define CONTROL_VERTICAL_ACCEL_HIGH 31
#define CONTROL_PITCH_RATE_LOW 32
#define CONTROL_PITCH_RATE_HIGH 33
#define CONTROL_ROLL_RATE_LOW 34
#define CONTROL_ROLL_RATE_HIGH 35
#define CONTROL_YAW_RATE_LOW 36
#define CONTROL_YAW_RATE_HIGH 37

#define NUM_IMU_CONTROL_ENTRIES 38
#define MAX_IMU_CONTROL_ENTRY 37

#define NUM_CONTROL_ENTRIES NUM_PRESSURE_CONTROL_ENTRIES
#define MAX_CONTROL_ENTRY MAX_PRESSURE_CONTROL_ENTRY

// #define NUM_CONTROL_ENTRIES NUM_PRESSURE_CONTROL_ENTRIES
// #define MAX_CONTROL_ENTRY MAX_PRESSURE_CONTROL_ENTRY

//		The following defines are for the IMU orientation that MicroRaptor uses...
#define ADC_FORWARDS_ACCEL_CHANNEL 2
#define ADC_SIDEWAYS_ACCEL_CHANNEL 1
#define ADC_VERTICAL_ACCEL_CHANNEL 0

#define ADC_PITCH_RATE_CHANNEL 6
#define ADC_ROLL_RATE_CHANNEL 7
#define ADC_YAW_RATE_CHANNEL 3

#define PRESSURE_FRONT_RIGHT_CHANNEL 3
#define PRESSURE_FRONT_LEFT_CHANNEL 0
#define PRESSURE_REAR_LEFT_CHANNEL 1
#define PRESSURE_REAR_RIGHT_CHANNEL 2

#endif /* BIOLOID_FOOT_H */
