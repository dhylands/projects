/****************************************************************************
*
*   Copyright (c) 2007 Dave Hylands     <dhylands@gmail.com>
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
*   @file   PacketProtocol.h
*
*   @brief  This file contains the interface for the packet protocol.
*
****************************************************************************/
/**
*   @defgroup   PacketProtocol   Allows sending/receiving packets
*
*   @brief      Allows packets to be sent or received via the serial port.
*
****************************************************************************/

#if !defined( PKT_PACKET_H )
#define PKT_PACKET_H                   ///< Include Guard

/* ---- Include Files ---------------------------------------------------- */

/**
 * @addtogroup PacketProtocol
 * @{
 */

#if defined( __cplusplus )
extern "C"
{
#endif

/**
 *  The layout of a packet looks like this:
 *
 *  ETX LEN DATA ... CRC STX
 *
 *  Currently, there is no ACK/NAK to the protocol as it is assumed that the
 *  the data is trvelling over a directly connected serial line.
 *
 *  In the event that this is done in the future, then intent is to use
 *  the top two bits of the length as a sequence number and use reserved
 *  values of the packet length to indicate ACK/NAK/SYN.
 *
 *  The CRC is calculated using CRC-8 (same as with the PEC in SMBus).
 *
 *  This protocol has two "modes", text mode and packet mode. When in text
 *  mode, any characters which are received are passed on directly, which 
 *  allows the underlying microprocessor to employ simple logging.
 */

/* ---- Constants and Types ---------------------------------------------- */

// Some Character codes used in the protocol

#define PKT_STX 0x82    /**< Used to deliniate the end of a packet and start of text mode           */
#define PKT_ETX 0x83    /**< Used to deliniate the end of text mode and the start of packet mode    */


/**
 *  Since we're using this to "wrap" i2c commands which have a maximum
 *  payload of 32 bytes, we pick a size just slightly bigger to allow for
 *  for the extra stuff like addressing.
 */

#define PKT_MAX_DATA_LEN    40

/**
 *  Describes a packet.
 */

typedef struct
{
    unsigned char   len;
    unsigned char   crc;
    unsigned char   data[ PKT_MAX_DATA_LEN ];

} PKT_Packet;

/**
 *  Type which describes a function called when a character is received in
 *  text mode.
 */

typedef void (*PKT_TextCharFuncPtr)( unsigned char ch, void *userData );

/**
 *  Type which describes a function called when a packet is received.
 */

typedef void (*PKT_PacketReceivedFuncPtr)( PKT_Packet *packet, void *userData );

/**
 *  Type which describes a function called to send a packet.
 */

typedef void (*PKT_SendCharFuncPtr)( unsigned char ch, void *userData );

/* ---- Variable Externs ------------------------------------------------- */

/**
 *  Pointer to a function which is called when a character is received in
 *  text mode.
 */

extern  PKT_TextCharFuncPtr         PKT_TextChar;

/**
 *  Pointer to a function which is called when a packet is received.
 */

extern  PKT_PacketReceivedFuncPtr   PKT_PacketReceived;

/**
 *  Pointer to a function which sends a character over the serial line.
 */

extern PKT_SendCharFuncPtr         PKT_SendChar;

/* ---- Function Prototypes ---------------------------------------------- */

void PKT_ProcessChar( unsigned char ch, void *userData );
void PKT_SendPacket( const PKT_Packet *pkt, void *userData );
void PKT_SendText( const char *str, void *userData );

#if defined( __cplusplus )
}
#endif

/** @} */

#endif // PKT_PACKET_H

