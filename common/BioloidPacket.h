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
*   @file   BioloidBus.h
*
*   @brief  Contains the definitions for a bioloid bus. Essentially
*           there is a bus for each UART with bioloid devices attached.
*
****************************************************************************/

#if !defined( BIOLOIDPACKET_H )
#define BIOLOIDPACKET_H       /**< Include Guard                             */

// ---- Include Files -------------------------------------------------------

#include <stdint.h>

#include "Config.h"
#include "Bioloid.h"

/**
 * @addtogroup bioloid
 * @{
 */

#if defined( CFG_BIOLOID_MAX_PARAM )
#   define BLD_MAX_PARAM    CFG_BIOLOID_MAX_PARAM
#else
#   define BLD_MAX_PARAM    8
#endif

class BioloidPacket
{
public:

    //------------------------------------------------------------------------
    // Default constructor

    BioloidPacket();

    //------------------------------------------------------------------------
    // Destructor

    virtual ~BioloidPacket();

    //------------------------------------------------------------------------
    // Accessor functions which allow portions of the packet to be retrieved.

    Bioloid::ID_t ID()      { return m_id; }
    uint8_t       Length()  { return m_length; }
    uint8_t       Command() { return m_cmd; }
    uint8_t       Param( uint8_t idx ) { return idx < BLD_MAX_PARAM ? m_param[ idx ] : 0; }
    uint8_t       CheckSum(){ return m_checksum; }

    //------------------------------------------------------------------------
    // Runs a single character through the state machine. Once a packet
    // has been parsed successfully, the PacketReceived virtual method
    // is called.

    virtual void ProcessChar( uint8_t ch );

    //------------------------------------------------------------------------
    // Called when a packet has been parsed. If the packet was well formed
    // then err will be equal to Bioloid::ERROR_NONE, otherwise err will 
    // be Bioloid::ERROR_CHECKSUM.

    virtual void PacketReceived( Bioloid::Error err ) = 0;

private:

    enum State
    {
        STATE_IDLE,         ///< We're waiting for the beginning of the packet
        STATE_1ST_FF_RCVD,  ///< We've received the 1st 0xFF
        STATE_2ND_FF_RCVD,  ///< We've received the 2nd 0xFF
        STATE_ID_RCVD,      ///< We've received the ID
        STATE_LENGTH_RCVD,  ///< We've received the length
        STATE_COMMAND_RCVD, ///< We've received the command
    };

    State           m_state;
    uint8_t         m_paramIdx;

    Bioloid::ID_t   m_id;
    uint8_t         m_length;
    uint8_t         m_cmd;
    uint8_t         m_param[ BLD_MAX_PARAM ];
    uint8_t         m_checksum;
};

/** @} */

#endif /* BIOLOIDPACKET_H */

