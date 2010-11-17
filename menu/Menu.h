/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/
/**
*
*   @file   Menu.h
*
*   @brief  Menu definitions suitable for use on a simple LCD
*
****************************************************************************/

#if !defined( MENU_H )
#define MENU_H

/* ---- Include Files ---------------------------------------------------- */

#include <stdint.h>
#if defined( AVR )
#   include <avr/pgmspace.h>
#else
typedef char    prog_char;
#define PSTR(str )  str
#define PROGMEM
#endif

/* ---- Constants and Types ---------------------------------------------- */


typedef uint8_t MENU_Type;

#define MENU_TYPE_MENU  0
#define MENU_TYPE_PROC  1
#define MENU_TYPE_BYTE  2
#define MENU_TYPE_UINT  3
#define MENU_TYPE_INT   4

typedef enum
{
    MENU_EVENT_INIT,
    MENU_EVENT_SELECT,
    MENU_EVENT_LEFT,
    MENU_EVENT_RIGHT,
    MENU_EVENT_UP,
    MENU_EVENT_DOWN,
    MENU_EVENT_TIMER,

} MENU_Event_t;

typedef const prog_char *(*MENU_Proc_t)( MENU_Event_t event );

struct MENU_Item_s;
typedef struct MENU_Item_s          MENU_MemItem_t;
typedef const struct MENU_Item_s    MENU_ProgItem_t PROGMEM;


typedef struct
{
    uint8_t            *bytePtr;
    uint8_t             minVal;
    uint8_t             maxVal;

} MENU_ByteVal_t;

typedef struct
{
    uint16_t           *uintPtr;
    uint16_t            minVal;
    uint16_t            maxVal;

} MENU_UIntVal_t;

typedef struct
{
    int16_t            *intPtr;
    int16_t             minVal;
    int16_t             maxVal;

} MENU_IntVal_t;

typedef union
{
    MENU_ProgItem_t    *menu;
    MENU_ByteVal_t      byteVal;
    MENU_UIntVal_t      uintVal;
    MENU_IntVal_t       intVal;
    MENU_Proc_t         proc;

} MENU_Value_t;

struct MENU_Item_s
{
    const prog_char    *name;
    MENU_Type           type;
    MENU_Value_t        val;

};


#define MENU_END    { NULL, 0 }

#define MENU_ITEM_MENU( _name, _items )   \
{ \
    .name = _name, \
    .type = MENU_TYPE_MENU, \
    .val = { .menu = _items } \
}
                                
#define MENU_ITEM_PROC( _name, _proc ) \
{ \
    .name = _name, \
    .type = MENU_TYPE_PROC, \
    .val = { .proc = _proc } \
}

#define MENU_ITEM_BYTE( _name, _bytePtr, _minVal, _maxVal ) \
{ \
    .name = _name, \
    .type = MENU_TYPE_BYTE, \
    .val = { .byteVal = { .bytePtr = _bytePtr, .minVal = _minVal, .maxVal = _maxVal }} \
}

#define MENU_ITEM_UINT( _name, _uintPtr, _minVal, _maxVal ) \
{ \
    .name = _name, \
    .type = MENU_TYPE_UINT, \
    .val = { .uintVal = { .uintPtr = _uintPtr, .minVal = _minVal, .maxVal = _maxVal }} \
}

#define MENU_ITEM_INT( _name, _intPtr, _minVal, _maxVal ) \
{ \
    .name = _name, \
    .type = MENU_TYPE_INT, \
    .val = { .intVal = { .intPtr = _intPtr, .minVal = _minVal, .maxVal = _maxVal }} \
}

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

uint8_t MENU_IsActive( void );
uint8_t MENU_IsModified( void );
void    MENU_ClearModified( void );

void MENU_Init( MENU_ProgItem_t *topMenu );
void MENU_Event( MENU_Event_t event );
void MENU_Exit( void );

#endif  // MENU_H

