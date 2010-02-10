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
****************************************************************************
*
*   This driver allows multiple GPIO pins to be monitored and allows a user
*   mode program to be notified when the pin changes.
*
****************************************************************************/

#if !defined( GPIO_EVENT_DRV_H )
#define GPIO_EVENT_DRV_H

/* ---- Include Files ----------------------------------------------------- */

#if defined( __KERNEL__ )
#   include <linux/types.h>
#   include <linux/time.h>
#   include <linux/ioctl.h>
#else
#   include <stdint.h>
#   include <sys/time.h>
#   include <sys/ioctl.h>
#endif


/* ---- Constants and Types ----------------------------------------------- */

// The ioctl "magic" is just some character value which is used to help
// detect when incorrect ioctl values are sent down to a driver.

#define GPIO_EVENT_IOCTL_MAGIC  'G'

/**
 *  Deefines for each of the ioctl commands. Note that since we want to reduce
 *  the possibility that a user mode program gets out of sync with a given 
 *  driver, we explicitly assign a value to each enumeration. This makes
 *  it more difficult to stick new ioctl's in the middle of the list.
 */

typedef enum
{
    GPIO_EVENT_CMD_FIRST                = 0x80,

    GPIO_EVENT_CMD_MONITOR_GPIO         = 0x80,
    GPIO_EVENT_CMD_SET_READ_MODE        = 0x81,

    /* Insert new ioctls here                                               */

    GPIO_EVENT_CMD_LAST,

} GPIO_EVENT_CMD;

typedef enum
{
    GPIO_EventRisingEdge     = 0x01,
    GPIO_EventFallingEdge    = 0x02,
    GPIO_EventBothEdges      = GPIO_EventRisingEdge | GPIO_EventFallingEdge,

} GPIO_EventEdgeType_t;

typedef struct
{
    uint8_t                 gpio;               // gpio to monitor
    uint8_t                 onOff;              // 0 = stop monitoring, 1 = start monitoring
    GPIO_EventEdgeType_t    edgeType;           // Monitor rising/falling/both edges?
    uint8_t                 debounceMilliSec;   // debounce time in milliseconds

} GPIO_EventMonitor_t;

typedef enum
{
    GPIO_EventReadModeAscii     = 0x00,     // Reads return ASCII data (default)
    GPIO_EventReadModeBinary    = 0x01,     // Reads return Binary data

} GPIO_EventReadMode_t;

/*
 * Definitions for the actual ioctl commands
 */

#define GPIO_EVENT_IOCTL_MONITOR_GPIO   _IOW( GPIO_EVENT_IOCTL_MAGIC, GPIO_EVENT_CMD_MONITOR_GPIO,  GPIO_EventMonitor_t ) // arg is GPIO_EventMonitor *
#define GPIO_EVENT_IOCTL_SET_READ_MODE  _IO(  GPIO_EVENT_IOCTL_MAGIC, GPIO_EVENT_CMD_SET_READ_MODE )                      // arg is int

/*
 * Definitions for sysctl. The top level define has to be unique system wide.
 * The kernel defines values 1 thru about 10 (see include/linunx/sysctl.h)
 */

#define CTL_GPIO_EVENT          0x47504576    // 'GPEv' in hex form

/*
 * Reads return GPIO_Event_t structures
 */

typedef struct
{
    uint8_t                 gpio;       // GPIO that this event is for
    GPIO_EventEdgeType_t    edgeType;   // Type of edge detected     
    struct timeval          time;       // Time the event occurred

} GPIO_Event_t;

#endif  // GPIO_EVENT_DRV_H

