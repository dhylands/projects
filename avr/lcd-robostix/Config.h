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

/*
#define CFG_USE_UART0   0
#define CFG_USE_UART1   0

#define CFG_UART0_BAUD_RATE         38400
#define CFG_UART0_RX_BUFFER_SIZE    128
#define CFG_UART0_TX_BUFFER_SIZE    128
#define CFG_UART0_LF_TO_CRLF        1
*/
#define CFG_LCD_DATA_PORT   PORTD
#define CFG_LCD_DATA_SHIFT  0

#define CFG_LCD_E_PORT      PORTD
#define CFG_LCD_E_PIN       4

#define CFG_LCD_RS_PORT     PORTD
#define CFG_LCD_RS_PIN      6
                
#define CFG_LCD_RW_PORT     PORTD
#define CFG_LCD_RW_PIN      5

#endif  // CONFIG_H


