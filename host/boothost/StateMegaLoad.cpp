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
*   @file   StateMegaLoad.cpp
*
*   @brief  Implements the state machine for dealing with the MegaLoad
*           bootloader.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <sys/unistd.h>

#include "StateMegaLoad.h"
#include "Log.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

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

StatePtrProxy StateMegaAckNack( char ch )
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

        case '@':
        {
            LogError( "*** Error *** - Retrying\n" );
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

    return StateMegaAckNack;

} // StateMegaAckNack

/***************************************************************************/
/**
*   We're expecting the boot size to be sent.
*/

StatePtrProxy StateMegaBootSize( char ch )
{
    unsigned    bootSize; // in bytes

    switch ( ch )
    {
        case 'a':   bootSize = 256;     break;
        case 'b':   bootSize = 512;     break;
        case 'c':   bootSize = 1024;    break;
        case 'd':   bootSize = 2048;    break;
        case 'e':   bootSize = 4096;    break;
        case 'f':   bootSize = 8192;    break;

        default:
        {
            LogUnrecognizedChar( ch, "FlashSize" );
            return NULL;
        }
    }

    Log( "Boot Size:  %3dk\n", bootSize / 1024 );

    gDeviceInfo.bootSize = bootSize;

    return StateMegaPageSize;

} // StateMegaBootSize

/***************************************************************************/
/**
*   We're expecting the device id to be sent.
*/

StatePtrProxy StateMegaDeviceId( char ch )
{
    const char  *deviceStr;

    switch ( ch )
    {
        case 'A':   deviceStr = "ATMega8";      break;
        case 'B':   deviceStr = "ATMega16";     break;
        case 'C':   deviceStr = "ATMega64";     break;
        case 'D':   deviceStr = "ATMega128";    break;
        case 'E':   deviceStr = "ATMega32";     break;

        case '>':
        {
            SendByte( '<' );
            return StateMegaDeviceId; 
            break;
        }

        default:
        {
            LogUnrecognizedChar( ch, "DeviceId" );
            return NULL;
        }
    }

    Log( "\nDeviceID: %s\n", deviceStr );

    gDeviceInfo.name = deviceStr;

    return StateMegaFlashSize;

} // StateMegaDeviceId

/***************************************************************************/
/**
*   We're at the very beginning, after receiving the device 
*   information, 
*/

StatePtrProxy StateMegaDeviceInfoAck( char ch )
{
    // Verify that we have all of the boot information

    if (( gDeviceInfo.flashSize == 0 )
    ||  ( gDeviceInfo.bootSize == 0 )
    ||  ( gDeviceInfo.pageSize == 0 ))
    {
        LogError( "Missing some device information\n" );
        return NULL;
    }

    if ( ch == '!' )
    {
        usleep( 1000 );

        if (( gDownloadFileName == NULL ) 
        ||  !CheckFile( gDownloadInfo, &gDeviceInfo )
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

        return StateMegaAckNack;
    }

    LogUnrecognizedChar( ch, "Ack" );

    return NULL;

} // StateMegaDeviceInfoAck

/***************************************************************************/
/**
*   We're expecting the flash size to be sent.
*/

StatePtrProxy StateMegaFlashSize( char ch )
{
    unsigned    flashSize;

    switch ( ch )
    {
        case 'l':   flashSize = 8192;   break;
        case 'm':   flashSize = 16384;  break;
        case 'n':   flashSize = 32768;  break;
        case 'o':   flashSize = 65536;  break;
        case 'p':   flashSize = 131072; break;

        default:
        {
            LogUnrecognizedChar( ch, "FlashSize" );
            return NULL;
        }
    }

    Log( "Flash Size: %3dk\n", flashSize / 1024 );

    gDeviceInfo.flashSize = flashSize;

    return StateMegaBootSize;

} // StateMegaFlashSize

/***************************************************************************/
/**
*   We're expecting the flash page size to be sent
*/

StatePtrProxy StateMegaPageSize( char ch )
{
    unsigned    pageShift;

    switch ( ch )
    {
        case 'Q':   pageShift = 5;  break;
        case 'R':   pageShift = 6;  break;
        case 'S':   pageShift = 7;  break;
        case 'T':   pageShift = 8;  break;
        case 'U':   pageShift = 9;  break;

        default:
        {
            LogUnrecognizedChar( ch, "PageSize" );
            return NULL;
        }
    }

    gDeviceInfo.pageShift = pageShift;
    gDeviceInfo.pageSize = 1 << pageShift;

    Log( "Page Size:  %3d\n", gDeviceInfo.pageSize );

    return StateMegaDeviceInfoAck;

} // StateMegaPageSize

