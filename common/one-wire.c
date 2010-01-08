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
*   @file   one-wire.c 
*
*   @brief  Implements an interface to the 1-wire protocol/bus.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <one-wire.h>

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

#define OW_WRITE_1_LOW_USECS            6   // A
#define OW_WRITE_1_RELEASE_USECS        64  // B

#define OW_WRITE_0_LOW_USECS            60  // C
#define OW_WRITE_0_RELEASE_USECS        10  // D

#define OW_READ_SETUP_USECS             9   // E
#define OW_READ_RELEASE_USECS           55  // F

#define OW_RESET_SETUP_USECS            0   // G
#define OW_RESET_LOW_USECS              480 // H
#define OW_RESET_DETECT_SETUP_USECS     70  // I
#define OW_RESET_DETECT_RELEASE_USECS   410 // J

/* ---- Private Variables ------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

/**
 * @addtogroup one-wire
 * @{
 */

//***************************************************************************
/**
*   Initializes the hardware to be used for the one-wire protocol.
*/

void OW_Init( void )
{
    OW_HAL_Init();
}

#if 0
//***************************************************************************
/**
*   Sets up the various protocol delays.
*
*   This is coded as a function, rather than a pointer to a structure
*   to reduce the amount of data space needed (on embedded devices
*   flash is generally much more available than RAM).
*/

void OW_SetSpeed( OW_Device_t *dev, OW_Speed_t speed )
{
    if ( speed == OW_SpeedOverDrive )
    {
        dev->write_1_low_ticks          = 1.5 * 4;  // A
        dev->write_1_release_ticks      = 7.5 * 4;  // B

        dev->write_0_low_ticks          = 7.5 * 4;  // C
        dev->write_0_release_ticks      = 2.5 * 4;  // D

        dev->read_setup_ticks           = 0.75 * 4; // E
        dev->read_release_ticks         = 7 * 4;    // F

        dev->reset_setup_ticks          = 2.5 * 4;  // G
        dev->reset_low_ticks            = 70 * 4;   // H
        dev->reset_detect_setup_ticks   = 8.5 * 4;  // I
        dev->reset_detect_release_ticks = 40 * 4;   // J
    }
    else
    {
        // Standard Speed

        dev->write_1_low_ticks          = 6 * 4;    // A
        dev->write_1_release_ticks      = 64 * 4;   // B

        dev->write_0_low_ticks          = 60 * 4;   // C
        dev->write_0_release_ticks      = 10 * 4;   // D

        dev->read_setup_ticks           = 9 * 4;    // E
        dev->read_release_ticks         = 55 * 4;   // F

        dev->reset_setup_ticks          = 0;        // G
        dev->reset_low_ticks            = 480 * 4;  // H
        dev->reset_detect_setup_ticks   = 70 * 4;   // I
        dev->reset_detect_release_ticks = 410 * 4;  // J
    }

    dev->speed = speed;

} // OW_SetSpeed

#endif

//***************************************************************************
/**
*   Resets the 1-wire bus, and returns 1 if no prescence detect
*   was found.
*/

int OW_Reset( void )
{
    int                     detect;
    OW_HAL_AtomicFlags_t    flags;

    flags = OW_HAL_AtomicStart();
    {
        OW_HAL_DelayUSecs( OW_RESET_SETUP_USECS );
        OW_HAL_DriveDQLow();
        OW_HAL_DelayUSecs( OW_RESET_LOW_USECS );
        OW_HAL_ReleaseDQ();
        OW_HAL_DelayUSecs( OW_RESET_DETECT_SETUP_USECS );
        detect = OW_HAL_ReadDQ();
        OW_HAL_DelayUSecs( OW_RESET_DETECT_RELEASE_USECS );
    }
    OW_HAL_AtomicEnd( flags );

    return detect;  // 1 means no devices detected
    
} // OW_Reset

//***************************************************************************
/**
*   Writes a single bit onto the 1-wire bus.
*/

void OW_WriteBit
(
    uint8_t bit     ///< Bit value to write onto the 1-wire bus (0 or 1)
)
{
    OW_HAL_AtomicFlags_t    flags;

    flags = OW_HAL_AtomicStart();
    {
        if ( bit )
        {
            // Write out a '1' bit
    
            OW_HAL_DriveDQLow();
            OW_HAL_DelayUSecs( OW_WRITE_1_LOW_USECS );
            OW_HAL_ReleaseDQ();
            OW_HAL_DelayUSecs( OW_WRITE_1_RELEASE_USECS );
        }
        else
        {
            // Write out a '0' bit
    
            OW_HAL_DriveDQLow();
            OW_HAL_DelayUSecs( OW_WRITE_0_LOW_USECS );
            OW_HAL_ReleaseDQ();
            OW_HAL_DelayUSecs( OW_WRITE_0_RELEASE_USECS );
        }
    }
    OW_HAL_AtomicEnd( flags );

} // OW_WriteBit

//***************************************************************************
/**
*   Writes a byte onto the 1-wire bus.
*/

void OW_WriteByte
(
    uint8_t byte       ///< Byte value to write onto the 1-wire bus
)
{
    uint8_t i;

    for ( i = 0; i < 8; i++ )
    {
        // Write the data out 1 bit at a time, least significant bit first

        OW_WriteBit( byte & 1 );

        byte >>= 1;
    }
} // OW_WriteByte

//***************************************************************************
/**
*   Reads a single bit from the 1-wire bus.
*/

uint8_t OW_ReadBit( void )
{
    uint8_t bit;

    OW_HAL_AtomicFlags_t    flags;

    flags = OW_HAL_AtomicStart();
    {
        OW_HAL_DriveDQLow();
        OW_HAL_DelayUSecs( OW_WRITE_1_LOW_USECS );
        OW_HAL_ReleaseDQ();
        OW_HAL_DelayUSecs( OW_READ_SETUP_USECS );
        bit = OW_HAL_ReadDQ();
        OW_HAL_DelayUSecs( OW_READ_RELEASE_USECS );
    }
    OW_HAL_AtomicEnd( flags );

    return bit;

} // OW_ReadBit

//***************************************************************************
/**
*   Reads a byte from the 1-wire bus.
*/

uint8_t OW_ReadByte( void )
{
    uint8_t i;
    uint8_t result = 0;

    for ( i = 0; i < 8; i++ )
    {
        result >>= 1;

        // Write the data out 1 bit at a time, least significant bit first

        if ( OW_ReadBit() )
        {
            result |= 0x80;
        }
    }

    return result;

} // OW_ReadByte

//***************************************************************************
/**
*   Writes a byte to the 1-wire bus and returns the sampled result.
*/

uint8_t OW_TouchByte
(
    uint8_t byte   ///< Byte to write
)
{
    uint8_t i;
    uint8_t result = 0;

    for ( i = 0; i < 8; i++ )
    {
        result >>= 1;

        if ( byte & 1 )
        {
            // Reading a bit is just like writing a 1 bit except that
            // it also does a sample

            if ( OW_ReadBit() )
            {
                result |= 0x80;
            }
        }
        else
        {
            OW_WriteBit( 0 );
        }

        byte >>= 1;
    }

    return result;


} // OW_TouchByte

//***************************************************************************
/**
*   Writes a block and returns the sampled result in the same buffer.
*/

void OW_TouchBlock
(
    void   *voidData,   ///< Pointer to block to read/write
    size_t  numBytes    ///< Number of bytes to write/read
)
{
    uint8_t *data = voidData;

    while ( numBytes > 0 )
    {
        *data = OW_TouchByte( *data );
        data++;
        numBytes--;
    }

} // OW_TouchBlock

/** @} */

