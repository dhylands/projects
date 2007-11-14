/****************************************************************************
*
*   Copyright (c) 2007 Jon Hylands     <jon@huv.com>
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

#define CFG_USE_UART0   1
#define CFG_USE_UART1   0

#define CFG_UART0_BAUD_RATE         19200
#define CFG_UART0_RX_BUFFER_SIZE    128
#define CFG_UART0_TX_BUFFER_SIZE    128
#define CFG_UART0_LF_TO_CRLF        1

////////////////////////////////////////////////////////////////////////////////

#define CFG_LCD_DATA_PORT   PORTD
#define CFG_LCD_DATA_SHIFT  2

#define CFG_LCD_E_PORT      PORTD
#define CFG_LCD_E_PIN       6

#define CFG_LCD_RS_PORT     PORTB
#define CFG_LCD_RS_PIN      0
                
#define CFG_LCD_RW_PORT     PORTD
#define CFG_LCD_RW_PIN      7

////////////////////////////////////////////////////////////////////////////////

#define CFG_ADC_8_BIT               0
#define CFG_ADC_POLL                0
#define CFG_ADC_AVERAGE             1


#endif  // CONFIG_H
