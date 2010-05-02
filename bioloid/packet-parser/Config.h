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
*   @file   Config.h
*
*   @brief  Global Configuration information.
*
****************************************************************************/

#if !defined( CONFIG_H )
#define CONFIG_H              /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

/* ---- Constants and Types ---------------------------------------------- */

/**
 *  Sets Logging parameters
 */

#define CFG_LOG_TO_BUFFER   0
#define CFG_BLD_MAX_PARAM   128
#define CFG_BLD_OPTIMIZE_SYNC_WRITE_PARSING 1

#endif // CONFIG_H

