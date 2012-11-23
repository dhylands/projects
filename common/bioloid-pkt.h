/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
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
*   @file   bioloid-pkt.h
*
*   @brief  Contains definitions for the bioloid packet parser.
*
****************************************************************************/

#if !defined( BIOLOID_PKT_H )
#define BIOLOID_PKT_H       /**< Include Guard                             */

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>
#include "Config.h"

/**
 * @addtogroup bioloid
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

#define BLD_STATE_IDLE          0   ///< We're waiting for the beginning of the packet
#define BLD_STATE_1ST_FF_RCVD   1   ///< We've received the 1st 0xFF
#define BLD_STATE_2ND_FF_RCVD   2   ///< We've received the 2nd 0xFF
#define BLD_STATE_ID_RCVD       4   ///< We've received the ID
#define BLD_STATE_LENGTH_RCVD   5   ///< We've received the length
#define BLD_STATE_COMMAND_RCVD  6   ///< We've received the command
#define BLD_STATE_SYNC_WRITE_CMD_RCVD       7   ///< We've received a SYNC_WRITE command
#define BLD_STATE_SYNC_WRITE_ADDRESS_RCVD   8   ///< We've received the offset of a SYNC_WRITE
#define BLD_STATE_SYNC_WRITE_LEN_RCVD       9   ///< We've received the length per ID of a SYNC_WRITE
#define BLD_STATE_SYNC_WRITE_STORE_DATA    10   ///< We've received our ID for a SYNC_WRITE
#define BLD_STATE_SYNC_WRITE_SKIP_DATA     11   ///< We've received somebody else's ID for a SYNC_WRITE

#if !defined( CFG_BLD_MAX_PARAM )
#   define  CFG_BLD_MAX_PARAM   8
#endif
#define BLD_MAX_PARAM       CFG_BLD_MAX_PARAM

#define BLD_BROADCAST_ID    0xFE

typedef uint8_t     BLD_Error_t;
typedef uint8_t     BLD_State_t;
typedef uint8_t     BLD_ID_t;
typedef uint8_t     BLD_Length_t;
typedef uint8_t     BLD_Command_t;

#define BLD_ERROR_RESERVED      0x80    ///< Reserved - set to zero
#define BLD_ERROR_INSTRUCTION   0x40    ///< Undefined instruction
#define BLD_ERROR_OVERLOAD      0x20    ///< Max torque can't control applied load
#define BLD_ERROR_CHECKSUM      0x10    ///< Checksum of instruction packet incorrect
#define BLD_ERROR_RANGE         0x08    ///< Instruction is out of range
#define BLD_ERROR_OVERHEATING   0x04    ///< Internal temperature is too high
#define BLD_ERROR_ANGLE_LIMIT   0x02    ///< Goal position is outside of limit range
#define BLD_ERROR_INPUT_VOLTAGE 0x01    ///< Input voltage out of range
#define BLD_ERROR_NONE          0x00    ///< No Error

typedef struct
{
    BLD_ID_t        m_id;
    BLD_Length_t    m_length;
    BLD_Command_t   m_cmd;
    uint8_t         m_param[ BLD_MAX_PARAM ];
    uint8_t         m_crc;                      // Placeholder for CRC

} BLD_Packet_t;

struct BLD_Instance_s;

typedef struct BLD_Instance_s BLD_Instance_t;

typedef void (*BLD_PacketReceived)( BLD_Instance_t *instance, BLD_Packet_t *pkt, BLD_Error_t err );
typedef void (*BLD_SendChar)( uint8_t ch );

struct BLD_Instance_s
{
    BLD_ID_t            m_id;       ///< Our ID on the Bioloid bus, 0xFF if we're just monitoring
    BLD_State_t         m_state;
    BLD_Length_t        m_paramIdx; 
    BLD_Packet_t        m_pkt;      ///< Contains the packet that was actually received
    BLD_PacketReceived  m_pktRcvd;  ///< Ptr to Fn called when a packet is successfully received
    BLD_SendChar        m_sendChar; ///< Ptr to Fn called to send out a character

    BLD_Length_t        m_syncWriteLen; ///< Length of data for each ID in a SYNC_WRITE
    BLD_Length_t        m_syncWriteIdx; ///< Current index if data for each ID in a SYNC_WRITE

    uint8_t             m_logPacket;  ///< Causes Log statements for each packet sent
};

#define BLD_LogPacket( inst, fmt, args... ) do { if ( inst->m_logPacket ) { Log( fmt, ## args ); }} while (0)

/*
 * Control Table
 */

// EEPROM Area

#define BLD_P_MODEL_NUMBER_L            0
#define BLD_P_MODOEL_NUMBER_H           1
#define BLD_P_VERSION                   2
#define BLD_P_ID                        3
#define BLD_P_BAUD_RATE                 4
#define BLD_P_RETURN_DELAY_TIME         5
#define BLD_P_CW_ANGLE_LIMIT_L          6
#define BLD_P_CW_ANGLE_LIMIT_H          7
#define BLD_P_CCW_ANGLE_LIMIT_L         8
#define BLD_P_CCW_ANGLE_LIMIT_H         9
#define BLD_P_RESERVED_10               10
#define BLD_P_LIMIT_TEMPERATURE         11
#define BLD_P_DOWN_LIMIT_VOLTAGE        12
#define BLD_P_UBLD_P_LIMIT_VOLTAGE          13
#define BLD_P_MAX_TORQUE_L              14
#define BLD_P_MAX_TORQUE_H              15
#define BLD_P_RETURN_LEVEL              16
#define BLD_P_ALARM_LED                 17
#define BLD_P_ALARM_SHUTDOWN            18
#define BLD_P_RESERVED_19               19
#define BLD_P_DOWN_CALIBRATION_L        20
#define BLD_P_DOWN_CALIBRATION_H        21
#define BLD_P_UBLD_P_CALIBRATION_L          22
#define BLD_P_UBLD_P_CALIBRATION_H          23

// RAM Area

#define BLD_P_TORQUE_ENABLE             24
#define BLD_P_LED                       25
#define BLD_P_CW_COMPLIANCE_MARGIN      26
#define BLD_P_CCW_COMPLIANCE_MARGIN     27
#define BLD_P_CW_COMPLIANCE_SLOPE       28
#define BLD_P_CCW_COMPLIANCE_SLOPE      29
#define BLD_P_GOAL_POSITION_L           30
#define BLD_P_GOAL_POSITION_H           31
#define BLD_P_GOAL_SPEED_L              32
#define BLD_P_GOAL_SPEED_H              33
#define BLD_P_TORQUE_LIMIT_L            34
#define BLD_P_TORQUE_LIMIT_H            35
#define BLD_P_PRESENT_POSITION_L        36
#define BLD_P_PRESENT_POSITION_H        37
#define BLD_P_PRESENT_SPEED_L           38
#define BLD_P_PRESENT_SPEED_H           39
#define BLD_P_PRESENT_LOAD_L            40
#define BLD_P_PRESENT_LOAD_H            41
#define BLD_P_PRESENT_VOLTAGE           42
#define BLD_P_PRESENT_TEMPERATURE       43
#define BLD_P_REGISTERED_INSTRUCTION    44
#define BLD_P_RESERVED_45               45
#define BLD_P_MOVING                    46
#define BLD_P_LOCK                      47
#define BLD_P_PUNCH_L                   48
#define BLD_P_PUNCH_H                   49

// Instructions

#define BLD_CMD_PING        0x01
#define BLD_CMD_READ        0x02
#define BLD_CMD_WRITE       0x03
#define BLD_CMD_REG_WRITE   0x04
#define BLD_CMD_ACTION      0x05
#define BLD_CMD_RESET       0x06

#define BLD_CMD_SYNC_WRITE  0x83

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void BLD_Init( BLD_Instance_t *instance );
void BLD_ProcessChar( BLD_Instance_t *instance, uint8_t ch );
void BLD_SendStatus( BLD_Instance_t *instance, uint8_t error, const void *param, uint8_t paramLen );

/** @} */

#endif /* BIOLOID_PKT_H */

