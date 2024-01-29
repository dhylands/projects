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
/**
 *
 *   @file   Log.h
 *
 *   @brief  Contains some logging macros.
 *
 ****************************************************************************/
/**
 *   @defgroup   Log   Logging
 *
 *   @brief      Provides a common interface for logging.
 *
 ****************************************************************************/

#pragma once

// ---- Include Files -------------------------------------------------------

#include <stdarg.h>

#if !defined(CONFIG_H)
#include "Config.h"
#endif
#if !defined(CFG_LOG_USE_STDIO) && !defined(CFG_LOG_USE_USB_DEBUG)
//! If CFG_LOG_USE_STIOD is non-zero then printf will be used for logging
#define CFG_LOG_USE_STDIO 1
#endif
#if !defined(CFG_LOG_USE_STDIO)
#define CFG_LOG_USE_STDIO 0
#endif
#if !defined(CFG_LOG_USE_USB_DEBUG)
//! If CFG_LOG_USE_USB_DEBUG is non-zero then a USB CDC port will be used for logging
#define CFG_LOG_USE_USB_DEBUG 0
#endif

#if defined(AVR)
#include <avr/pgmspace-fix.h>
#include <avr/interrupt.h>

/* It turns out that newer versions of gcc have troubles with the
 * default PSTR macro when compiled using C++. This change
 * provides the equivalent behaviour, but gets rid of the
 * warning: only initialized variables can be placed into program memory area
 */

#define LOG_PROGMEM PROGMEM
#define LOG_PSTR(s) PSTR(s)

// #define LOG_PROGMEM __attribute__(( section(".progmem.data") ))
// #define LOG_PSTR(s) (__extension__({static char __c[] LOG_PROGMEM = (s); &__c[0];}))

#endif

#if CFG_LOG_TO_BUFFER
#if !defined(CBUF_H)
#include "CBUF.h"
#endif
#endif

#if (!CFG_LOG_USE_STDIO && !CFG_LOG_USE_USB_DEBUG)

#if !defined(CFG_LOG_PUT_CHAR_FUNC)
#define CFG_LOG_PUT_CHAR_FUNC UART0_PutChar
#define CFG_LOG_PUT_CHAR_HDR "UART.h"
#endif

#include CFG_LOG_PUT_CHAR_HDR
#endif

/**
 * @addtogroup Log
 * @{
 */

#if !defined(CFG_LOG_ENABLED)
//! Setting CFG_LOG_ENABLED to 0 turns off all logging.
#define CFG_LOG_ENABLED 1
#endif

#if !CFG_LOG_ENABLED

#define Log(fmt, args...)
#define LogError(fmt, args...)
#define LogAssertFailed(exprStr, file, lineNum, function)

#define ASSERT(expr) ((void)0)

#else

//! Assert macro which logs failures using the Log functions.
#define ASSERT(expr) \
    ((expr) ? ((void)0) : LogAssertFailed(#expr, __FILE__, __LINE__, __FUNCTION__), ((void)0))

#if defined(__cplusplus)
extern "C" {
#endif

#if CFG_LOG_ALLOW_DEFERRED_NL
extern int gDeferredNewline;
#endif

/***************************************************************************
 *
 *   Log Buffer support
 */

#if CFG_LOG_TO_BUFFER

#if defined(AVR)

typedef struct {
    const char* fmt;
    uint8_t param1;
    uint8_t param2;
#if CFG_LOG_EXTRA_PARAMS
    uint8_t param3;
    uint8_t param4;
#endif
} LOG_Entry_t;

#if CFG_LOG_EXTRA_PARAMS
#define LOG_EXTRA_PARAMS_DECL , uint8_t arg3, uint8_t arg4
#define LOG_EXTRA_PARAMS , 0, 0
#else
#define LOG_EXTRA_PARAMS_DECL
#define LOG_EXTRA_PARAMS
#endif

void LogBuf_P(const char* fmt, uint8_t arg1, uint8_t arg2 LOG_EXTRA_PARAMS_DECL);

#define LogBuf0(fmt) LogBuf_P(LOG_PSTR(fmt), 0, 0 LOG_EXTRA_PARAMS)
#define LogBuf1(fmt, arg1) LogBuf_P(LOG_PSTR(fmt), arg1, 0 LOG_EXTRA_PARAMS)
#define LogBuf2(fmt, arg1, arg2) LogBuf_P(LOG_PSTR(fmt), arg1, arg2 LOG_EXTRA_PARAMS)

#if CFG_LOG_EXTRA_PARAMS
#define LogBuf3(fmt, arg1, arg2, arg3) LogBuf_P(LOG_PSTR(fmt), arg1, arg2, arg3, 0)
#define LogBuf4(fmt, arg1, arg2, arg3, arg4) LogBuf_P(LOG_PSTR(fmt), arg1, arg2, arg3, arg4)
#endif

#else

typedef struct {
    const char* fmt;
    uint8_t param1;
    uint8_t param2;
} LOG_Entry_t;

void LogBuf(const char* fmt, uint8_t arg1, uint8_t arg2);

#define LogBuf0(fmt, arg1) LogBuf(fmt, 0, 0)
#define LogBuf1(fmt, arg1) LogBuf(fmt, arg1, 0)
#define LogBuf2(fmt, arg1, arg2) LogBuf(fmt, arg1, arg2)

#endif  // AVR

#if (CFG_LOG_NUM_BUFFER_ENTRIES > 128)
typedef uint16_t LOG_BufferIdx_t;
#else
typedef uint8_t LOG_BufferIdx_t;
#endif

typedef struct {
    LOG_BufferIdx_t m_getIdx;
    LOG_BufferIdx_t m_putIdx;
    LOG_Entry_t m_entry[CFG_LOG_NUM_BUFFER_ENTRIES];
} LOG_Buffer_t;

extern volatile LOG_Buffer_t LOG_gBuffer;

#define LOG_gBuffer_SIZE (sizeof(LOG_gBuffer.m_entry) / sizeof(LOG_gBuffer.m_entry[0]))

void LogBufDump(void);

#endif  // CFG_LOG_TO_BUFFER

/***************************************************************************
 *
 *   Regular logging support
 */

#if CFG_LOG_USE_STDIO
#include <stdio.h>

extern FILE* gLogFs;

void LogInit(FILE* logFs);
#endif

#if defined(AVR)

void Log_P(const char* fmt, ...);
void LogError_P(const char* fmt, ...);
void LogAssertFailed_P(
    const char* exprStr,
    const char* file,
    unsigned lineNum,
    const char* function);
void vLog_P(const char* fmt, va_list args);

#define Log(fmt, args...) Log_P(LOG_PSTR(fmt), ##args)
#define LogError(fmt, args...) LogError_P(LOG_PSTR(fmt), ##args)
#define LogAssertFailed(exprStr, file, lineNum, function) \
    LogAssertFailed_P(LOG_PSTR(expr), LOG_PSTR(file), lineNum, LOG_PSTR(function))
#define vLog(fmt, va_list, args) vLog_P(LOG_PSTR(fmt), args)

#else  // AVR

//! Logging level for normal log messages.
#define LOG_LEVEL_NORMAL 0

//! Logging level for logging errors.
#define LOG_LEVEL_ERROR 1

//! Logging level for logging fatal errors (i.e. ASSERTs)
#define LOG_LEVEL_ASSERT 2

//! typedef for overriding the logging function.
typedef void (*LogFunc_t)(int logLevel, const char* fmt, va_list args);

extern int gQuiet;

void Log(const char* fmt, ...);
void LogError(const char* fmt, ...);
void LogAssertFailed(const char* expr, const char* file, unsigned lineNum, const char* function);
void vLog(const char* fmt, va_list args);
void vLogError(const char* fmt, va_list args);

void LogFunc(int logLevel, const char* fmt, ...);
void vLogFunc(int logLevel, const char* fmt, va_list args);

//! Compatability macro for AVR Log_P function
#define Log_P(fmt, args...) Log(fmt, ##args)

//! Compatability macro for AVR LogError_P function
#define LogError_P(fmt, args...) LogError(fmt, ##args)

//! Compatability macro for AVR LogAssertFailed_P function
#define LogAssertFailed_P(expr, file, lineNum, function) LogAssertFailed(expr, file, line, function)

//! Compatability macro for AVR vLog_P function
#define vLog_P(fmt, args) vLog(fmt, args)

void SetLogFunc(LogFunc_t logFunc);
void DefaultLogFunc(int logLevel, const char* fmt, va_list args);

#endif  // AVR

extern int gVerbose;
extern int gDebug;

//! Macro for logging a debug log.
#define LogDebug(fmt, args...) \
    do {                       \
        if (gDebug) {          \
            Log(fmt, ##args);  \
        }                      \
    } while (0)

//! Macro for logging a verbose log.
#define LogVerbose(fmt, args...) \
    do {                         \
        if (gVerbose) {          \
            Log(fmt, ##args);    \
        }                        \
    } while (0)

#if defined(__cplusplus)
}
#endif

#endif  // CFG_LOG_ENABLED

/** @} */
