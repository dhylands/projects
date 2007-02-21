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

#define CFG_TIMER_UART_TX_BUFFER_SIZE   64
#define CFG_TIMER_UART_BAUD_RATE    38400
#define CFG_TIMER_UART_LF_TO_CRLF   1
#define CFG_TIMER_UART_DDR          DDRD
#define CFG_TIMER_UART_PORT         PORTD
#define CFG_TIMER_UART_MASK         ( 1 << 2 )

#define CFG_LOG_TO_BUFFER           1
#define CFG_LOG_NUM_BUFFER_ENTRIES  32
#define CFG_LOG_USE_STDIO           0
#define CFG_LOG_PUT_CHAR_FUNC       TimerUART_PutChar
#define CFG_LOG_PUT_CHAR_HDR        "Timer-UART.h"

#define CFG_USE_UART0               1

#define CFG_UART0_BAUD_RATE         38400

#define CFG_UART0_RX_BUFFER_SIZE    128
#define CFG_UART0_TX_BUFFER_SIZE    128
#define CFG_UART0_LF_TO_CRLF        1

#endif  // CONFIG_H


