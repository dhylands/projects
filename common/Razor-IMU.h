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
*   @file   Razor-9DOF.h
*
*   @brief  Contains definitions for the Razoe-9DOF parser.
*
****************************************************************************/

#if !defined( RAZOR_IMU_H )
#define RAZOR_IMU_H       /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#define RAZOR_IMU_STATE_IDLE        0
#define RAZOR_IMU_STATE_COLLECTING  1

/* ---- Constants and Types ---------------------------------------------- */

#define RAZOR_IMU_LINE_LEN      32

struct RAZOR_IMU_s;
typedef struct RAZOR_IMU_s RAZOR_IMU_t;

typedef uint8_t     RAZOR_IMU_State_t;

typedef void    (*RAZOR_IMU_EulerCallback_t)( RAZOR_IMU_t *instance, int16_t roll, int16_t pitch, int16_t yaw );

struct RAZOR_IMU_s
{
    RAZOR_IMU_State_t   m_state;

    uint8_t             m_line[RAZOR_IMU_LINE_LEN];
    uint8_t             m_index;
    uint8_t             m_debug;

    RAZOR_IMU_EulerCallback_t   m_eulerCallback;
};

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

#if defined( __cplusplus )
extern "C" {
#endif

void RAZOR_IMU_Init( RAZOR_IMU_t *instance );
void RAZOR_IMU_ProcessChar( RAZOR_IMU_t *instance, uint8_t ch );

#if defined( __cplusplus )
}
#endif


#endif /* RAZOR_IMU_H */

