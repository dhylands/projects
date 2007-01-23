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

/* ---- Constants and Types ---------------------------------------------- */

#define BLD_STATE_IDLE          0   ///< We're waiting for the beginning of the packet
#define BLD_STATE_1ST_FF_RCVD   1   ///< We've received the 1st 0xFF
#define BLD_STATE_2ND_FF_RCVD   2   ///< We've received the 2nd 0xFF
#define BLD_STATE_ID_RCVD       4   ///< We've received the ID
#define BLD_STATE_LENGTH_RCVD   5   ///< We've received the length
#define BLD_STATE_COMMAND_RCVD  6   ///< We've received the command

#define BLD_MAX_PARAM       8

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

struct BLD_Instance_s
{
    BLD_ID_t            m_id;       ///< Our ID on the Bioloid bus, 0xFF if we're just monitoring
    BLD_State_t         m_state;
    BLD_Length_t        m_paramIdx; 
    BLD_Packet_t        m_pkt;
    BLD_PacketReceived  m_pktRcvd;

};


/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

void BLD_Init( BLD_Instance_t *instance );
void BLD_ProcessChar( BLD_Instance_t *instance, uint8_t ch );

/** @} */

#endif /* BIOLOID_PKT_H */

