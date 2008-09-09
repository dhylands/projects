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
*   @file   compassTest.h
*
*   @brief  Contains definitions for an ATmega168 interfacing to a Honeywell HMC6364
*		3-axis solid state tilt compensated compass over I2C
*
****************************************************************************/

#if !defined( COMPASS_TEST_H )
#define COMPASS_TEST_H	   /**< Include Guard							 */

#define TRUE 1
#define FALSE 0

#define COMPASS_ADDRESS_WRITE 0x32
#define COMPASS_ADDRESS_READ 0x33

#define COMPASS_RAW_ACCEL_COMMAND 0x40
#define COMPASS_RAW_MAG_COMMAND 0x45
#define COMPASS_HEADING_COMMAND 0x50
#define COMPASS_TILT_COMMAND 0x55

#define COMPASS_POST_OP_MODE_COMMAND 0x65

#define COMPASS_CALIBRATION_COMMAND 0x71
#define COMPASS_END_CALIBRATION_COMMAND 0x7E

#define COMPASS_READ_EEPROM_COMMAND 0xE1
#define COMPASS_WRITE_EEPROM_COMMAND 0xF1

#endif /* COMPASS_TEST_H */
