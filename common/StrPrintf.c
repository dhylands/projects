/****************************************************************************
*
*   Since this code originated from code which is public domain, I
*   hereby declare this code to be public domain as well.
*
*   Dave Hylands - dhylands@gmail.com
*
****************************************************************************/
/**
*
*  @file    StrPrintf.cpp
*
*  @brief   Implementation of a re-entrant printf function.
*
*  Implements a reentrant version of the printf function. Also allows a 
*  function pointer to be provided to perform the actual output.
*
*  This version of printf was taken from 
*
*     http://www.efgh.com/software/gprintf.htm
*
*  This software was posted by the author as being in the "public" domain.
*  I've taken the original gprintf.txt and made some minor revisions.
*
****************************************************************************/

/**
* @defgroup StrPrintf  String Formatting
* @ingroup  Str
*/
/**
*  @defgroup StrPrintfInternal   String Formatting Internals
*  @ingroup  StrPrintf
*/

/* ---- Include Files ---------------------------------------------------- */

#include "Str.h"
#include <limits.h>
#include <string.h>

#if defined( AVR )

#undef  StrPrintf
#undef  vStrPrintf

#undef  StrXPrintf
#undef  vStrXPrintf

#define StrPrintf   StrPrintf_P
#define vStrPrintf  vStrPrintf_P

#define StrXPrintf  StrXPrintf_P
#define vStrXPrintf vStrXPrintf_P

#else

#define prog_char               char
#define pgm_read_byte( addr )  *addr

#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/**
 * @addtogroup StrPrintfInternal
 * @{
 */

/**
 * Controls a variety of output options.
 */
  
typedef enum
{
    NO_OPTION       = 0x00, /**< No options specified.						*/
	MINUS_SIGN		= 0x01, /**< Should we print a minus sign?              */
	RIGHT_JUSTIFY   = 0x02, /**< Should field be right justified?           */
	ZERO_PAD        = 0x04, /**< Should field be zero padded?               */
	CAPITAL_HEX     = 0x08  /**< Did we encounter %X?                       */

} FmtOption;

/** @def IsOptionSet( p, x )   Determines if an option has been set.       */
/** @def IsOptionClear( p, x ) Determines if an option is not set.         */
/** @def SetOption( p, x )     Sets an option.                             */
/** @def ClearOption( p, x )   Unsets an option.                           */

#define  IsOptionSet( p, x )     (( (p)->options & (x)) != 0 )
#define  IsOptionClear( p, x )   (( (p)->options & (x)) == 0 )
#define  SetOption( p, x )       (p)->options = (FmtOption)((p)->options |  (x))
#define  ClearOption( p, x )     (p)->options = (FmtOption)((p)->options & ~(x))

/**
 * Internal structure which is used to allow vStrXPrintf() to be reentrant.
 */

typedef struct 
{
   /** Number of characters output so far.                                 */
   int            numOutputChars;

   /** Options determined from parsing format specification.               */
   FmtOption      options;          

   /** Minimum number of characters to output.                             */
   short          minFieldWidth;    

   /** The exact number of characters to output.                           */
   short          editedStringLen;

   /** The number of leading zeros to output.                              */
   short          leadingZeros;

   /** The function to call to perform the actual output.                  */
   StrXPrintfFunc outFunc;

   /** Parameter to pass to the output function.                           */
   void          *outParm;

} Parameters;

/**
 * Internal structure used by vStrPrintf() .
 */
typedef struct
{
   char *str;     /**< Buffer to store results into.                       */
   int   maxLen;  /**< Maximum number of characters which can be stored.   */

} StrPrintfParms;

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

static void OutputChar( Parameters *p, int c );
static void OutputField( Parameters *p, char *s );
static int  StrPrintfFunc( void *outParm, int ch );

/** @} */

/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup StrPrintf
 * @{
 */

/***************************************************************************/
/**
*  Writes formatted data into a user supplied buffer.
*
*  @param   outStr   (out) Place to store the formatted string.
*  @param   maxLen   (in)  Max number of characters to write into @a outStr.
*  @param   fmt      (in)  Format string (see vStrXPrintf() for sull details).
*/

int StrPrintf( char *outStr, int maxLen, const char *fmt, ... )
{
   int      rc;
   va_list  args;

   va_start( args, fmt );
   rc = vStrPrintf( outStr, maxLen, fmt, args );
   va_end( args );

   return rc;

} // StrPrintf

/***************************************************************************/
/**
*  Generic printf function which writes formatted data by calling a user 
*  supplied function.
*
*  @a outFunc will be called to output each character. If @a outFunc returns
*  a number >= 0, then StrXPrintf will continue to call @a outFunc with 
*  additional characters.
*
*  If @a outFunc returns a negative number, then StrXPrintf will stop 
*  calling @a outFunc and will return the non-negative return value.
*
*  @param   outFunc  (in)  Pointer to function to call to do the actual output.
*  @param   outParm  (in)  Passed to @a outFunc.
*  @param   fmt      (in)  Format string (see vStrXPrintf() for sull details).
*
*/

int StrXPrintf( StrXPrintfFunc outFunc, void *outParm, const prog_char *fmt, ... )
{
   int      rc;
   va_list  args;

   va_start( args, fmt );
   rc = vStrXPrintf( outFunc, outParm, fmt, args );
   va_end( args );

   return rc;

} // StrxPrintf

/***************************************************************************/
/**
*  Writes formatted data into a user supplied buffer.
*
*  @param   outStr   (out) Place to store the formatted string.
*  @param   maxLen   (in)  Max number of characters to write into @a outStr.
*  @param   fmt      (in)  Format string (see vStrXPrintf() for sull details).
*  @param   args     (in)  Arguments in a format compatible with va_arg().
*/

int vStrPrintf( char *outStr, int maxLen, const prog_char *fmt, va_list args )
{
   StrPrintfParms    strParm;

   strParm.str    = outStr;
   strParm.maxLen = maxLen - 1; /* Leave space for temrinating null char   */

   return vStrXPrintf( StrPrintfFunc, &strParm, fmt, args );

} // vStrPrintf

/***************************************************************************/
/**
*  Generic, reentrant printf function. This is the workhorse of the StrPrintf
*  functions.
*
*  @a outFunc will be called to output each character. If @a outFunc returns
*  a number >= 0, then vStrXPrintf will continue to call @a outFunc with 
*  additional characters.
*
*  If @a outFunc returns a negative number, then vStrXPrintf will stop calling
*  @a outFunc and will return the non-negative return value.
*
*  The format string @a fmt consists of ordinary characters, escape
*  sequences, and format specifications. The ordinary characters and escape
*  sequences are output in their order of appearance. Format specifications
*  start with a percent sign (%) and are read from left to right. When 
*  the first format specification (if any) is encountered, it converts the 
*  value of the first argument after @a fmt and outputs it accordingly. 
*  The second format specification causes the second argument to be 
*  converted and output, and so on. If there are more arguments than there 
*  are format specifications, the extra arguments are ignored. The 
*  results are undefined if there are not enough arguments for all the 
*  format specifications.
*
*  A format specification has optional, and required fields, in the following
*  form:
*
*     %[flags][width][.precision][l]type
*
*  Each field of the format specification is a single character or a number
*  specifying a particular format option. The simplest format specification
*  contains only the percent sign and a @b type character (for example %s).
*  If a percent sign is followed by a character that has no meaning as a 
*  format field, the character is sent to the output function. For example, 
*  to print a percent-sign character, use %%.
*
*  The optional fields, which appear before the type character, control 
*  other aspects of the formatting, as follows:
*
*  @b flags may be one of the following:
*
*  - - (minus sign) left align the result within the given field width.
*  - 0 (zero) Zeros are added until the minimum width is reached.
*
*  @b width may be one of the following:
*  - a number specifying the minimum width of the field
*  - * (asterick) means that an integer taken from the argument list will 
*    be used to provide the width. The @a width argument must precede the
*    value being formatted in the argument list.
*
*  @b precision may be one of the following:
*  - a number 
*  - * (asterick) means that an integer taken from the argument list will 
*    be used to provide the precision. The @a precision argument must 
*    precede the value being formatted in the argument list.
*
*  The interpretation of @a precision depends on the type of field being
*  formatted:
*  - For b, d, o, u, x, X, the precision specifies the minimum number of
*    digits that will be printed. If the number of digits in the argument
*    is less than @a precision, the output value is padded on the left with 
*    zeros. The value is not truncated when the number of digits exceeds 
*    @a prcision.
*  - For s, the precision specifies the maximum number of characters to be
*    printed.
*
*  The optional type modifier l (lowercase ell), may be used to specify
*  that the argument is a long argument. This makes a difference on 
*  architectures where the sizeof an int is different from the sizeof a long.
*
*  @b type causes the output to be formatted as follows:
*  - b Unsigned binary integer.
*  - c Character.
*  - d Signed decimal integer.
*  - o Unsigned octal integer.
*  - s Null terminated character string.
*  - u Unsigned Decimal integer.
*  - x Unsigned hexadecimal integer, using "abcdef".
*  - X Unsigned hexadecimal integer, using "ABCDEF".
*
*  @param   outFunc     (in) Pointer to function to call to output a character.
*  @param   outParm     (in) Passed to @a outFunc.
*  @param   fmt         (in) Format string (ala printf, descrtibed above).
*  @param   args        (in) Variable length list of arguments.
*
*  @return  The number of characters successfully output, or a negative number
*           if an error occurred.
*/

int vStrXPrintf
(
   StrXPrintfFunc   outFunc,
   void            *outParm, 
   const prog_char *fmt, 
   va_list          args
)
{
   Parameters  p;
   char        controlChar;

   p.numOutputChars  = 0;
   p.outFunc         = outFunc;
   p.outParm         = outParm;

   controlChar = pgm_read_byte( fmt++ );

   while ( controlChar != '\0' )
   {
      if ( controlChar == '%' )
      {
         short precision = -1;
         short longArg   = 0;
         short base      = 0;

         controlChar = pgm_read_byte( fmt++ );
         p.minFieldWidth = 0;
         p.leadingZeros  = 0;
         p.options       = NO_OPTION;
         
         SetOption( &p, RIGHT_JUSTIFY );

         /*
          * Process [flags]
          */

         if ( controlChar == '-' )
         {
            ClearOption( &p, RIGHT_JUSTIFY );
            controlChar = pgm_read_byte( fmt++ );
         }

         if ( controlChar == '0' )
         {
            SetOption( &p, ZERO_PAD );
            controlChar = pgm_read_byte( fmt++ );
         }

         /*
          * Process [width]
          */

         if ( controlChar == '*' )
         {
            p.minFieldWidth = (short)va_arg( args, int );
            controlChar = pgm_read_byte( fmt++ );
         }
         else
         {
            while (( '0' <= controlChar ) && ( controlChar <= '9' ))
            {
               p.minFieldWidth =
                  p.minFieldWidth * 10 + controlChar - '0';
               controlChar = pgm_read_byte( fmt++ );
            }
         }

         /*
          * Process [.precision]
          */

         if ( controlChar == '.' )
         {
            controlChar = pgm_read_byte( fmt++ );
            if ( controlChar == '*' )
            {
               precision = (short)va_arg( args, int );
               controlChar = pgm_read_byte( fmt++ );
            }
            else
            {
               precision = 0;
               while (( '0' <= controlChar ) && ( controlChar <= '9' ))
               {
                  precision = precision * 10 + controlChar - '0';
                  controlChar = pgm_read_byte( fmt++ );
               }
            }
         }

         /*
          * Process [l]
          */

         if ( controlChar == 'l' )
         {
            longArg = 1;
            controlChar = pgm_read_byte( fmt++ );
         }

         /*
          * Process type.
          */

         if ( controlChar == 'd' )
         {
            base = 10;
         }
         else 
         if ( controlChar == 'x' )
         {
            base = 16;
         }
         else 
         if ( controlChar == 'X' )
         {
            base = 16;
            SetOption( &p, CAPITAL_HEX );
         }
         else 
         if ( controlChar == 'u' )
         {
            base = 10;
         }
         else
         if ( controlChar == 'o' )
         {
            base = 8;
         }
         else 
         if ( controlChar == 'b' )
         {
            base = 2;
         }
         else 
         if ( controlChar == 'c' )
         {
            base = -1;
            ClearOption( &p, ZERO_PAD );
         }
         else
         if ( controlChar == 's' )
         {
            base = -2;
            ClearOption( &p, ZERO_PAD );
         }

         if ( base == 0 )  /* invalid conversion type */
         {
            if ( controlChar != '\0' )
            {
               OutputChar( &p, controlChar );
               controlChar = pgm_read_byte( fmt++ );
            }
         }
         else
         {
            if ( base == -1 )  /* conversion type c */
            {
               char c = (char)va_arg( args, int );
               p.editedStringLen = 1;
               OutputField( &p, &c );
            }
            else if ( base == -2 )  /* conversion type s */
            {
               char *string = va_arg( args, char * );

               p.editedStringLen = 0;
               while ( string[ p.editedStringLen ] != '\0' )
               {
                  if (( precision >= 0 ) && ( p.editedStringLen >= precision ))
                  {
                     /*
                      * We don't require the string to be null terminated
                      * if a precision is specified.
                      */

                     break;
                  }
                  p.editedStringLen++;
               }
               OutputField( &p, string );
            }
            else  /* conversion type d, b, o or x */
            {
               unsigned long x;

               /*
                * Worst case buffer allocation is required for binary output,
                * which requires one character per bit of a long.
                */

               char buffer[ CHAR_BIT * sizeof( unsigned long ) + 1 ];

               p.editedStringLen = 0;
               if ( longArg )
               {
                  x = va_arg( args, unsigned long );
               }
               else
               if ( controlChar == 'd' )
               {
                  x = va_arg( args, int );
               }
               else
               {
                  x = va_arg( args, unsigned );
               }

               if (( controlChar == 'd' ) && ((long) x < 0 ))
               {
                  SetOption( &p, MINUS_SIGN );
                  x = - (long) x;
               }

               do 
               {
                  int c;
                  c = x % base + '0';
                  if ( c > '9' )
                  {
                     if ( IsOptionSet( &p, CAPITAL_HEX ))
                     {
                        c += 'A'-'9'-1;
                     }
                     else
                     {
                        c += 'a'-'9'-1;
                     }
                  }
                  buffer[ sizeof( buffer ) - 1 - p.editedStringLen++ ] = (char)c;
               }
               while (( x /= base ) != 0 );

               if (( precision >= 0 ) && ( precision > p.editedStringLen ))
               {
                  p.leadingZeros = precision - p.editedStringLen;
               }
               OutputField( &p, buffer + sizeof(buffer) - p.editedStringLen );
            }
            controlChar = pgm_read_byte( fmt++ );
         }
      }
      else
      {
         /*
          * We're not processing a % output. Just output the character that
          * was encountered.
          */

         OutputChar( &p, controlChar );
         controlChar = pgm_read_byte( fmt++ );
      }
   }
   return p.numOutputChars;

} // vStrXPrintf

/** @} */

/**
 * @addtogroup StrPrintfInternal
 * @{
 */

/***************************************************************************/
/**
*  Outputs a single character, keeping track of how many characters have
* been output.
*
*  @param   p     (mod) State information.
*  @param   c     (in)  Character to output.
*/

static void OutputChar( Parameters *p, int c )
{
   if ( p->numOutputChars >= 0 )
   {
      int n = (*p->outFunc)(p->outParm, c);

      if ( n >= 0 )
      {
         p->numOutputChars++;
      }
      else
      {
         p->numOutputChars = n;
      }
   }

} // OutputChar

/***************************************************************************/
/**
*  Outputs a formatted field. This routine assumes that the field has been
*  converted to a string, and this routine takes care of the width 
*  options, leading zeros, and any leading minus sign.
*
*  @param   p     (mod) State information.
*  @param   s     (in)  String to output.
*/

static void OutputField( Parameters *p, char *s )
{
   short padLen = p->minFieldWidth - p->leadingZeros - p->editedStringLen;

   if ( IsOptionSet( p, MINUS_SIGN ))
   {
      if ( IsOptionSet( p, ZERO_PAD ))
      {
         /*
          * Since we're zero padding, output the minus sign now. If we're space
          * padding, we wait until we've output the spaces.
          */

         OutputChar( p, '-' );
      }

      /*
       * Account for the minus sign now, even if we are going to output it
       * later. Otherwise we'll output too much space padding.
       */

      padLen--;
   }

   if ( IsOptionSet( p, RIGHT_JUSTIFY ))
   {
      /*
       * Right justified: Output the spaces then the field.
       */

      while ( --padLen >= 0 )
      {
         OutputChar( p, p->options & ZERO_PAD ? '0' : ' ' );
      }
   }
   if ( IsOptionSet( p, MINUS_SIGN ) && IsOptionClear( p, ZERO_PAD ))
   {
      /*
       * We're not zero padding, which means we haven't output the minus
       * sign yet. Do it now.
       */

      OutputChar( p, '-' );
   }

   /*
    * Output any leading zeros.
    */

   while ( --p->leadingZeros >= 0 )
   {
      OutputChar( p, '0' );
   }

   /*
    * Output the field itself.
    */

   while ( --p->editedStringLen >= 0 )
   {
      OutputChar( p, *s++ );
   }

   /*
    * Output any trailing space padding. Note that if we output leading
    * padding, then padLen will already have been decremented to zero.
    */

   while ( --padLen >= 0 )
   {
      OutputChar( p, ' ' );
   }

} // OutputField

/***************************************************************************/
/**
*  Helper function, used by vStrPrintf() (and indirectly by StrPrintf()) 
*  for outputting characters into a user supplied buffer.
*
*  @param   outParm  (mod) Pointer to StrPrintfParms structure.
*  @param   ch       (in)  Character to output.
*
*  @return  1 if the character was stored successfully, -1 if the buffer
*           was overflowed.
*/

static int StrPrintfFunc( void *outParm, int ch )
{
   StrPrintfParms *strParm = (StrPrintfParms *)outParm;

   if ( strParm->maxLen > 0 )
   {
      *strParm->str++ = (char)ch;
      *strParm->str = '\0';
      strParm->maxLen--;

      return 1;
   }

   /*
    * Whoops. We ran out of space.
    */

   return -1;

} // StrPrintfFunc

