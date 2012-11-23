/****************************************************************************
*
*  Delay.h
*
*  Routines for waiting various amounts of time.
*
****************************************************************************/

#if !defined( DELAY_H )
#define DELAY_H

#if !defined( TYPES_H )
#   include "Types.h"
#endif

extern volatile uns16 gTickCount;

void Delay100uSec( uns8 num100uSec );
void Delay10uSec( uns8 num10uSec );

void InitTimer( void );
void WaitForTimer0Rollover( void );

#endif  // DELAY_H

