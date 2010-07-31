/****************************************************************************
*
*  Puts.h
*
*  Routines for outputting strings.
*
****************************************************************************/

#if !defined( PUTS_H )
#define  PUTS_H

#if !defined( TYPES_H )
#   include "Types.h"
#endif

#if !defined( DEBUG_H )
#  include "Debug.h"
#endif

uns8  gDeferredNewline;

#define putc    dh_putc
#define puts    dh_puts

void dh_putc( char ch );
void dh_puts( const char *s );
void putdigit( uns8 num );
void putu8( uns8 num );
void putu16( uns16 num );

#endif   // PUTS_H

