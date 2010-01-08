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
*   @file   AvrInfo.h
*
*   @brief  Contains fucntions for mapping AVR Part numbers to information
*           structures.
*
****************************************************************************/

#if !defined( AVRINFO_H )
#define AVRINFO_H         ///< Include Guard

/* ---- Include Files ---------------------------------------------------- */

#include <inttypes.h>

/* ---- Constants and Types ---------------------------------------------- */

typedef struct
{
    const char *m_string;       ///< Human readable prt identification
    uint16_t    m_signature;    ///< Signature minus the vendor ID
    uint32_t    m_flashSize;    ///< Size of the flash (in bytes)
    uint16_t    m_bootSize;     ///< Max size of bootloader area.
    uint16_t    m_eepromSize;   ///< Size of the EEPROM area (in bytes)
    uint16_t    m_pageSize;     ///< Page size in bytes (for flash programming)
    uint8_t     m_pageShift;    ///< Page Size = 1 << m_pageShift

} AvrInfo_t;

// Every AVR is assigned a 3 byte signature sequence. The first byte is a 
// vendor ID and is the value 0x1E for Atmel. We use the remaining two bytes.

#define AVR_ATMEGA_48_STRING        "ATMega48"                                    
#define AVR_ATMEGA_48_SIGNATURE     0x9205
#define AVR_ATMEGA_48_FLASH_SIZE    (  4 * 1024 )
#define AVR_ATMEGA_48_BOOT_SIZE     (  4 * 1024 )
#define AVR_ATMEGA_48_EEPROM_SIZE   256
#define AVR_ATMEGA_48_PAGE_SHIFT    6
#define AVR_ATMEGA_48_PAGE_SIZE     64

#define AVR_ATMEGA_8_STRING         "ATMega8"                                    
#define AVR_ATMEGA_8_SIGNATURE      0x9307
#define AVR_ATMEGA_8_FLASH_SIZE     (  8 * 1024 )
#define AVR_ATMEGA_8_BOOT_SIZE      (  2 * 1024 )
#define AVR_ATMEGA_8_EEPROM_SIZE    512
#define AVR_ATMEGA_8_PAGE_SHIFT     6 
#define AVR_ATMEGA_8_PAGE_SIZE      64

#define AVR_ATMEGA_88_STRING        "ATMega88"                                    
#define AVR_ATMEGA_88_SIGNATURE     0x930A
#define AVR_ATMEGA_88_FLASH_SIZE    (  8 * 1024 )
#define AVR_ATMEGA_88_BOOT_SIZE     (  2 * 1024 )
#define AVR_ATMEGA_88_EEPROM_SIZE   512
#define AVR_ATMEGA_88_PAGE_SHIFT    6
#define AVR_ATMEGA_88_PAGE_SIZE     64

#define AVR_ATMEGA_16_STRING        "ATMega16"                                    
#define AVR_ATMEGA_16_SIGNATURE     0x9403
#define AVR_ATMEGA_16_FLASH_SIZE    ( 16 * 1024 )
#define AVR_ATMEGA_16_BOOT_SIZE     (  2 * 1024 )
#define AVR_ATMEGA_16_EEPROM_SIZE   512
#define AVR_ATMEGA_16_PAGE_SHIFT    7
#define AVR_ATMEGA_16_PAGE_SIZE     128

#define AVR_ATMEGA_168_STRING       "ATMega168"                                    
#define AVR_ATMEGA_168_SIGNATURE    0x9406
#define AVR_ATMEGA_168_FLASH_SIZE   ( 16 * 1024 )
#define AVR_ATMEGA_168_BOOT_SIZE    (  2 * 1024 )
#define AVR_ATMEGA_168_EEPROM_SIZE  512
#define AVR_ATMEGA_168_PAGE_SHIFT   7
#define AVR_ATMEGA_168_PAGE_SIZE    128

#define AVR_ATMEGA_169_STRING       "ATMega169"                                    
#define AVR_ATMEGA_169_SIGNATURE    0x9405
#define AVR_ATMEGA_169_FLASH_SIZE   ( 16 * 1024 )
#define AVR_ATMEGA_169_BOOT_SIZE    (  2 * 1024 )
#define AVR_ATMEGA_169_EEPROM_SIZE  512
#define AVR_ATMEGA_169_PAGE_SHIFT   7
#define AVR_ATMEGA_169_PAGE_SIZE    128

#define AVR_ATMEGA_32_STRING        "ATMega32"                                    
#define AVR_ATMEGA_32_SIGNATURE     0x9502
#define AVR_ATMEGA_32_FLASH_SIZE    ( 32 * 1024 )
#define AVR_ATMEGA_32_BOOT_SIZE     (  4 * 1024 )
#define AVR_ATMEGA_32_EEPROM_SIZE   (  1 * 1024 )
#define AVR_ATMEGA_32_PAGE_SHIFT    7
#define AVR_ATMEGA_32_PAGE_SIZE     128

#define AVR_ATMEGA_64_STRING        "ATMega64"                                    
#define AVR_ATMEGA_64_SIGNATURE     0x9602
#define AVR_ATMEGA_64_FLASH_SIZE    ( 64 * 1024 )
#define AVR_ATMEGA_64_BOOT_SIZE     (  8 * 1024 )
#define AVR_ATMEGA_64_EEPROM_SIZE   (  2 * 1024 )
#define AVR_ATMEGA_64_PAGE_SHIFT    8
#define AVR_ATMEGA_64_PAGE_SIZE     256

#define AVR_ATMEGA_644_STRING       "ATMega644"
#define AVR_ATMEGA_644_SIGNATURE    0x9609
#define AVR_ATMEGA_644_FLASH_SIZE   ( 64 * 1024 )
#define AVR_ATMEGA_644_BOOT_SIZE    (  8 * 1024 )
#define AVR_ATMEGA_644_EEPROM_SIZE  (  2 * 1024 )
#define AVR_ATMEGA_644_PAGE_SHIFT   8
#define AVR_ATMEGA_644_PAGE_SIZE    256

#define AVR_ATMEGA_128_STRING       "ATMega128"                                    
#define AVR_ATMEGA_128_SIGNATURE    0x9702
#define AVR_ATMEGA_128_FLASH_SIZE   ( 128 * 1024 )
#define AVR_ATMEGA_128_BOOT_SIZE    (   8 * 1024 )
#define AVR_ATMEGA_128_EEPROM_SIZE  (   4 * 1024 )
#define AVR_ATMEGA_128_PAGE_SHIFT   8
#define AVR_ATMEGA_128_PAGE_SIZE    256

#define AVR_PART_STRING( part )         AVR_ ## part ## _STRING
#define AVR_PART_SIGNATURE( part )      AVR_ ## part ## _SIGNATURE
#define AVR_PART_FLASH_SIZE( part )     AVR_ ## part ## _FLASH_SIZE
#define AVR_PART_BOOT_SIZE( part )      AVR_ ## part ## _BOOT_SIZE
#define AVR_PART_EEPROM_SIZE( part )    AVR_ ## part ## _EEPROM_SIZE
#define AVR_PART_PAGE_SHIFT( part )     AVR_ ## part ## _PAGE_SHIFT
#define AVR_PART_PAGE_SIZE( part )      AVR_ ## part ## _PAGE_SIZE

#if defined( __AVR__ )

#if defined ( __AVR_ATmega48__ )
#   define  AVR_PART_NUM    ATMEGA_48

#elif defined( __AVR_ATmega8__ )
#   define  AVR_PART_NUM    ATMEGA_8

#elif defined( __AVR_ATmega88__ )
#   define  AVR_PART_NUM    ATMEGA_88

#elif defined( __AVR_ATmega16__ )
#   define  AVR_PART_NUM    ATMEGA_16

#elif defined( __AVR_ATmega168__ )
#   define  AVR_PART_NUM    ATMEGA_168

#elif defined( __AVR_ATmega169__ )
#   define  AVR_PART_NUM    ATMEGA_169

#elif defined( __AVR_ATmega32__ )
#   define  AVR_PART_NUM    ATMEGA_32

#elif defined( __AVR_ATmega64__ )
#   define  AVR_PART_NUM    ATMEGA_64

#elif defined( __AVR_ATmega128__ )
#   define  AVR_PART_NUM    ATMEGA_128

#else
#   error Unrecognized AVR Device
#endif

#define AVR_PART_STRING2( part )        AVR_PART_STRING(  part )
#define AVR_PART_SIGNATURE2( part )     AVR_PART_SIGNATURE( part )
#define AVR_PART_FLASH_SIZE2( part )    AVR_PART_FLASH_SIZE( part )
#define AVR_PART_BOOT_SIZE2( part )     AVR_PART_BOOT_SIZE( part )
#define AVR_PART_EEPROM_SIZE2( part )   AVR_PART_EEPROM_SIZE( part )
#define AVR_PART_PAGE_SHIFT2( part )    AVR_PART_PAGE_SHIFT( part )
#define AVR_PART_PAGE_SIZE2( part )     AVR_PART_PAGE_SIZE( part )

#define AVR_STRING      AVR_PART_STRING2( AVR_PART_NUM )
#define AVR_SIGNATURE   AVR_PART_SIGNATURE2( AVR_PART_NUM )
#define AVR_FLASH_SIZE  AVR_PART_FLASH_SIZE2( AVR_PART_NUM )
#define AVR_BOOT_SIZE   AVR_PART_BOOT_SIZE2( AVR_PART_NUM )
#define AVR_EEPROM_SIZE AVR_PART_EEPROM_SIZE2( AVR_PART_NUM )
#define AVR_PAGE_SHIFT  AVR_PART_PAGE_SHIFT2( AVR_PART_NUM )
#define AVR_PAGE_SIZE   AVR_PART_PAGE_SIZE2( AVR_PART_NUM )

#endif  // __AVR__

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

#if defined( __cplusplus )
extern "C"
{
#endif

const AvrInfo_t *FindAvrInfoBySignature( uint16_t signature );

#if defined( __cplusplus )
}
#endif

#endif /* AVRINFO_H */

