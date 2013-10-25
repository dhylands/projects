/****************************************************************************
*
*   Copyright (c) 2005 - 2008 Dave Hylands     <dhylands@gmail.com>
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
*   @file   StateAVReflash.cpp
*
*   @brief  Implements the state machine for dealing with the AVReflash
*           bootloader.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <sys/unistd.h>

#include "StateAVReflash.h"
#include "Log.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#define MAX_PAGE_SIZE   1024

// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------

static  void SendExecCode();
static  void SendPage( unsigned address, unsigned char *data, unsigned numBytes );

// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*   Sends the "ExecCode" which tells the target to execute the code.
*/

void SendExecCode()
{
    Log( "Executing...\n" );

    SendByte( 0xFF );
    SendByte( 0xFF );
    SendByte( 0xFF );
    SendByte( 0xFF );

} // SendExecCode

/***************************************************************************/
/**
*   Sends a page of data down to the bootloader
*/

void SendPage( unsigned address, unsigned char *data, unsigned numBytes )
{
    Log( "Writing 0x%05x ... ", address );

    address >>= gDeviceInfo.pageShift;

    SendByte(( address & 0xFF00 ) >> 8 );
    SendByte( address & 0x00FF );

    SendData( data, numBytes );

    unsigned char checkSum = 0;
    for ( unsigned i = 0; i < numBytes; i++ ) 
    {
        checkSum += data[ i ];
    }

    SendByte( checkSum );

} // SendPage

/***************************************************************************/
/**
*   We're waiting for a page to be programmed.
*/

StatePtrProxy StateAVReflashAckNack( char ch )
{
    switch ( ch )
    {
        case '!':
        {
            Log( "ok\n" );

            gDownloadOffset += gDeviceInfo.pageSize;
            if ( gDownloadOffset >= gDownloadSegment->lenThisSegment )
            {
                gDownloadSegment = gDownloadSegment->next;
                gDownloadOffset = 0;
            }

            if ( gDownloadSegment == NULL )
            {
                // We're done.

                SendExecCode();

                return NULL;
            }
            break;
        }

        case '$':
        {
            LogError( "*** Error *** Verify failed - Retrying\n" );
            gRetryCount++;
            break;
        }

        case '@':
        {
            LogError( "*** Error *** CheckSum failed - Retrying\n" );
            gRetryCount++;
            break;
        }

        default:
        {
            LogError( "Unexpected character received: 0x%02x while waiting for Ack/Nak\n", (unsigned)ch );
            return NULL;
        }
    }

    if ( gRetryCount > 3 )
    {
        return NULL;
    }

    SendPage( gDownloadSegment->address + gDownloadOffset, 
              &gDownloadSegment->data[ gDownloadOffset ], 
              gDeviceInfo.pageSize );

    return StateAVReflashAckNack;

} // StateAVReflashAckNack

/***************************************************************************/
/**
*   We're waiting for the page size to be transmimtted.
*/

StatePtrProxy StateAVReflashPageSize( char ch )
{
    if (( ch >= '0' ) && ( ch <= '9' ))
    {
        gDeviceInfo.pageSize *= 10;
        gDeviceInfo.pageSize += ( ch - '0' );

        if ( gDeviceInfo.pageSize > MAX_PAGE_SIZE )
        {
            LogError( "Invalid Page Size: %d\n", gDeviceInfo.pageSize );
            return NULL;
        }

        return StateAVReflashPageSize;
    }

    if ( ch == '!' )
    {
        Log( "Page Size:  %3d\n", gDeviceInfo.pageSize );

        // Currently, we don't know the device size or the bootloader size
        // so we can't call CheckFile.

        if (( gDownloadFileName == NULL ) 
//        ||  !CheckFile( gDownloadInfo, &gDeviceInfo )
        ||  gDownloadInfo->head == NULL )
        {
            // No download - just send the ExecCode

            SendExecCode();

            return NULL;
        }

        gDownloadSegment = gDownloadInfo->head;
        gDownloadOffset = 0;
        gRetryCount = 0;

        SendPage( gDownloadSegment->address + gDownloadOffset, 
                  &gDownloadSegment->data[ gDownloadOffset ], 
                  gDeviceInfo.pageSize );

        return StateAVReflashAckNack;
    }

    LogError( "Unexpected character received: 0x%02x while waiting for Ack/Nak\n", (unsigned)ch );
    return NULL;

} // StateAVReflashPageSize

