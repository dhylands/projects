/****************************************************************************
*
*   Copyright (c) 2005 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
/**
*
*   @file   I2C_BootLoader.h
*
*   @brief  This file describes the interface to the I2C_BootLoader class.
*
****************************************************************************/

#if !defined( I2C_BOOTLOADER_H )
#define I2C_BOOTLOADER_H                   ///< Include Guard

// ---- Include Files -------------------------------------------------------

#include "i2c.h"
#include "I2C_Adapter.h"

/**
 * @addtogroup i2c
 * @{
 */

//---------------------------------------------------------------------------
/**
*   The I2C_BootLoader class talks to a device using the I2C BootLoader
*   protocol.
*/

class I2C_BootLoader
{
public:

    //------------------------------------------------------------------------
    //  Default constructor

    I2C_BootLoader( I2C_Adapter *i2cAdapter, I2C_Addr_t slaveAddr );

    //------------------------------------------------------------------------
    //  Destructor

    ~I2C_BootLoader();

    //------------------------------------------------------------------------
    // Retrieves information about the device we're talking to.

    bool GetBootLoaderInfo( BootLoaderInfo_t *bootInfo );

    //------------------------------------------------------------------------
    // Downloads a file to the BootLoader

    bool DownloadFile( FileInfo *fileInfo );

    //------------------------------------------------------------------------
    // Prints information about the bootloader.

    void PrintBootLoaderInfo( const BootLoaderInfo_t &bootInfo );

    //------------------------------------------------------------------------
    // Reads data from memory inferred by @a cmd.

    bool ReadMemory( uint8_t cmd, uint32_t addr, I2C_Adapter::Buffer *readBuffer );

    //------------------------------------------------------------------------
    // Reads data from EEPROM memory

    bool ReadEeprom( uint32_t addr, I2C_Adapter::Buffer *readBuffer )
    {
        return ReadMemory( BL_CMD_READ_EEPROM_DATA, addr, readBuffer );
    }

    //------------------------------------------------------------------------
    // Reads data from flash memory

    bool ReadFlash( uint32_t addr, I2C_Adapter::Buffer *readBuffer )
    {
        return ReadMemory( BL_CMD_READ_FLASH_DATA, addr, readBuffer );
    }

    //------------------------------------------------------------------------
    // Runs the application that's currently downloaded.

    bool RunApp();

    //------------------------------------------------------------------------
    // Writes data to memory inferred by @a cmd.

    bool WriteMemory( uint8_t cmd, uint32_t addr, I2C_Adapter::Buffer *writeBuffer );

    //------------------------------------------------------------------------
    // Writes data to EEPROM memory

    bool WriteEeprom( uint32_t addr, I2C_Adapter::Buffer *writeBuffer )
    {
        return WriteMemory( BL_CMD_WRITE_EEPROM_DATA, addr, writeBuffer );
    }

    //------------------------------------------------------------------------
    // Writes data to flash memory

    bool WriteFlash( uint32_t addr, I2C_Adapter::Buffer *writeBuffer )
    {
        return WriteMemory( BL_CMD_WRITE_FLASH_DATA, addr, writeBuffer );
    }

private:

    //------------------------------------------------------------------------
    // The copy constructor and assignment operator are not need for this
    // class so we declare them private and don't provide an implementation.

    I2C_BootLoader( const I2C_BootLoader & copy );
    I2C_BootLoader &operator =( const I2C_BootLoader &rhs );

    //------------------------------------------------------------------------

    I2C_Adapter        *m_adapter;      ///< I2C Adapter used to send the commands with
    I2C_Addr_t          m_slaveAddr;    ///< I2C Address of device
    BootLoaderInfo_t    m_bootInfo;     ///< BootLoader Info about our slave device
};

// ---- Inline Functions ----------------------------------------------------

/** @} */

#endif // I2C_BOOTLOADER_H

