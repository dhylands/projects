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
*   @file   lcd-stdio.c
*
*   @brief  Integrates printf style output into the AVR C runtime library.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <stdio.h>

#include "lcd.h"

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */

//***************************************************************************
/**
*   This function is called by fprintf when outputting a character.
*/
int  LCD_PutCharStdio( char ch, FILE *fs )
{
    (void)fs;

    LCD_PutChar( ch );

    return 1;

} // LCD_PutCharStdio

