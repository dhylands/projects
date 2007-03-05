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

#if !defined( CONFIG_H )
#define CONFIG_H

#define CFG_USE_UART0                   1
#define CFG_UART0_BAUD_RATE             115200
#define CFG_UART0_RX_BUFFER_SIZE        64
#define CFG_UART0_TX_BUFFER_SIZE        64

#define CFG_BLD_USE_UART1               1
#define CFG_BLD_UART1_BAUD_RATE         1000000
#define CFG_BLD_UART1_RX_BUFFER_SIZE    64
#define CFG_BLD_UART1_TX_BUFFER_SIZE    64
#define CFG_BLD_UART1_RCV_TX            0
#define CFG_BLD_UART1_ENABLE_TX_PULLUP  1

#endif  // CONFIG_H


