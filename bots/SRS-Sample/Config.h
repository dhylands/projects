/****************************************************************************
*
*  Config.h
*
*  Contains configuration information.
*
****************************************************************************/

#if !defined( CONFIG_H )
#define CONFIG_H

#define CFG_CPU_CLOCK   16000000L

#define CFG_USE_UART0   1

#define CFG_UART0_RX_BUFFER_SIZE    32
#define CFG_UART0_TX_BUFFER_SIZE    32
#define CFG_UART0_LF_TO_CRLF        1

#define CFG_USE_ADC 1

#define CFG_TIMER0_INCLUDE  "Encoder.h"
#define CFG_TIMER0_MS_TICK  PollEncoder()

#endif  // CONFIG_H


