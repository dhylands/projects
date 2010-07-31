/****************************************************************************
*
*  Puts.c
*
*  Routines for outputting strings.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Types.h"
#include "Puts.h"
#include "Uart.h"

// ---- Public Variables ----------------------------------------------------

uns8  gDeferredNewline;

// ---- Private Constants and Types -----------------------------------------

static char buf[ 6 ];

// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/****************************************************************************
*
*  Writes a string the serial port.
*/

void dh_puts( const char *s )
{
   if ( gDeferredNewline )
   {
      putc( '\r' );
      putc( '\n' );
      putc( '+' );
      gDeferredNewline = 0;
   }

   while ( *s )
   {
      if ( *s == '\n' )
      {
         putc( '\r' );         
      }

      putc( *s );

      s++;
   }

} // puts

/****************************************************************************
*
*  Writes a number as a single digit.
*/

void putdigit( uns8 num )
{
   if ( num > 9 )
   {
      putc( '*' );
   }
   else
   {
      putc( num + '0' );
   }
   
} // putdigit

/****************************************************************************
*
*  Writes a single character to the serial port.
*/

void dh_putc( char ch )
{
    UART_PutChar( (char)ch );

} // putc

/****************************************************************************
*
*  Writes an integer in ASCII form to the serial port
*/

void putu8( uns8 num )
{
    int   i = 0;
    
    do
    {
        uns8  c;
        
        c = num % 10;
        c += '0';
        
        buf[ i ] = c;
        i++;
        num /= 10;
        
    } while ( num != 0 );
    
    while ( i < 3 )
    {
        buf[ i ] = ' ';
        i++;
    }
    
    while ( i > 0 )
    {
        i--;
        
        putc( buf[ i ]);
    }

} // putu8

/****************************************************************************
*
*  Writes an integer in ASCII form to the serial port
*/

void putu16( uns16 num )
{
#if 0
   uns16 num2;

   num2 =  num & 0xFF00;
   num2 >>= 8;
   putu8( (uns8)num2 );
   putc( ':' );
   num2 = num & 0x00FF;    
   putu8( (uns8)num2 );

#else

   int   i = 0;

   do
   {
      uns16  c;

      c = num % 10;
      c += '0';
      
      buf[ i ] = (uns8)c;
      i++;
      num /= 10;
      
   } while ( num != 0 );

//   puts( "i:" ); putu8( i ); putc( ' ' );

   while ( i > 0 )
   {
      i--;

      putc( buf[ i ]);
   }

//   puts (" done PutU16\n" );
#endif

} // putu16
