/****************************************************************************
*
*  Config.h
*
*  Contains configuration information.
*
****************************************************************************/

#if !defined( CONFIG_H )
#define CONFIG_H

#define CFG_CPU_CLOCK   16000000

#define CFG_LOG_USE_STDIO           0
#define CFG_LOG_ALLOW_DEFERRED_NL   1

#define CFG_ADC_8_BIT               0
#define CFG_ADC_POLL                0
#define CFG_ADC_AVERAGE             1

#define CFG_USE_UART    1

#define CFG_UART_BAUD_RATE         38400
#define CFG_UART_RX_BUFFER_SIZE    8
#define CFG_UART_TX_BUFFER_SIZE    64
#define CFG_UART_LF_TO_CRLF        1

#endif  // CONFIG_H


