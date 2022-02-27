/****************************************************************************
 *
 *   Copyright (c) 2010 Dave Hylands     <dhylands@gmail.com>
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
 *   @file   Razor-9DOF.c
 *
 *   @brief  Implements the Razor 9DOF parser.
 *
 *   The data which comes from the sensor when its running the code from
 *   http://code.google.com/p/sf9domahrs/ looks like:
 *
 *   Sparkfun 9DOF Razor AHRS
 *   227
 *   330
 *   284
 *   27
 *   210
 *   -215
 *   !ANG:-0.72,-1.35,0.43
 *   !ANG:-0.08,-2.33,0.81
 *   !ANG:0.42,-3.20,1.13
 *   !ANG:0.88,-4.06,1.42
 *   !ANG:1.37,-4.77,1.67
 *
 *****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Log.h"
#include "Razor-IMU.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

#define IMU_DEBUG(inst, fmt, args...) \
    do {                              \
        if (inst->m_debug) {          \
            Log(fmt, ##args);         \
        }                             \
    } while (0)

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
 *   Parses an angle from a string, and returns the angle multiplied by
 *   one hundred (so no floating point is required).
 */

static char* ParseAngle(char* s, int16_t* angle) {
    int negative = 0;
    char* nextStr;
    int16_t num;

    if (s == NULL) {
        return NULL;
    }

    if ((nextStr = strchr(s, ',')) != NULL) {
        *nextStr++ = '\0';
    }

    if (*s == '-') {
        negative = 1;
        s++;
    }
    num = 0;
    while ((*s >= '0') && (*s <= '9')) {
        num *= 10;
        num += (*s - '0');
        s++;
    }

    if (*s == '.') {
        int i;

        s++;
        for (i = 0; i < 2; i++) {
            num *= 10;
            if ((*s >= '0') && (*s <= '9')) {
                num += (*s - '0');
                s++;
            }
        }
    } else {
        num *= 100;
    }
    if (negative) {
        *angle = -num;
    } else {
        *angle = num;
    }

    return nextStr;
}

//***************************************************************************
/**
 *   Parses a "Euler" line, which is of the format:
 *
 *   !ANG:1.37,-4.77,1.67
 *
 *   where the angles are roll, pitch, and yaw.
 */

static void ParseEulerLine(RAZOR_IMU_t* instance) {
    char* s;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;

    // !ANG: was already at the beginning of the line

    s = &instance->m_line[5];

    s = ParseAngle(s, &roll);
    s = ParseAngle(s, &pitch);
    s = ParseAngle(s, &yaw);

    if (instance->m_eulerCallback) {
        instance->m_eulerCallback(instance, roll, pitch, yaw);
    }
}

//***************************************************************************
/**
 *   Initializes a BLD_State_t structure.
 */

void RAZOR_IMU_Init(RAZOR_IMU_t* instance) {
    memset(instance, 0, sizeof(*instance));

}  // BLD_Init

//***************************************************************************
/**
 *   Runs the state machine for parsing the stream from the Razor 9DOF IMU.
 */

void RAZOR_IMU_ProcessChar(RAZOR_IMU_t* instance, uint8_t ch) {
    RAZOR_IMU_State_t nextState = instance->m_state;

    switch (nextState) {
        case RAZOR_IMU_STATE_IDLE: {
            IMU_DEBUG(instance, "State: IDLE ch: '%c'\n", ch);

            if (ch == '!') {
                instance->m_line[0] = ch;
                instance->m_index = 1;
                nextState = RAZOR_IMU_STATE_COLLECTING;
            }
            break;
        }

        case RAZOR_IMU_STATE_COLLECTING: {
            IMU_DEBUG(instance, "State: COLLECTING ch: '%c'\n", ch);

            if (ch == '\r') {
                instance->m_line[instance->m_index] = '\0';
                if (strncmp(instance->m_line, "!ANG:", 5) == 0) {
                    // We've collected a "Euler" line

                    ParseEulerLine(instance);
                }
                instance->m_index = 0;
                nextState = RAZOR_IMU_STATE_IDLE;
                break;
            }

            if (instance->m_index < (sizeof(instance->m_line) - 1)) {
                instance->m_line[instance->m_index++] = ch;
            }
            break;
        }

        default: {
            IMU_DEBUG(instance, "State: ??? ch: '%c'\n", ch);
            break;
        }
    }

    instance->m_state = nextState;
}
