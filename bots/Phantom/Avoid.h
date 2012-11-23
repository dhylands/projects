/****************************************************************************
*
*  Avoid.h
*
*  AvoidEdge and AvoidTilt
*
****************************************************************************/

#if !defined( AVOID_H )
#define  AVOID_H

#if !defined( TYPES_H )
#   include  "Types.h"
#endif

#define  LINE_DETECTED_LEFT   0x01
#define  LINE_DETECTED_RIGHT  0x02

#define  HUNT_LEFT            1
#define  HUNT_RIGHT           2
#define  SWAP_HUNT_DIR(dir)   (3 - (dir))

void AvoidEdge( uns8 lineSide );
void AvoidTilt( void );

#endif   // AVOID_H

