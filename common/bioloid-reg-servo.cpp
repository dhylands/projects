/****************************************************************************
*
*   Copyright (c) 2009 Dave Hylands     <dhylands@gmail.com>
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
*   @file   bioloid-reg-servo.c 
*
*   @brief  Provides the description of the registers used by the
*           Dynamixel AX-12 servo.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include "bioloid-reg-servo.h"

/* ---- Public Variables -------------------------------------------------- */

BLD_Reg_t   BLD_gServoReg[] =
{
    { 0x00, "model",                BLD_REG_FLAG_16_RD,      0,    0, NULL,                 NULL },
    { 0x02, "version",              BLD_REG_FLAG_8_RD,       0,    0, NULL,                 NULL },
    { 0x03, "id",                   BLD_REG_FLAG_8_RW,       0,  253, NULL,                 NULL },
    { 0x04, "baud-rate",            BLD_REG_FLAG_8_RW,       0,  254, BLD_RegFmtBaud,       BLD_RegParseBaud },
    { 0x05, "return-delay-time",    BLD_REG_FLAG_8_RW,       0,  254, BLD_RegFmtRDT,        BLD_RegParseRDT },
    { 0x06, "cw-angle-limit",       BLD_REG_FLAG_16_RW,      0, 1023, BLD_RegFmtAngle,      BLD_RegParseAngle },
    { 0x08, "ccw-angle-limit",      BLD_REG_FLAG_16_RW,      0, 1023, BLD_RegFmtAngle,      BLD_RegParseAngle },
    { 0x0B, "high-limit-temp",      BLD_REG_FLAG_8_RW,       0,  150, BLD_RegFmtTemp,       BLD_RegParseTemp },
    { 0x0C, "low-limit-voltage",    BLD_REG_FLAG_8_RW,      50,  250, BLD_RegFmtVolt,       BLD_RegParseVolt },
    { 0x0D, "high-limit-voltage",   BLD_REG_FLAG_8_RW,      50,  250, BLD_RegFmtVolt,       BLD_RegParseVolt },
    { 0x0E, "max-torque-def",       BLD_REG_FLAG_16_RW,      0, 1023, NULL,                 NULL },
    { 0x10, "status-level",         BLD_REG_FLAG_8_RW,       0,    2, BLD_RegFmtStatusRet,  BLD_RegParseStatusRet },
    { 0x11, "alarm-led",            BLD_REG_FLAG_8_RW,       0, 0x7f, BLD_RegFmtAlarm,      NULL },
    { 0x12, "alarm-shutdown",       BLD_REG_FLAG_8_RW,       0, 0x7f, BLD_RegFmtAlarm,      NULL },
    { 0x14, "down-calib",           BLD_REG_FLAG_16_RD,      0,    0, NULL,                 NULL },
    { 0x16, "up-calib",             BLD_REG_FLAG_16_RD,      0,    0, NULL,                 NULL },
    { 0x18, "torque-enable",        BLD_REG_FLAG_8_RW,       0,    1, BLD_RegFmtOnOff,      BLD_RegParseOnOff },
    { 0x19, "led",                  BLD_REG_FLAG_8_RW,       0,    1, BLD_RegFmtOnOff,      BLD_RegParseOnOff },
    { 0x1A, "cw-comp-margin",       BLD_REG_FLAG_8_RW,       0,  254, NULL,                 NULL },
    { 0x1B, "ccw-comp-margin",      BLD_REG_FLAG_8_RW,       0,  254, NULL,                 NULL },
    { 0x1C, "cw-comp-slope",        BLD_REG_FLAG_8_RW,       1,  254, NULL,                 NULL },
    { 0x1D, "ccw-comp-slope",       BLD_REG_FLAG_8_RW,       1,  254, NULL,                 NULL },
    { 0x1E, "goal-position",        BLD_REG_FLAG_16_RW,      0, 1023, BLD_RegFmtAngle,      BLD_RegParseAngle },
    { 0x20, "moving-speed",         BLD_REG_FLAG_16_RW,      0, 1023, BLD_RegFmtVelocity,   BLD_RegParseVelocity },
    { 0x22, "max-torque",           BLD_REG_FLAG_16_RW,      0, 1023, NULL,                 NULL },
    { 0x24, "present-position",     BLD_REG_FLAG_16_RD,      0,    0, BLD_RegFmtAngle,      BLD_RegParseAngle },
    { 0x26, "present-speed",        BLD_REG_FLAG_16_RD,      0,    0, BLD_RegFmtVelocity,   NULL },
    { 0x28, "present-load",         BLD_REG_FLAG_16_RD,      0,    0, BLD_RegFmtLoad,       NULL },
    { 0x2A, "present-voltage",      BLD_REG_FLAG_8_RD,       0,    0, BLD_RegFmtVolt,       BLD_RegParseVolt },
    { 0x2B, "present-temp",         BLD_REG_FLAG_8_RD,       0,    0, BLD_RegFmtTemp,       BLD_RegParseTemp },
    { 0x2C, "reg-instruction",      BLD_REG_FLAG_8_RW,       0,    1, BLD_RegFmtOnOff,      BLD_RegParseOnOff },
    { 0x2E, "moving",               BLD_REG_FLAG_8_RD,       0,    0, NULL,                 NULL },
    { 0x2F, "lock",                 BLD_REG_FLAG_8_RW,       0,    1, NULL,                 NULL },
    { 0x30, "punch",                BLD_REG_FLAG_16_RW,      0, 1023, NULL,                 NULL },
    {    0, NULL }
};

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup XXX
 * @{
 */

//***************************************************************************
/**
*   Description goes here
*/

/** @} */

