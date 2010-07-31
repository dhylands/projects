/****************************************************************************
*
*  Types.h
*
****************************************************************************/

#if !defined( TYPES_H )
#define TYPES_H

#if !defined( __INTTYPES_H_ )
#include <inttypes.h>
#endif

typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;

typedef uint8_t     uns8;
typedef uint16_t    uns16;
typedef uint32_t    uns32;

#define TRUE    1
#define FALSE   0

#define ClearBits( val, bits )  ( (val) &= ~(bits) )
#define SetBits( val, bits )    ( (val) |= (bits) )

#endif  // TYPES_H
