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
 *   @file   Log.c
 *
 *   @brief  This file contains the implementation of the logging functions.
 *
 ****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Log.h"

#if CFG_LOG_USE_STDIO
#include <stdio.h>
#else
#include "Str.h"
#endif

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

#if defined(AVR)

#if CFG_LOG_USE_USB_DEBUG
#include "avr/usb_debug_only.h"
#endif

#undef Log
#undef LogError
#undef LogAssertFailed
#undef vLog

#define Log Log_P
#define LogError LogError_P
#define LogAssertFailed LogAssertFailed_P
#define vLog vLog_P
#define LogBuf LogBuf_P

#else

//! Compatability macro for AVR
#define PSTR(str) str

//! When qQuiet is non-zero NORMAL logs will be suppressed.
int gQuiet = 0;

#endif

//! When qVerbose is non-zero LogVerbose output will be enabled.
int gVerbose = 0;

//! When qVerbose is non-zero LogDebug output will be enabled.
int gDebug = 0;

#if CFG_LOG_TO_BUFFER

volatile LOG_Buffer_t LOG_gBuffer;

#endif
#if CFG_LOG_ALLOW_DEFERRED_NL
int gDeferredNewline = 0;
#endif

// ---- Private Variables ---------------------------------------------------

#if CFG_LOG_USE_STDIO

//! Pointer to file stream to use for logging to stdio
FILE* gLogFs = NULL;

#endif

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

/**
 * @addtogroup Log
 * @{
 */

#if !defined(AVR)

//! @brief Default logging function for non-AVR platforms
void DefaultLogFunc(
    int logLevel,     //!< [in] Logging level for this message.
    const char* fmt,  //!< [in] printf style format string.
    va_list args      //!< [in] varadic arguments.
) {
    FILE* fs;

    if (gQuiet && (logLevel == LOG_LEVEL_NORMAL)) {
        return;
    }

    if (gLogFs == NULL) {
        fs = stderr;
    } else {
        fs = gLogFs;
    }

    if (logLevel == LOG_LEVEL_ERROR) {
        fprintf(fs, "ERROR: ");
    } else if (logLevel == LOG_LEVEL_ASSERT) {
        fprintf(fs, "ASSERT: ");
    }
    vfprintf(fs, fmt, args);
    fflush(fs);
}  // DefaultLogFunc

static LogFunc_t gLogFunc = DefaultLogFunc;

//! Sets the logging function to use.
void SetLogFunc(
    LogFunc_t logFunc  //!< [in] Pointer to logging function to use for logging.
) {
    gLogFunc = logFunc;
}  // SetLogFunc

#endif

#if CFG_LOG_USE_STDIO
//***************************************************************************
/**
 *   Sets the logging stream
 */
void LogInit(
    FILE* logFs  //!< [in] Pointer to file stream to use for logging.
) {
    gLogFs = logFs;
}  // LogInit

#elif CFG_LOG_USE_USB_DEBUG

static int LogToUsbDebugFunc(void* outParm, int ch) {
    (void)outParm;

    usb_debug_putchar(ch);
    if (ch == '\n') {
        usb_debug_flush_output();
    }
    return 1;
}

#else

static int LogToUartFunc(void* outParm, int ch) {
    CFG_LOG_PUT_CHAR_FUNC(ch);

    return 1;
}
#endif

//***************************************************************************
/**
 *   Logs a string using printf like semantics
 */

void Log(
    const char* fmt,  ///< printf style format specifier
    ...               ///< variable list of arguments
) {
    va_list args;

    va_start(args, fmt);
    vLog(fmt, args);
    va_end(args);
}

//***************************************************************************
/**
 *   Logs a string using printf like semantics
 */

void vLog(
    const char* fmt,  ///< printf style format specified
    va_list args      ///< variable list of arguments
) {
    // For now we call printf directly. A better way would be to install
    // a callback which does the real work

#if CFG_LOG_ALLOW_DEFERRED_NL
    if (gDeferredNewline && (*fmt != '\r')) {
        gDeferredNewline = 0;
        Log("\r\n");
    }
#endif

#if defined(AVR)
#if CFG_LOG_USE_STDIO
    if (gLogFs != NULL) {
        vfprintf_P(gLogFs, fmt, args);
    }
#elif CFG_LOG_USE_USB_DEBUG
    vStrXPrintf_P(LogToUsbDebugFunc, NULL, fmt, args);
#else
    vStrXPrintf_P(LogToUartFunc, NULL, fmt, args);
#endif
#else
    if (gLogFunc != NULL) {
        gLogFunc(LOG_LEVEL_NORMAL, fmt, args);
    }
#endif
}

#if !defined(AVR)

//***************************************************************************
/**
 *   Generic logging function
 */

void vLogFunc(
    int logLevel,     ///< Type of log
    const char* fmt,  ///< printf style format specified
    va_list args      ///< variable list of arguments
) {
    if (gLogFunc != NULL) {
        gLogFunc(logLevel, fmt, args);
    }
}  // vLogFunc

//***************************************************************************
/**
 *   Generic logging function
 */

void LogFunc(
    int logLevel,     ///< Type of log
    const char* fmt,  ///< printf style format specified
    ...               ///< variable list of arguments
) {
    va_list args;

    va_start(args, fmt);
    vLogFunc(logLevel, fmt, args);
    va_end(args);
}  // LogFunc

//***************************************************************************
/**
 *   Logs an error.
 */

void vLogError(
    const char* fmt,  ///< printf style format specified
    va_list args      ///< variable list of arguments
) {
    vLogFunc(LOG_LEVEL_ERROR, fmt, args);
}  // vLogError

#endif

/***************************************************************************/
/**
 *   Logs an error
 */

void LogError(
    const char* fmt,  ///< printf style format specifier
    ...               ///< variable list of arguments
) {
    va_list args;

#if defined(AVR)
    Log_P(PSTR("ERROR: "));

    va_start(args, fmt);
    vLog(fmt, args);
    va_end(args);
#else
    va_start(args, fmt);
    vLogFunc(LOG_LEVEL_ERROR, fmt, args);
    va_end(args);
#endif
}  // LogError

/***************************************************************************/
/**
 *   Logs an assertion failure
 */
void LogAssertFailed(
    const char* expr,      //!< [in] Expression that failed.
    const char* fileName,  //!< [in] Name of file the assert occurred in.
    unsigned lineNum,      //!< [in] Line number that the asseet occurred on.
    const char* function   //!< [in] Name of the function that the assert occurred in.
) {
#if defined(AVR)
    Log_P(PSTR("ASSERT failed: "));
    Log_P(fileName);
    Log_P(PSTR(": %d: "), lineNum);
    Log_P(function);
    Log_P(PSTR(" Assertion '"));
    Log_P(expr);
    Log_P(PSTR("' failed.\n"));
#else
    LogFunc(
        LOG_LEVEL_ASSERT, "%s: %d: %s Assertion '%s' failed.\n", fileName, lineNum, function, expr);
#endif
}  // LogAssertFailed

/***************************************************************************/
/**
 *   Logs an entry to the log buffer
 */

#if CFG_LOG_TO_BUFFER

void LogBuf(const char* fmt, uint8_t arg1, uint8_t arg2 LOG_EXTRA_PARAMS_DECL) {
#if defined(AVR)
    uint8_t sreg = SREG;
    cli();
#endif

    if (CBUF_IsFull(LOG_gBuffer)) {
        volatile LOG_Entry_t* entry = CBUF_GetLastEntryPtr(LOG_gBuffer);

        entry->fmt = PSTR("*** Lost Messages ***\n");
    } else {
        volatile LOG_Entry_t* entry = CBUF_GetPushEntryPtr(LOG_gBuffer);

        entry->fmt = fmt;
        entry->param1 = arg1;
        entry->param2 = arg2;

#if CFG_LOG_EXTRA_PARAMS
        entry->param3 = arg3;
        entry->param4 = arg4;
#endif

        CBUF_AdvancePushIdx(LOG_gBuffer);
    }

#if defined(AVR)
    SREG = sreg;
#endif
}  // LogBuf

#endif  // CFG_LOG_TO_BUFFER

/***************************************************************************/
/**
 *   Dumps any unlogged entries from the log buffer
 */

#if CFG_LOG_TO_BUFFER

void LogBufDump(void) {
    while (!CBUF_IsEmpty(LOG_gBuffer)) {
        volatile LOG_Entry_t* entry = CBUF_GetPopEntryPtr(LOG_gBuffer);

#if CFG_LOG_EXTRA_PARAMS
        Log(entry->fmt, entry->param1, entry->param2, entry->param3, entry->param4);
#else
        Log(entry->fmt, entry->param1, entry->param2);
#endif

        CBUF_AdvancePopIdx(LOG_gBuffer);
    }
}  // LogBufDump

#endif  // CFG_LOG_TO_BUFFER

/** @} */
