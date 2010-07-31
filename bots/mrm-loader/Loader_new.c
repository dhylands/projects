/*****************************************************************************
*
*     Copyright (c) 2003 Dave Hylands
*           All Rights Reserved
*
*   Permission is granted to any individual or institution to use, copy, or
*   redistribute this software so long as it is not sold for profit, and that
*   this copyright notice is retained.
*
*****************************************************************************/
/**
*
*   @file    Loader.c 
*
*   @brief   Small loader function which bumps up the baud rate used
*            by CPU32 bug.
*
*****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

/* ---- Public Variables -------------------------------------------------- */
/* ---- Private Constants and Types --------------------------------------- */
/* ---- Private Variables ------------------------------------------------- */
/* ---- Private Function Prototypes --------------------------------------- */
/* ---- Functions --------------------------------------------------------- */

/****************************************************************************/
/**
*   Main program
*
*/

void main( void )
{
    // The value of SCCRO[1] MUST be changed with different clock speeds
    // as enumerated by this table of values for 0xfffc09: 
    // 
    // Clock Freq (MHz)
    // Baud   || 8  | 16 | 25 |
    // -----------------------
    // 9600   || 27 | 55 | 82 |
    // 19200  || 14 | 27 | 41 |
    // 38400  || 7  | 14 | 20 |
    // 57600  || 5  | 9  | 14 |
    // 115200 || -- | -- | 7  |

    char *ptr;
    ptr = (char *)0xfffc08;

    if ( ptr[1] != 14 )
    {
        ptr[1] = 14;

        asm( " trap #15" );
        asm( " dc.w 0x0063" );
    }

    {
        unsigned long *p2 = (unsigned long *)0x4000;

        if ( *p2 == 0xbeefbeef )
        {
            asm( " trap #15" );
            asm( " dc.w 0x0063" );
        }
    }

} /* main */

/** @} */

