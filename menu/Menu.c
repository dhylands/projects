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
*   @file   Menu.c
*
*   @brief  Menu definitions suitable for use on a simple LCD
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <string.h>
#include "Config.h"
#include "Delay.h"
#include "Lcd.h"
#include "Log.h"
#include "Menu.h"

/* ---- Public Variables -------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------- */

#define MENU_LOG_ENABLED    0

#if MENU_LOG_ENABLED
#   define MENU_LOG( fmt, args... )  Log( fmt, ## args )
#else
#   define MENU_LOG( fmt, args... )
#endif

#if !defined( CFG_MENU_MAX_DEPTH )
#   define CFG_MENU_MAX_DEPTH   8
#endif

#define MENU_STACK_INACTIVE 0xFF

/* ---- Private Variables ------------------------------------------------- */

#if MENU_LOG_ENABLED

static char *gMenuEventStr[] =
{
    "Init", "Select", "Left", "Right", "Up", "Down", "Timer"
};

#endif

static  MENU_ProgItem_t    *gMenuStack[ CFG_MENU_MAX_DEPTH ];
static  uint8_t             gMenuStackTop;
static  uint8_t             gMenuIsEditing;
static  uint8_t             gMenuModified;

static  union
{
    uint8_t     byteVal;
    uint16_t    uintVal;
    int16_t     intVal;

} gMenuEditVal;

typedef void (*MENU_EventEditFunc_t)( MENU_Event_t event, MENU_MemItem_t *item );

static  void MENU_EventEditMenu( MENU_Event_t event, MENU_MemItem_t *item );
static  void MENU_EventEditProc( MENU_Event_t event, MENU_MemItem_t *item );
static  void MENU_EventEditByte( MENU_Event_t event, MENU_MemItem_t *item );
static  void MENU_EventEditUInt( MENU_Event_t event, MENU_MemItem_t *item );

static  const MENU_EventEditFunc_t    gMenuEditFunc[] =
{
    MENU_EventEditMenu,
    MENU_EventEditProc,
    MENU_EventEditByte,
    MENU_EventEditUInt,
};


/* ---- Private Function Prototypes --------------------------------------- */

/* ---- Functions --------------------------------------------------------- */

//***************************************************************************
/**
*   Reads a menu item from flash
*/

static inline MENU_MemItem_t *MENU_GetItem( const MENU_ProgItem_t *progItem, MENU_MemItem_t *memItem )
{
    memcpy_P( memItem, progItem, sizeof( *memItem ));

    return memItem;

} // MENU_GetItem

//***************************************************************************
/**
*   Determines if the menu is currently active or not
*/

uint8_t MENU_IsActive( void )
{
    return gMenuStackTop > 0;
}

//***************************************************************************
/**
*   Determines if the menu is currently active or not
*/

uint8_t MENU_IsModified( void )
{
    return gMenuModified;
}

//***************************************************************************
/**
*   Determines if the menu is currently active or not
*/

void MENU_ClearModified( void )
{
    gMenuModified = 0;
}

//***************************************************************************
/**
*   Determines if the menu is currently being edited or not
*/

static inline uint8_t MENU_IsEditing( void )
{
    return gMenuIsEditing;
}

//***************************************************************************
/**
*   Puts the indicated item on the top of the menu stack
*/

static inline void MENU_Activate( MENU_ProgItem_t *menuItem )
{
    gMenuStackTop++;
    gMenuStack[ gMenuStackTop ] = menuItem;

} // MENU_Activate

//***************************************************************************
/**
*   Delays for the specified number of milliseconds
*/

static inline void MENU_Delay( unsigned msec )
{
    ms_spin( msec );

} // MENU_Delay

//***************************************************************************
/**
*   Draws the current menu item
*/

void MENU_Draw( void )
{
    char           *s;
    char            fmtStr[ 10 ];
    uint16_t        val = 0;
    uint16_t        maxVal = 0;
    int16_t         ival = 0;
    int16_t         maxIVal = 0;
    uint8_t         maxLen;
    uint8_t         valLen;
    MENU_MemItem_t  item;
    uint8_t         signedVal = 0;

    if ( !MENU_IsActive() )
    {
        return;
    }

    if ( MENU_IsEditing() )
    {
        MENU_GetItem( gMenuStack[ gMenuStackTop ], &item );
    }
    else
    {
        // By only drawing when we're not editing, we reduce flicker

        LCD_Clear();

        // First of all, draw the title of the containing menu

        MENU_GetItem( gMenuStack[ gMenuStackTop - 1 ], &item );
        LCD_PutStr_P( item.name );

        // Now draw the currently selected item

        LCD_MoveTo( 0, 1 );

        MENU_GetItem( gMenuStack[ gMenuStackTop ], &item );
        LCD_PutStr_P( item.name );
    }

    switch ( item.type )
    {
        case MENU_TYPE_BYTE:
        {
            if ( MENU_IsEditing() )
            {
                val = gMenuEditVal.uintVal;
            }
            else
            {
                val = *item.val.byteVal.bytePtr;
            }
            maxVal =  item.val.byteVal.maxVal;
            break;
        }

        case MENU_TYPE_UINT:
        {
            if ( MENU_IsEditing() )
            {
                val = gMenuEditVal.uintVal;
            }
            else
            {
                val = *item.val.uintVal.uintPtr;
            }
            maxVal =  item.val.uintVal.maxVal;
            break;
        }

        case MENU_TYPE_INT:
        {
            signedVal = 1;

            if ( MENU_IsEditing() )
            {
                ival = gMenuEditVal.intVal;
            }
            else
            {
                ival = *item.val.intVal.intPtr;
            }
            maxIVal =  item.val.intVal.maxVal;
            break;
        }

        default:
        {
            return;
        }
    }

    if ( signedVal )
    {
        snprintf( fmtStr, sizeof( fmtStr ), "%d", maxIVal );
        maxLen = strlen( fmtStr );
        snprintf( fmtStr, sizeof( fmtStr ), "%d", ival );
    }
    else
    {
        snprintf( fmtStr, sizeof( fmtStr ), "%u", maxVal );
        maxLen = strlen( fmtStr );
        snprintf( fmtStr, sizeof( fmtStr ), "%u", val );
    }

    LCD_MoveTo( LCD_NumCols() - maxLen - 2, LCD_NumLines() - 1 );

    LCD_PutChar( MENU_IsEditing() ? '[' : ' ' );

    valLen = strlen( fmtStr );
    while ( maxLen > valLen )
    {
        LCD_PutChar( ' ' );
        maxLen--;
    }

    s = fmtStr;
    while ( valLen > 0 )
    {
        LCD_PutChar( *s++ );
        valLen--;
    }

    LCD_PutChar( MENU_IsEditing() ? ']' : ' ' );

} // MENU_Draw

//***************************************************************************
/**
*   Dumps the contents of the menu
*/

#if MENU_LOG_ENABLED

void MENU_Dump( uint8_t indent, MENU_ProgItem_t *menuItem )
{
    uint8_t         i;
    MENU_MemItem_t  item;

    while ( MENU_GetItem( menuItem, &item )->name != NULL )
    {
        for ( i = 0; i < indent; i++ ) 
        {
            Log( "  " );
        }
        Log( "Name: " );
        Log_P( item.name );
        Log( ", type: %d\n", item.type );

        if ( item.type == MENU_TYPE_MENU )
        {
            MENU_Dump( indent + 1, item.val.menu );
        }
        menuItem++;
    }

} // MENU_Dump

#endif

//***************************************************************************
/**
*   Called to process events while editing a "procedure"
*/

static void MENU_EventEditMenu( MENU_Event_t event, MENU_MemItem_t *item )
{
    MENU_LOG(  "MENU_EventEditMenu: %s\n", gMenuEventStr[ event ]);

    switch ( event )
    {
        case MENU_EVENT_SELECT:
        {
            if ( item->type != MENU_TYPE_MENU )
            {
                gMenuIsEditing = 1;

                MENU_Event( MENU_EVENT_INIT );
                break;
            }

            // Fall thru
        }

        case MENU_EVENT_RIGHT:
        {
            if ( item->type == MENU_TYPE_MENU )
            {
                // The item selected is another menu. Activate it.

                MENU_Activate( item->val.menu );
            }
            break;
        }

        case MENU_EVENT_LEFT:
        {
            gMenuStackTop--;
            break;
        }

        case MENU_EVENT_UP:
        {
            if ( gMenuStack[ gMenuStackTop ] <= MENU_GetItem( gMenuStack[ gMenuStackTop - 1 ], item )->val.menu ) 
            {
                while ( MENU_GetItem( gMenuStack[ gMenuStackTop ], item )->name != NULL )
                {
                    gMenuStack[ gMenuStackTop ]++;
                }
            }
            gMenuStack[ gMenuStackTop ]--;
            break;
        }

        case MENU_EVENT_DOWN:
        {
            gMenuStack[ gMenuStackTop ]++;

            if ( MENU_GetItem( gMenuStack[ gMenuStackTop ], item )->name == NULL )
            {
                gMenuStack[ gMenuStackTop ] = MENU_GetItem( gMenuStack[ gMenuStackTop - 1 ], item )->val.menu;
            }
            break;
        }

        default:
            break;
    }

} // MENU_EventEditMenu

//***************************************************************************
/**
*   Called to process events while editing a "procedure"
*/

static void MENU_EventEditProc( MENU_Event_t event, MENU_MemItem_t *item )
{
    const prog_char *str;

    MENU_LOG(  "MENU_EventEditProc: %s\n", gMenuEventStr[ event ]);

    if ( event == MENU_EVENT_LEFT )
    {
        // Acts like a cancel

        gMenuIsEditing = 0;
        return;
    }

    if (( str = item->val.proc( event )) != NULL )
    {
        LCD_Clear();
        LCD_PutStr_P( str );
        MENU_Delay( 1000 );

        gMenuIsEditing = 0;
    }

} // MENU_EventEditProc

//***************************************************************************
/**
*   Called to process events while editing an unsigned entity
*/

static void MENU_EventEditUnsigned( MENU_Event_t event, uint16_t *valPtr, uint16_t minVal, uint16_t maxVal )
{
    if ( *valPtr < minVal )
    {
        *valPtr = minVal;
    }
    if ( *valPtr > maxVal )
    {
        *valPtr = maxVal;
    }

    switch ( event )
    {
        case MENU_EVENT_DOWN:
        {
            if ( *valPtr > minVal  )
            {
                (*valPtr)--;
            }
            break;
        }

        case MENU_EVENT_UP:
        {
            if ( *valPtr < maxVal  )
            {
                (*valPtr)++;
            }
            break;
        }

        default:
            break;
    }
}

//***************************************************************************
/**
*   Called to process events while editing a BYTE
*/

static void MENU_EventEditByte( MENU_Event_t event, MENU_MemItem_t *item )
{
    MENU_LOG( "MENU_EventEditByte: %s\n", gMenuEventStr[ event ]);

    if ( event == MENU_EVENT_INIT )
    {
        gMenuEditVal.uintVal = *item->val.byteVal.bytePtr;
    }

    MENU_EventEditUnsigned( event, &gMenuEditVal.uintVal, item->val.byteVal.minVal, item->val.byteVal.maxVal );

    if ( event == MENU_EVENT_SELECT )
    {
        *item->val.byteVal.bytePtr = (uint8_t)gMenuEditVal.uintVal;
    }

} // MENU_EventEditByte

//***************************************************************************
/**
*   Called to process events while editing a UINT
*/

static void MENU_EventEditUInt( MENU_Event_t event, MENU_MemItem_t *item )
{
    MENU_LOG( "MENU_EventEditUInt: %s\n", gMenuEventStr[ event ]);

    if ( event == MENU_EVENT_INIT )
    {
        gMenuEditVal.uintVal = *item->val.uintVal.uintPtr;
    }

    MENU_EventEditUnsigned( event, &gMenuEditVal.uintVal, item->val.uintVal.minVal, item->val.uintVal.maxVal );

    if ( event == MENU_EVENT_SELECT )
    {
        *item->val.uintVal.uintPtr = gMenuEditVal.uintVal;
    }

} // MENU_EventEditUInt

//***************************************************************************
/**
*   Allows a menu procedure to exit the menu system
*/

void MENU_Exit( void )
{
    gMenuStackTop = 0;
    gMenuIsEditing = 0;

} // MENU_Exit

//***************************************************************************
/**
*   Initializes the menu subsystem and sets the top level menu
*/

void MENU_Init( MENU_ProgItem_t *topMenu )
{
    gMenuStack[ 0 ] = topMenu;
    gMenuStackTop = 0;

#if MENU_LOG_ENABLED
    MENU_Dump( 0,  topMenu );
#endif

} // MENU_Init

//***************************************************************************
/**
*   Handles a menu event (typically a key/button press)
*/

void MENU_Event( MENU_Event_t event )
{
    //MENU_LOG( "MENU_Event: %s\n", gMenuEventStr[ event ]);

    if ( !MENU_IsActive() )
    {
        MENU_MemItem_t  mainMenu;

        // Menu isn't currently being displayed. Activate at the top.

        gMenuStackTop = 0;
        gMenuModified = 0;

        MENU_GetItem( gMenuStack[ 0 ], &mainMenu ); 

        MENU_Activate( mainMenu.val.menu );
    }
    else
    {
        MENU_MemItem_t  topItem;

        MENU_GetItem( gMenuStack[ gMenuStackTop ], &topItem ); 

        if ( MENU_IsEditing() )
        {
            (gMenuEditFunc[ topItem.type ])( event, &topItem );

            if ( event == MENU_EVENT_LEFT )
            {
                // Acts like a cancel

                gMenuIsEditing = 0;
            }
            else
            if ( event == MENU_EVENT_SELECT )
            {
                // Value was selected

                gMenuModified = 1;
                gMenuIsEditing = 0;
            }
        }
        else
        {
            MENU_EventEditMenu( event, &topItem );
        }
    }

    if ( event != MENU_EVENT_TIMER )
    {
        MENU_Draw();
    }

} // MENU_Event

