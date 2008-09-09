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
*   @file   bioloid-io.h
*
*   @brief  Contains definitions for a bioloid I/O module.
*
****************************************************************************/

#if !defined( BIOLOID_IO_H )
#define BIOLOID_IO_H	   /**< Include Guard							 */

#define TRUE 1
#define FALSE 0

#define BLD_DEFAULT_ID 122

#define BLD_STATUS_RETURN_LEVEL_NONE 0    // Don't respond
#define BLD_STATUS_RETURN_LEVEL_READ_DATA 1    // respond only to READ_DATA
#define BLD_STATUS_RETURN_LEVEL_ALL 2    // Respond to everything

#define EEPROM_BASE_ADDRESS 11
#define EEPROM_ELEMENT_COUNT 12

#define CONTROL_MODEL_NUMBER_LOW 0
#define CONTROL_MODEL_NUMBER_HIGH 1
#define CONTROL_FIRMWARE_VERSION 2
#define CONTROL_ID 3
#define CONTROL_BAUD_RATE 4
#define CONTROL_RETURN_DELAY_TIME 5

#define CONTROL_DIGITAL_0_DIR 6
#define CONTROL_DIGITAL_1_DIR 7
#define CONTROL_DIGITAL_2_DIR 8
#define CONTROL_DIGITAL_3_DIR 9

#define CONTROL_PWM_TYPE 10

#define CONTROL_STATUS_RETURN_LEVEL 16

#define MAX_EEPROM_ENTRY 23

#define CONTROL_LED 25
#define CONTROL_ADC_0_LOW 26
#define CONTROL_ADC_0_HIGH 27
#define CONTROL_ADC_1_LOW 28
#define CONTROL_ADC_1_HIGH 29
#define CONTROL_ADC_2_LOW 30
#define CONTROL_ADC_2_HIGH 31
#define CONTROL_ADC_3_LOW 32
#define CONTROL_ADC_3_HIGH 33
#define CONTROL_ADC_4_LOW 34
#define CONTROL_ADC_4_HIGH 35
#define CONTROL_ADC_5_LOW 36
#define CONTROL_ADC_5_HIGH 37

#define CONTROL_DIGITAL_0 38
#define CONTROL_DIGITAL_1 39
#define CONTROL_DIGITAL_2 40
#define CONTROL_DIGITAL_3 41

#define CONTROL_MOTOR_A_SPEED 42
#define CONTROL_MOTOR_A_DIR 43
#define CONTROL_MOTOR_B_SPEED 44
#define CONTROL_MOTOR_B_DIR 45

#define NUM_CONTROL_ENTRIES 46
#define MAX_CONTROL_ENTRY 45

#define DIGITAL_INPUT_DIR 0
#define DIGITAL_OUTPUT_DIR 1

#define PWM_SIGN_MAGNITUDE 0
#define PWM_LOCKED_ANTIPHASE 1

#endif /* BIOLOID_IO_H */
