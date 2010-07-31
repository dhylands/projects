/****************************************************************************
*
*  Avoid.c
*
*  AvoidEdge and AvoidTilt
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include "Avoid.h"
#include "Debug.h"
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"
#include "Puts.h"

// ---- Private Constants and Types -----------------------------------------
// ---- Private Variables ---------------------------------------------------
// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------

/***************************************************************************/
/**
*  AvoidEdge
*
*  Backs away from the edge
*/

extern void AvoidEdge( uns8 lineSide )
{
   uns8  i;

   // Back up a bit

   DBG( puts( "Avoid Edge: Backing up\n" ); )

   gSpeedL = SPEED_BWD;
   gSpeedR = SPEED_BWD;

   for ( i = 0; i < 60; i++ ) 
   {
      PulseMotors();
      WaitForTimer0Rollover();
   }

   DBG( puts( "Avoid Edge: Turning\n" ); )
   
   // Turn away from the line

   if (( lineSide & LINE_DETECTED_LEFT ) != 0 )
   {
      gSpeedL = SPEED_FWD;
   }
   else
   {
      gSpeedR = SPEED_FWD;
   }

   for ( i = 0; i < 60; i++ ) 
   {
      PulseMotors();
      WaitForTimer0Rollover();
   }

} // AvoidEdge

#if 0
/****************************************************************************
*
*  AvoidTilt
*
*  Backs away from being tilted
*/

extern void AvoidTilt( void )
{
   if (( gTiltState & TILTED_LEFT ) != 0 )
   {
      DBG( puts( "Tilted Left\n" ); )

      gSpeedL = SPEED_BWD;
      gSpeedR = SPEED_FWD;
   }
   else
   if (( gTiltState & TILTED_RIGHT ) != 0 )
   {
      DBG( puts( "Tilted Right\n" ); )
   
      gSpeedL = SPEED_FWD;
      gSpeedR = SPEED_BWD;
   }
   else
   if (( gTiltState & TILTED_BACK ) != 0 )
   {
      DBG( puts( "Tilted Back\n" ); )

      if ( gHuntDir == HUNT_RIGHT )
      {
         // Target is probably to the right. Back away
         // to the right.

         gSpeedL = SPEED_BWD;
         gSpeedR = SPEED_FWD;
      }
      else
      {
         // Target is probably to the left. Back away
         // to the left.

         gSpeedL = SPEED_FWD;
         gSpeedR = SPEED_BWD;
      }
   }
   else
   {
      DBG( puts( "Tilt Unknown\n" ); )
   
      gSpeedL = SPEED_FWD;
      gSpeedR = SPEED_BWD;
   }

   DBG( puts( "AvoidTilt: Turning away\n" ); )

   for ( i = 0; i < 30; i++ ) 
   {
      PulseMotors();
      WaitForTimer0Rollover();
   }

   DBG( puts( "AvoidTilt: Backing up\n" ); )

   gSpeedL = SPEED_BWD;
   gSpeedR = SPEED_BWD;
   
   for ( i = 0; i < 30; i++ ) 
   {
      PulseMotors();
      WaitForTimer0Rollover();
   }

} // AvoidTilt

#endif
