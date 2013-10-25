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

#define CFG_USE_UART0               1

#define CFG_UART0_BAUD_RATE         38400

#define CFG_UART0_RX_BUFFER_SIZE    128
#define CFG_UART0_TX_BUFFER_SIZE    128
#define CFG_UART0_LF_TO_CRLF        1

// D7 is the green LED on the bottom of the 3pi

#define LED_DDR                 DDRD
#define LED_PORT                PORTD
#define LED_MASK                ( 1 << 7 )   

#define CFG_LCD_SCATTERED_DATA  1

#define CFG_LCD_DATA_DB4_PORT   PORTB
#define CFG_LCD_DATA_DB4_PIN    1

#define CFG_LCD_DATA_DB5_PORT   PORTB
#define CFG_LCD_DATA_DB5_PIN    4

#define CFG_LCD_DATA_DB6_PORT   PORTB
#define CFG_LCD_DATA_DB6_PIN    5

#define CFG_LCD_DATA_DB7_PORT   PORTD
#define CFG_LCD_DATA_DB7_PIN    7

#define CFG_LCD_E_PORT      PORTD
#define CFG_LCD_E_PIN       4

#define CFG_LCD_RS_PORT     PORTD
#define CFG_LCD_RS_PIN      2
                
#define CFG_LCD_RW_PORT     PORTB
#define CFG_LCD_RW_PIN      0

#endif  // CONFIG_H


