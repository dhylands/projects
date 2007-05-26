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

#if !defined( BIOLOID_GRIPPER_H )
#define BIOLOID_GRIPPER_H	   /**< Include Guard							 */

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
#define CONTROL_DESIRED_FINGER_POSITION_LOW 26
#define CONTROL_DESIRED_FINGER_POSITION_HIGH 27
#define CONTROL_DESIRED_FINGER_SPEED_LOW 28
#define CONTROL_DESIRED_FINGER_SPEED_HIGH 29
#define CONTROL_FINGER_POSITION_LOW 30
#define CONTROL_FINGER_POSITION_HIGH 31
#define CONTROL_FINGER_PRESSURE_LOW 32
#define CONTROL_FINGER_PRESSURE_HIGH 33
#define CONTROL_DESIRED_WRIST_POSITION_LOW 34
#define CONTROL_DESIRED_WRIST_POSITION_HIGH 35
#define CONTROL_DESIRED_WRIST_SPEED_LOW 36
#define CONTROL_DESIRED_WRIST_SPEED_HIGH 37
#define CONTROL_WRIST_POSITION_LOW 38
#define CONTROL_WRIST_POSITION_HIGH 39

#define NUM_CONTROL_ENTRIES 40
#define MAX_CONTROL_ENTRY 39

#define GRIPPER_PRESSURE_CHANNEL 2
#define GRIPPER_POSITION_CHANNEL 7
#define GRIPPER_ROTATION_CHANNEL 3

#define WRIST_ROTATION_STOPPED 0
#define WRIST_ROTATION_FORWARDS 1
#define WRIST_ROTATION_BACKWARDS 2

#define WRIST_ROTATION_THRESHOLD 500
#define WRIST_ROTATION_HIGH 1
#define WRIST_ROTATION_LOW 0

#define WRIST_ROTATION_TICK_ANGLE 10

#endif /* BIOLOID_GRIPPER_H */
