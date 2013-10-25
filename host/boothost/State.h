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
*   @file   State.h
*
*   @brief  Common State machine definitions.
*
****************************************************************************/

#if !defined( STATE_H )
#define STATE_H

// ---- Include Files -------------------------------------------------------

#include <stdlib.h>

#if !defined( READFILE_H )
#   include "ReadFile.h"
#endif

// ---- Constants and Types -------------------------------------------------

// Hmm. We want our states to be functions, and we want each state to return
// a pointer to the next state. C won't let us do that, but we can play
// games in C++.

class StatePtrProxy;
typedef StatePtrProxy (*StatePtr)( char ch );

class StatePtrProxy
{
    StatePtr    m_statePtr;
public:
    StatePtrProxy( StatePtr statePtr ) : m_statePtr( statePtr ) {}
    operator StatePtr() { return m_statePtr; }
};

// Information obtained from the device we're talking to.

struct DeviceInfo
{
    const char *name;
    unsigned    flashSize;
    unsigned    bootSize;
    unsigned    pageSize;
    unsigned    pageShift;
};

// ---- Variable Externs ----------------------------------------------------

/**
 *  gDeviceInfo contains information about the device determined by early
 *  conversations with the bootloader.
 */

extern  DeviceInfo      gDeviceInfo;

/**
 *  The name of the file that we're downloading.
 */

extern  const char     *gDownloadFileName;

/**
 *  Contains the data to be downloaded to the device.
 */

extern  FileInfo       *gDownloadInfo;

/**
 *  Pointer used to traverse through the list of FileSegments pointed to
 * by gDownloadInfo->head
 */

extern  FileSegment    *gDownloadSegment;

/**
 *  Used to maintain position with a FileSegment.
 */

extern  unsigned        gDownloadOffset;

/**
 *  Used to keep track of retries for sending pages
 */

extern  unsigned        gRetryCount;

// ---- Function Prototypes -------------------------------------------------

bool CheckFile( FileInfo *fileInfo, DeviceInfo *devInfo );
void LogUnrecognizedChar( char ch, const char *label );
void SendByte( unsigned char ch );
void SendData( const void *buf, size_t bytesToWrite );

// This is the main starting state in BootHost.cpp

StatePtrProxy   StateDownload( char ch );
StatePtrProxy   StateLog( char ch );

#endif  // STATE_H

